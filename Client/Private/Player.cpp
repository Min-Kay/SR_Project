#include "stdafx.h"
#include "..\Public\Player.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "PortalControl.h"
#include "Gun.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	m_Info = *static_cast<PLAYERINFO*>(pArg);

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(SetUp_Weapons()))
		return E_FAIL;

	
	Set_Type(OBJ_PLAYER);
	Set_Hp(m_Info.Hp);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Info.Pos);

	m_beforeHp = m_HP;
	if (FAILED(SetUp_UI()))
		return E_FAIL;

	g_ControlShader = -1.f;
	m_PlayerDead = true;

	return S_OK;
}

_int CPlayer::Tick(_float fTimeDelta)
{
	if (Get_Dead())
		return 0;

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	Setting_Dying(fTimeDelta);

	if(!m_PlayerDead)
	{
		Player_Control(fTimeDelta);

		Check_OnGround();

		if (m_bJump)
			Tick_JumpState(fTimeDelta);
		m_pTransformCom->Gravity(1.f, fTimeDelta);

		m_pTransformCom->Add_Force(fTimeDelta);

		if (m_pBoxColliderCom)
			m_pBoxColliderCom->Set_Collider();
	}

	return _int();
}

_int CPlayer::LateTick(_float fTimeDelta)
{
	if (Get_Dead())
		return 0;

	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (nullptr == m_pTransformCom)
		return -1;


	if (m_HP <= 0)
	{
		m_PlayerDead = true;
	}

	if (FAILED(Synchronize_Camera(fTimeDelta)))
		return -1;

	Setting_HpUi(fTimeDelta);

	return _int();
}

HRESULT CPlayer::Render()
{

	if (Get_Dead())
		return 0;

	return S_OK;
}

HRESULT CPlayer::Set_Cam(CCamera* cam)
{
	m_Camera = cam;
	static_cast<CCamera_Player*>(m_Camera)->Set_Player(this);
	return S_OK;
}

HRESULT CPlayer::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RECT, COM_BUFFER, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Box */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pBoxColliderCom)))
		return E_FAIL;

	Set_Type(OBJ_PLAYER);
	m_pBoxColliderCom->Set_ParentInfo(this);
	m_pBoxColliderCom->Set_State(CBoxCollider::COLLIDERINFO::COLL_SIZE, _float3(0.5f, 1.f, 0.5f));

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_OBJ, m_pBoxColliderCom);
	Set_Cam(p_instance->Find_Camera_Object(MAIN_CAM));
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CPlayer::Tick_JumpState(_float fTimeDelta)
{
	_float3 m_vJumpPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);

	m_vJumpPos += *D3DXVec3Normalize(&vUp, &vUp) * fTimeDelta * m_fForce * m_pTransformCom->Get_Gravity();
	m_CurrForce += m_fForce;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vJumpPos);

	if (m_CurrForce >= m_fMaxForce )
	{
		m_bJump = false;
		m_CurrForce = 0.f;
	}
}

HRESULT CPlayer::Reset_PlayerPos(_float3 resetPos)
{
	m_Info.Pos = resetPos;
	return S_OK;
}

void CPlayer::Set_Shake(_float _timer, _float _force)
{
	m_ShakeTime = _timer;
	m_ShakeForce = _force;
	m_Shake = true;
}

void CPlayer::Erase_Portal()
{
	if (!m_pPortalCtrl)
		return;

	m_pPortalCtrl->Erase_Portal();
}

void CPlayer::Set_UIColor(_float4 _color)
{
	m_PlayerHP_1->Set_Color(_color);
	m_PlayerHP_2->Set_Color(_color);
	m_PlayerHP_3->Set_Color(_color);
	m_PlayerFullHP_1->Set_Color(_color);
	m_PlayerFullHP_2->Set_Color(_color);
	m_PlayerFullHP_3->Set_Color(_color);
	m_pSlash_UI->Set_Color(_color);
	m_pGun->Set_UIColor(_color);
}

const _int& CPlayer::Get_Hp() const
{
	return m_HP;
}

void CPlayer::Set_Hp(_int _hp)
{
	m_HP = _hp;
}

