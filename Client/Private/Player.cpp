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

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(SetUp_Weapons()))
		return E_FAIL;

	Set_Type(OBJ_PLAYER);

	
	return S_OK;
}

_int CPlayer::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	Player_Control(fTimeDelta);

	Check_OnGround();

	if (m_bJump)
		Tick_JumpState(fTimeDelta);

	m_pTransformCom->Gravity(1.f, fTimeDelta);

	m_pTransformCom->Add_Force(fTimeDelta);

	if(m_pBoxColliderCom)
		m_pBoxColliderCom->Set_Collider();

	return _int();
}

_int CPlayer::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (nullptr == m_pTransformCom)
		return -1;

	if (FAILED(Synchronize_Camera()))
		return -1;

	return _int();
}

HRESULT CPlayer::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	//m_pBoxColliderCom->Draw_Box();

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

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
	m_pBoxColliderCom->Set_State(CBoxCollider::COLLIDERINFO::COLL_SIZE, _float3(1.f, 1.f, 1.f));

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
	m_HP += _add;
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
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (static_cast<CCamera_Player*>(m_Camera)->Get_Break())
	{
		RELEASE_INSTANCE(CGameInstance);
		return 0;
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

HRESULT CPlayer::Synchronize_Camera()
{
	if (!m_Camera || !m_pTransformCom)
		return E_FAIL;

	m_Camera->Get_CameraTransform()->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

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