void CPlayer::Add_Hp(_int _add)
{
	if (m_PlayerDead)
		return;

	if(!m_Invincible)
		m_HP += _add;

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->StopSound(CSoundMgr::PLAYER_EFFECT3);
	p_instance->Play_Sound(rand() % 2 ? TEXT("hurt_0.mp3") : TEXT("hurt_1.mp3"), CSoundMgr::PLAYER_EFFECT3, 1.f);
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CPlayer::SetUp_UI()
{
	/* player_hp_Image*/
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	CUI::UIDESC PlayerHpUi;
	ZeroMemory(&PlayerHpUi, sizeof(PlayerHpUi));
	PlayerHpUi.WinCX = g_iWinCX;
	PlayerHpUi.WinCY = g_iWinCY;

	PlayerHpUi.Layer = 2;
	PlayerHpUi.FrameCount = 0;
	PlayerHpUi.Alpha = CUI::ALPHA_BLEND;
	PlayerHpUi.PosX = g_iWinCX * 0.05f;
	PlayerHpUi.PosY = g_iWinCY * 0.9f;
	PlayerHpUi.SizeX = 150.f;
	PlayerHpUi.SizeY = 150.f;
	PlayerHpUi.AnimateSpeed = 30.f;
	PlayerHpUi.Style = CUI::STYLE_FIX;
	PlayerHpUi.Texture = TEXT("Prototype_Component_PlayerUI");
	PlayerHpUi.Shader_Control = &g_ControlShader;
	PlayerHpUi.Shader_Style = SHADER_SETCOLOR_BLEND;


	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("PlayerHp_Image"), PROTO_UI, &PlayerHpUi)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}



	/* Playerlhp_Font_100*/

	CUI::UIDESC PlayerHP_Font100;
	ZeroMemory(&PlayerHP_Font100, sizeof(PlayerHP_Font100));
	PlayerHP_Font100.WinCX = g_iWinCX;
	PlayerHP_Font100.WinCY = g_iWinCY;

	PlayerHP_Font100.Layer = 2;
	PlayerHP_Font100.FrameCount = 10;
	PlayerHP_Font100.Alpha = CUI::ALPHA_BLEND;
	PlayerHP_Font100.PosX = g_iWinCX * 0.1f;
	PlayerHP_Font100.PosY = g_iWinCY * 0.9f;
	PlayerHP_Font100.SizeX = 50.f;
	PlayerHP_Font100.SizeY = 50.f;
	PlayerHP_Font100.AnimateSpeed = 100.f;
	PlayerHP_Font100.Style = CUI::STYLE_FIX;
	PlayerHP_Font100.Texture = TEXT("Prototype_Component_Texture_Font");
	PlayerHP_Font100.Shader_Control = &g_ControlShader;
	PlayerHP_Font100.Shader_Style = SHADER_SETCOLOR_BLEND;

	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("PlayerHP_Font100"), PROTO_UI, &PlayerHP_Font100)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_PlayerHP_1 = static_cast<CUI*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("PlayerHP_Font100")));

	/* Playerlhp_Font_10*/

	CUI::UIDESC PlayerHP_Font10;
	ZeroMemory(&PlayerHP_Font10, sizeof(PlayerHP_Font10));
	PlayerHP_Font10.WinCX = g_iWinCX;
	PlayerHP_Font10.WinCY = g_iWinCY;

	PlayerHP_Font10.Layer = 2;
	PlayerHP_Font10.FrameCount = 10;
	PlayerHP_Font10.Alpha = CUI::ALPHA_BLEND;
	PlayerHP_Font10.PosX = g_iWinCX * 0.13f;
	PlayerHP_Font10.PosY = g_iWinCY * 0.9f;
	PlayerHP_Font10.SizeX = 50.f;
	PlayerHP_Font10.SizeY = 50.f;
	PlayerHP_Font10.AnimateSpeed = 100.f;
	PlayerHP_Font10.Style = CUI::STYLE_FIX;
	PlayerHP_Font10.Texture = TEXT("Prototype_Component_Texture_Font");
	PlayerHP_Font10.Shader_Control = &g_ControlShader;
	PlayerHP_Font10.Shader_Style = SHADER_SETCOLOR_BLEND;

	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("PlayerHP_Font10"), PROTO_UI, &PlayerHP_Font10)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_PlayerHP_2 = static_cast<CUI*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("PlayerHP_Font10")));

	/* Playerlhp_Font_1*/

	CUI::UIDESC PlayerHP_Font1;
	ZeroMemory(&PlayerHP_Font1, sizeof(PlayerHP_Font1));
	PlayerHP_Font1.WinCX = g_iWinCX;
	PlayerHP_Font1.WinCY = g_iWinCY;

	PlayerHP_Font1.Layer = 2;
	PlayerHP_Font1.FrameCount = 10;
	PlayerHP_Font1.Alpha = CUI::ALPHA_BLEND;
	PlayerHP_Font1.PosX = g_iWinCX * 0.16f;
	PlayerHP_Font1.PosY = g_iWinCY * 0.9f;
	PlayerHP_Font1.SizeX = 50.f;
	PlayerHP_Font1.SizeY = 50.f;
	PlayerHP_Font1.AnimateSpeed = 100.f;
	PlayerHP_Font1.Style = CUI::STYLE_FIX;
	PlayerHP_Font1.Texture = TEXT("Prototype_Component_Texture_Font");
	PlayerHP_Font1.Shader_Control = &g_ControlShader;
	PlayerHP_Font1.Shader_Style = SHADER_SETCOLOR_BLEND;

	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("PlayerHP_Font1"), PROTO_UI, &PlayerHP_Font1)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_PlayerHP_3 = static_cast<CUI*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("PlayerHP_Font1")));

	/* Font 고정 slash*/

	CUI::UIDESC PlayerHp_slash;
	ZeroMemory(&PlayerHp_slash, sizeof(PlayerHp_slash));
	PlayerHp_slash.WinCX = g_iWinCX;
	PlayerHp_slash.WinCY = g_iWinCY;
	PlayerHp_slash.Layer = 2;
	PlayerHp_slash.FrameCount = 0;
	PlayerHp_slash.Alpha = CUI::ALPHA_BLEND;
	PlayerHp_slash.PosX = g_iWinCX * 0.195f;
	PlayerHp_slash.PosY = g_iWinCY * 0.9f;
	PlayerHp_slash.SizeX = 50.f;
	PlayerHp_slash.SizeY = 50.f;
	PlayerHp_slash.AnimateSpeed = 100.f;
	PlayerHp_slash.Style = CUI::STYLE_FIX;
	PlayerHp_slash.Texture = TEXT("Prototype_Component_Texture_Slash");
	PlayerHp_slash.Shader_Control = &g_ControlShader;
	PlayerHp_slash.Shader_Style = SHADER_SETCOLOR_BLEND;

	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("PlayerHp_slash"), PROTO_UI, &PlayerHp_slash)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_pSlash_UI = static_cast<CUI*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("PlayerHp_slash")));



	/* PlayerFullhp_Font_100*/

	CUI::UIDESC PlayerFullHP_Font100;
	ZeroMemory(&PlayerFullHP_Font100, sizeof(PlayerFullHP_Font100));
	PlayerFullHP_Font100.WinCX = g_iWinCX;
	PlayerFullHP_Font100.WinCY = g_iWinCY;

	PlayerFullHP_Font100.Layer = 2;
	PlayerFullHP_Font100.FrameCount = 10;
	PlayerFullHP_Font100.Alpha = CUI::ALPHA_BLEND;
	PlayerFullHP_Font100.PosX = g_iWinCX * 0.22f;
	PlayerFullHP_Font100.PosY = g_iWinCY * 0.9f;
	PlayerFullHP_Font100.SizeX = 50.f;
	PlayerFullHP_Font100.SizeY = 50.f;
	PlayerFullHP_Font100.AnimateSpeed = 100.f;
	PlayerFullHP_Font100.Style = CUI::STYLE_FIX;
	PlayerFullHP_Font100.Texture = TEXT("Prototype_Component_Texture_Font");
	PlayerFullHP_Font100.Shader_Control = &g_ControlShader;
	PlayerFullHP_Font100.Shader_Style = SHADER_SETCOLOR_BLEND;

	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("PlayerFullHP_Font100"), PROTO_UI, &PlayerFullHP_Font100)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_PlayerFullHP_1 = static_cast<CUI*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("PlayerFullHP_Font100")));
	m_PlayerFullHP_1->Set_CurrFrameIndex(1);

	/* PlayerFullhp_Font_10*/
	CUI::UIDESC PlayerFullHP_Font10;
	ZeroMemory(&PlayerFullHP_Font10, sizeof(PlayerFullHP_Font10));
	PlayerFullHP_Font10.WinCX = g_iWinCX;
	PlayerFullHP_Font10.WinCY = g_iWinCY;

	PlayerFullHP_Font10.Layer = 2;
	PlayerFullHP_Font10.FrameCount = 10;
	PlayerFullHP_Font10.Alpha = CUI::ALPHA_BLEND;
	PlayerFullHP_Font10.PosX = g_iWinCX * 0.25f;
	PlayerFullHP_Font10.PosY = g_iWinCY * 0.9f;
	PlayerFullHP_Font10.SizeX = 50.f;
	PlayerFullHP_Font10.SizeY = 50.f;
	PlayerFullHP_Font10.AnimateSpeed = 100.f;
	PlayerFullHP_Font10.Style = CUI::STYLE_FIX;
	PlayerFullHP_Font10.Texture = TEXT("Prototype_Component_Texture_Font");
	PlayerFullHP_Font10.Shader_Control = &g_ControlShader;
	PlayerFullHP_Font10.Shader_Style = SHADER_SETCOLOR_BLEND;

	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("PlayerFullHP_Font10"), PROTO_UI, &PlayerFullHP_Font10)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}
	m_PlayerFullHP_2 = static_cast<CUI*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("PlayerFullHP_Font10")));
	m_PlayerFullHP_2->Set_CurrFrameIndex(0);

	/* PlayerFullhp_Font_1*/

	CUI::UIDESC PlayerFullHP_Font1;
	ZeroMemory(&PlayerFullHP_Font1, sizeof(PlayerFullHP_Font1));
	PlayerFullHP_Font1.WinCX = g_iWinCX;
	PlayerFullHP_Font1.WinCY = g_iWinCY;

	PlayerFullHP_Font1.Layer = 2;
	PlayerFullHP_Font1.FrameCount = 10;
	PlayerFullHP_Font1.Alpha = CUI::ALPHA_BLEND;
	PlayerFullHP_Font1.PosX = g_iWinCX * 0.28f;
	PlayerFullHP_Font1.PosY = g_iWinCY * 0.9f;
	PlayerFullHP_Font1.SizeX = 50.f;
	PlayerFullHP_Font1.SizeY = 50.f;
	PlayerFullHP_Font1.AnimateSpeed = 100.f;
	PlayerFullHP_Font1.Style = CUI::STYLE_FIX;
	PlayerFullHP_Font1.Texture = TEXT("Prototype_Component_Texture_Font");
	PlayerFullHP_Font1.Shader_Control = &g_ControlShader;
	PlayerFullHP_Font1.Shader_Style = SHADER_SETCOLOR_BLEND;

	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("PlayerFullHP_Font1"), PROTO_UI, &PlayerFullHP_Font1)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_PlayerFullHP_3 = static_cast<CUI*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("PlayerFullHP_Font1")));
	m_PlayerFullHP_3->Set_CurrFrameIndex(0);


	CUI::UIDESC PlayerHitUI;
	ZeroMemory(&PlayerHitUI, sizeof(PlayerHitUI));
	PlayerHitUI.WinCX = g_iWinCX;
	PlayerHitUI.WinCY = g_iWinCY;

	PlayerHitUI.Layer = 1;
	PlayerHitUI.FrameCount = 9;
	PlayerHitUI.Alpha = CUI::ALPHA_BLEND;
	PlayerHitUI.PosX = g_iWinCX * 0.5;
	PlayerHitUI.PosY = g_iWinCY * 0.5f;
	PlayerHitUI.SizeX = g_iWinCX;
	PlayerHitUI.SizeY = g_iWinCY;
	PlayerHitUI.AnimateSpeed = 100.f;
	PlayerHitUI.Style = CUI::STYLE_FIX;
	PlayerHitUI.Texture = TEXT("Prototype_Component_PlayerHit");
	PlayerHitUI.Shader_Control = &g_ControlShader;
	PlayerHitUI.Shader_Style = SHADER_SETCOLOR_BLEND;


	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Player_hit"), PROTO_UI, &PlayerHitUI)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_pPlayerHit = static_cast<CUI*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("Player_hit")));
	m_pPlayerHit->Set_CurrFrameIndex(0);


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CPlayer::Setting_HpUi(_float ftimedelta)
{

	m_uChageHp = m_HP;
	m_PlayerHP_1->Set_CurrFrameIndex(m_uChageHp / 100);
	m_PlayerHP_2->Set_CurrFrameIndex((m_uChageHp % 100) / 10);
	m_PlayerHP_3->Set_CurrFrameIndex((m_uChageHp % 10));

	//맞으면 쿨타이머 돌리고
	if (m_bHit == true)
		m_fHitCoolTime += ftimedelta;

	//타이머가 시간 지나면 다시 맞을수있게
	if (m_fHitCoolTime >= 1.f)
	{
		m_bHit = false;
		m_fHitCoolTime = 0.f;
		m_pPlayerHit->Set_CurrFrameIndex(0);
	}
	//이전이랑 체력이 다르고 맞을수 있으면 
	if (m_beforeHp != m_uChageHp && m_bHit == false)
	{
		m_bHit = true;
		_int test = (_int)m_uChageHp / 10;

		if (test <= 2)
			test = 1;
		else if (test <= 5)
			test = 2;
		else if (test <= 8)
			test = 3;
		else if (test <= 10)
			test = 4;
		m_pPlayerHit->Set_CurrFrameIndex(test);
		m_beforeHp = m_uChageHp;
	}
}

void CPlayer::Setting_Dying(_float fTimeDelta)
{
	if (!m_PlayerDead)
		return;

	if(!m_FadeOut)
	{
		g_ControlShader -= fTimeDelta;


		if (g_ControlShader <= -1.f)
		{
			m_pPortalCtrl->Erase_Portal();
			m_pGun->Reset();
			m_beforeHp = m_Info.Hp;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_Info.Pos));
			m_HP = m_Info.Hp;
			static_cast<CCamera_Player*>(m_Camera)->Drop_Interaction();
			m_FadeOut = true;
		}
	}
	else
	{
		g_ControlShader += fTimeDelta;
		if (g_ControlShader >= 0.f)
		{
			g_ControlShader = 0.f;
			m_PlayerDead = false;
			m_FadeOut = false;
		}
	}
	

}

HRESULT CPlayer::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Release_RenderState()
{

	return S_OK;
}

_int CPlayer::Player_Control(_float fTimeDelta)
{
	if (m_PlayerDead)
		return 0;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (static_cast<CCamera_Player*>(m_Camera)->Get_Break())
	{
		RELEASE_INSTANCE(CGameInstance);
		return 0;
	}


	if (pGameInstance->Get_Key_Down(DIK_TAB))
	{
		Set_Shake(5.f,1.f);
	}


	if (pGameInstance->Get_Key_Press(DIK_W))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (pGameInstance->Get_Key_Press(DIK_S))
	{
		m_pTransformCom->Go_BackWard(fTimeDelta);
	}

	if (pGameInstance->Get_Key_Press(DIK_A))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (pGameInstance->Get_Key_Press(DIK_D))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	if (pGameInstance->Get_Key_Press(DIK_Q))
	{
		_float3 m_vJumpPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);

		m_vJumpPos += *D3DXVec3Normalize(&vUp, &vUp) * fTimeDelta * m_fMaxForce ;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vJumpPos);
	}

	if (pGameInstance->Get_Key_Press(DIK_W) || pGameInstance->Get_Key_Press(DIK_S) || pGameInstance->Get_Key_Press(DIK_D) || pGameInstance->Get_Key_Press(DIK_A))
		pGameInstance->Play_Sound(TEXT("Walk.mp3"), CSoundMgr::CHANNELID::PLAYER, 1.f);
	else
		pGameInstance->StopSound(CSoundMgr::PLAYER);


	if (!m_bJump && m_OnGround && pGameInstance->Get_Key_Down(DIK_SPACE))
		m_bJump = true;



	if (nullptr != m_pGun && nullptr != m_pPortalCtrl)
	{
		if (m_iCurrIndex != 0 && pGameInstance->Get_Key_Down(DIKEYBOARD_1))
		{
			m_iCurrIndex = 0;
			pGameInstance->StopSound(CSoundMgr::UI);
			pGameInstance->Play_Sound(TEXT("PortalGunSwap.wav"), CSoundMgr::UI, 1.f);
			m_pPortalCtrl->Set_Vaild(true);
			m_pGun->Set_Vaild(false);

		}
		else if (m_iCurrIndex != 1 && pGameInstance->Get_Key_Down(DIKEYBOARD_2))
		{
			m_iCurrIndex = 1;
			pGameInstance->StopSound(CSoundMgr::UI);
			pGameInstance->Play_Sound(TEXT("GunSwap.wav"), CSoundMgr::UI, 1.f);
			m_pPortalCtrl->Set_Vaild(false);
			m_pGun->Set_Vaild(true);
		}
	}

	if (pGameInstance->Get_Key_Down(DIK_5))
	{
		m_Invincible = !m_Invincible;
	}

	
	switch (m_iCurrIndex)
	{
	case 0:
		if (nullptr != m_pPortalCtrl)
		{
			if (pGameInstance->Get_Mouse_Up(CInput_Device::MBS_LBUTTON) && pGameInstance->Get_Mouse_Down(CInput_Device::MBS_RBUTTON))
				break;

			if (pGameInstance->Get_Mouse_Up(CInput_Device::MBS_LBUTTON))
			{
				m_pPortalCtrl->Spawn_Portal(CPortal::PORTAL_ORANGE);
			}
			else if (pGameInstance->Get_Mouse_Down(CInput_Device::MBS_RBUTTON))
			{
				m_pPortalCtrl->Spawn_Portal( CPortal::PORTAL_BLUE);
			}

			if (pGameInstance->Get_Key_Up(DIK_C))
			{
				m_pPortalCtrl->Erase_Portal();
			}
		}
		break;
	case 1:
		if (nullptr != m_pGun)
		{
			if (pGameInstance->Get_Mouse_Press(CInput_Device::MBS_LBUTTON))
			{
				m_pGun->Shoot(fTimeDelta);
			}
			else
				m_pGun->Set_OnFire(false);

			if (pGameInstance->Get_Key_Down(DIK_R))
			{
				m_pGun->Reload();
			}
		}
		break;
	}
	RELEASE_INSTANCE(CGameInstance);
	return 0;
}

HRESULT CPlayer::Synchronize_Camera(_float fTimeDelta)
{
	if (!m_Camera || !m_pTransformCom)
		return E_FAIL;

	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION); 

	if (m_Shake && m_fTimer <= m_ShakeTime)
	{
		m_fTimer += fTimeDelta;
		_float3 vRight, vUp;
		vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);

		_int force = (_int)(m_ShakeForce * 10000);

		_float randRight = _float(rand() % 2 == 0 ? (rand() % force) : -(rand() % force));
		_float randUp = _float(rand() % 2 == 0 ? (rand() % force) : -(rand() % force));

		randRight /= 100000;
		randUp /= 100000;

		vPos += vRight * randRight + vUp * randUp;
	}
	else
	{
		m_fTimer = 0.f;
		m_Shake = false;
	}

	m_Camera->Get_CameraTransform()->Set_State(CTransform::STATE_POSITION, vPos);

	_float3 vRight, vUp, vLook;

	vRight = m_Camera->Get_CameraTransform()->Get_State(CTransform::STATE_RIGHT);
	D3DXVec3Normalize(&vRight, &vRight);

	vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
	vLook = *D3DXVec3Cross(&vLook, &vRight, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);
	_float3 vScale = m_pTransformCom->Get_Scale();

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);

	return S_OK;
}

void CPlayer::Check_OnGround()
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	_float3 vUp = -m_pTransformCom->Get_State(CTransform::STATE_UP);
	D3DXVec3Normalize(&vUp, &vUp);

	list<CCollision_Manager::COLLPOINT> hitList = p_instance->Get_Ray_Collision_List(vUp, m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_Scale().y * 0.5f + 0.01f);

	m_OnGround = (hitList.empty() || (hitList.size() == 1 && hitList.front().CollObj->Get_Type() == OBJ_PLAYER)) ? false : true;


	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CPlayer::SetUp_Weapons()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(g_CurrLevel, TEXT("PortalCtrl"), TEXT("Prototype_GameObject_PortalCtrl"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(g_CurrLevel, TEXT("Gun"), TEXT("Prototype_GameObject_Gun"))))
		return E_FAIL;

	m_pPortalCtrl = static_cast<CPortalControl*>(pGameInstance->Get_GameObject(g_CurrLevel, TEXT("PortalCtrl"), 0));
	m_pPortalCtrl->Set_Camera(m_Camera);
	Safe_AddRef(m_pPortalCtrl);

	m_pGun = static_cast<CGun*>(pGameInstance->Get_GameObject(g_CurrLevel, TEXT("Gun")));
	Safe_AddRef(m_pGun);
	m_pGun->Set_Vaild(false);

	m_iCurrIndex = 0;
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CCamera* CPlayer::Get_Camera()
{
	if (!m_Camera)
		return nullptr;
	return m_Camera;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*	pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone(void* pArg )
{
	/* 새로운객체를 복제하여 생성한다. */
	CPlayer*	pInstance = new CPlayer(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{
	Safe_Release(m_pGun);
	Safe_Release(m_pPortalCtrl);
	__super::Free();

	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
