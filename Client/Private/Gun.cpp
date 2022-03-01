#include "stdafx.h"
#include "Gun.h"
#include "GameInstance.h"
#include "UI.h"
#include "Camera_Player.h"
#include "Effect.h"
#include "Enemy.h"
#include "Impact.h"

CGun::CGun(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CGun::CGun(const CGun& rhs)
	:CGameObject(rhs)
{
}

HRESULT CGun::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK; 
}

HRESULT CGun::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	SetUp_UI();
	m_iCurrBulletCount = m_iFullBulletCount;

	return S_OK;
}

_int CGun::Tick(_float fTimeDelta)
{
	if (!m_Vaild)
		return 0;

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	m_fTickShoot += fTimeDelta;
	m_fTickSpread += fTimeDelta * 10.f;
	if(m_iCurrSpread > 0 && m_fTickSpread > 0.4f)
	{
		--m_iCurrSpread;
		m_fTickSpread = 0.f; 
	}


	return 0;
}

_int CGun::LateTick(_float fTimeDelta)
{
	if (!m_Vaild)
		return 0;

	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	m_pMuzzle_UI->Set_Vaild(m_OnFire);
	Animate(fTimeDelta);

	return 0;
}

HRESULT CGun::Render()
{
	if (!m_Vaild)
		return S_OK;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CGun::Set_Vaild(_bool _bool)
{
	m_Vaild = _bool;
	m_pGun_UI->Set_Vaild(m_Vaild);
	m_pMuzzle_UI->Set_Vaild(false);
}

void CGun::Set_OnFire(_bool _bool)
{
	m_OnFire = _bool;
}

void CGun::Shoot(_float fTimeDelta)
{
	if (!m_Reloading && m_fTickShoot > 0.1f && m_iCurrBulletCount > 0)
	{
		Set_OnFire(true);
		Fire();
		Rebound(fTimeDelta);
		--m_iCurrBulletCount;
		m_fTickShoot = 0.f;
	}
	else if (m_Reloading || m_iCurrBulletCount <= 0)
		Set_OnFire(false);
}

void CGun::Fire()
{

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	mt19937 ranX(rd());
	mt19937 ranY(rd());

	const uniform_int_distribution<_int> spread(-m_iCurrSpread, m_iCurrSpread);

	_float randomPos[2];

	randomPos[0] = spread(ranX);
	randomPos[1] = spread(ranY);

	m_iCurrSpread += m_iSpread;

	_float4		vTargetPos = { randomPos[0] * 0.001f,randomPos[1] * 0.001f,0.f,1.f};
	//_float4		vTargetPos = { 0.5f,0.5f,0.f,1.f };

	/* �佺���̽� ���� ��ġ�� ��ȯ�Ѵ�. */
	/* ������ġ * ���� * �� */

	_float4x4		ProjMatrix;
	const CCamera::CAMERADESC camDesc = m_camera_->Get_Desc();
	D3DXMatrixPerspectiveFovLH(&ProjMatrix, camDesc.fFovy, camDesc.fAspect, camDesc.fNear, camDesc.fFar);
	D3DXMatrixInverse(&ProjMatrix, nullptr, &ProjMatrix);
	D3DXVec4Transform(&vTargetPos, &vTargetPos, &ProjMatrix);
	memcpy(&m_vRayDirCH, &(vTargetPos - _float4(0.f, 0.f, 0.f, 1.f)), sizeof(_float3));

	m_vRayPosCH = _float3(0.f, 0.f, 0.f);

	/* ���彺���̽� ���� ��ġ�� ��ȯ�Ѵ�. */
	/* ������ġ * ���� */
	const _float4x4		ViewMatrixInverse = m_camera_->Get_CameraTransform()->Get_WorldMatrix();
	D3DXVec3TransformNormal(&m_vRayDirCH, &m_vRayDirCH, &ViewMatrixInverse);
	D3DXVec3TransformCoord(&m_vRayPosCH, &m_vRayPosCH, &ViewMatrixInverse);

	D3DXVec3Normalize(&m_vRayDirCH, &m_vRayDirCH);

	p_instance->StopSound(CSoundMgr::SYSTEM_EFFECT2);
	p_instance->Play_Sound(TEXT("Rifle_Fire.mp3"), CSoundMgr::SYSTEM_EFFECT2, 1.0f);

	list<CCollision_Manager::COLLPOINT> hitList = p_instance->Get_Ray_Collision_List(m_vRayDirCH, m_vRayPosCH, m_fRange);

	if (hitList.empty() || (hitList.size() == 1 && hitList.front().CollObj->Get_Type() == OBJ_PLAYER))
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	CGameObject* hittedObj = nullptr;
	_float3 point,nor; 
	for(auto& target : hitList)
	{
		if (target.CollObj->Get_Type() == CGameObject::OBJ_PLAYER)
			continue;

		hittedObj = target.CollObj;
		point = target.Point;
		nor = target.NormalVec;
		break;
	}

	CImpact::IMPACT Impact1;
	ZeroMemory(&Impact1, sizeof(Impact1));
	Impact1.Pos = point;
	Impact1.Size = _float3(0.1f, 0.1f, 0.1f);
	Impact1.randomPos = 5;
	Impact1.deleteCount = 1;//rand() % 5 + 2;
	Impact1.DeleteImpact = false;
	Impact1.Color = D3DXCOLOR(0.0, 0.1, 0.9, 0.0);

	for (int i = 0; i < 20; ++i)
	{
		if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Impact"), TEXT("Prototype_GameObject_Impact"), &Impact1)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
	}

	if(hittedObj->Get_Type() == OBJ_STATIC)
	{
		CEffect::EFFECTDESC eDesc;
		ZeroMemory(&eDesc, sizeof(eDesc));

		eDesc.Texture = TEXT("Prototype_Component_Texture_Gun_BulletHole");
		eDesc.FrameCount = 4;
		eDesc.Alpha = CEffect::EFFECTALPHA_BLEND;
		eDesc.Bilboard = false;
		eDesc.Style = CEffect::EFFECTSTYLE_FIX;

		if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("BulletHole"), PROTO_EFFECT, &eDesc)))
			return;

		CGameObject* p_ball = p_instance->Get_GameObject(g_CurrLevel, TEXT("BulletHole"), m_test++);
		static_cast<CEffect*>(p_ball)->Set_CurrentFrameIndex(rand() % eDesc.FrameCount);

		CTransform* tr = static_cast<CTransform*>(p_ball->Get_Component(COM_TRANSFORM));
		tr->Scaled(_float3(0.1f, 0.1f, 0.1f));
		tr->Set_State(CTransform::STATE_POSITION, point + nor * 0.01f);
		tr->LookAt(tr->Get_State(CTransform::STATE_POSITION) - nor * 0.1f);
	}
	else if(hittedObj->Get_Type() == OBJ_ENEMY)
	{
		static_cast<CEnemy*>(hittedObj)->Add_HP(-m_iDamage);

	}


	RELEASE_INSTANCE(CGameInstance);
}

void CGun::Rebound(_float fTimeDelta)
{
	if(nullptr != m_camera_)
	{
		_float3 vRight = m_camera_->Get_CameraTransform()->Get_State(CTransform::STATE_RIGHT);
		D3DXVec3Normalize(&vRight, &vRight);
		m_camera_->Locked_Turn(vRight, -fTimeDelta * m_fRebound);
	}
}
void CGun::Reload()
{
	if(!m_Reloading)
	{
		m_pGun_UI->Set_Style(CUI::STYLE_STRAIGHT);
		m_Reloading = true;
	}
}

HRESULT CGun::SetUp_UI()
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	CUI::UIDESC desc;
	ZeroMemory(&desc,sizeof(desc));
	desc.WinCX = g_iWinCX;
	desc.WinCY = g_iWinCY;

	desc.Layer = 2;
	desc.FrameCount = 76;
	desc.Alpha = CUI::ALPHA_BLEND;
	desc.PosX = g_iWinCX * 0.75f;
	desc.PosY = g_iWinCY * 0.65f;
	desc.SizeX = 750.f;
	desc.SizeY = 500.f;
	desc.AnimateSpeed = 30.f;
	desc.Style = CUI::STYLE_FIX;
	desc.Texture = TEXT("Prototype_Component_Texture_Gun_Reload");


	m_fGun_fx = desc.PosX;
	m_fGun_fy = desc.PosY;


	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Gun_UI"), PROTO_UI, &desc)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_pGun_UI = static_cast<CUI*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("Gun_UI")));


	CUI::UIDESC desc2;
	ZeroMemory(&desc2, sizeof(desc2));
	desc2.WinCX = g_iWinCX;
	desc2.WinCY = g_iWinCY;
		
	desc2.Layer = 3;
	desc2.FrameCount = 4;
	desc2.Alpha = CUI::ALPHA_BLEND;
	desc2.PosX = m_fGun_fx - 100.f;
	desc2.PosY = m_fGun_fy - 20.f;
	desc2.SizeX = 400.f;
	desc2.SizeY = 400.f;
	desc2.AnimateSpeed = 100.f;
	desc2.Style = CUI::STYLE_REPEAT;
	desc2.Texture = TEXT("Prototype_Component_Texture_Gun_Muzzle");

	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Muzzle_UI"), PROTO_UI, &desc2)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_pMuzzle_UI = static_cast<CUI*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("Muzzle_UI")));


	m_camera_ = static_cast<CCamera_Player*>(p_instance->Find_Camera_Object(MAIN_CAM));
	
	if (!m_camera_)
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CGun::Animate(_float fTimeDelta)
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	if (p_instance->Get_Key_Press(DIK_W) || p_instance->Get_Key_Press(DIK_D) || p_instance->Get_Key_Press(DIK_A) || p_instance->Get_Key_Press(DIK_S))
	{
		m_fFrWalk += fTimeDelta * 400.f;
	}
	else
	{
		m_fFrWalk -= fTimeDelta * 400.f;
	}

	if (m_fFrWalk >= 180.f || m_fFrWalk <= 0.f)
		m_fFrWalk = 0.f;

	if ( !m_Reloading && p_instance->Get_Mouse_Press(CInput_Device::MBS_LBUTTON) && m_iCurrBulletCount > 0)
	{
		m_fFrShoot += fTimeDelta * 2000.f;
	}
	else
	{
		m_fFrShoot -= fTimeDelta * 1800.f;
	}

	if (m_fFrShoot >= 180.f || m_fFrShoot <= 0.f)
		m_fFrShoot = 0.f;

	RELEASE_INSTANCE(CGameInstance);

	if(m_Reloading)
	{
		if(m_pGun_UI->Get_CurrFrameIndex() >= 75)
		{
			m_Reloading = false;
			m_iCurrBulletCount = m_iFullBulletCount;
			m_pGun_UI->Set_Style(CUI::STYLE_FIX);
			m_pGun_UI->Set_CurrFrameIndex(0);
		}
	}

	m_pGun_UI->Set_Pos(m_fGun_fx + sinf(D3DXToRadian(m_fFrShoot)) * m_fGun_fx * 0.1f, m_fGun_fy + sinf(D3DXToRadian(m_fFrShoot)) * m_fGun_fy * 0.11f + sinf(D3DXToRadian(m_fFrWalk)) * m_fGun_fy * 0.1f);

	m_pMuzzle_UI->Set_Pos(m_fGun_fx - 100.f + sinf(D3DXToRadian(m_fFrShoot)) * m_fGun_fx * 0.1f, m_fGun_fy - 20.f + sinf(D3DXToRadian(m_fFrShoot)) * m_fGun_fy * 0.11f + sinf(D3DXToRadian(m_fFrWalk)) * m_fGun_fy * 0.1f);

}

CGun* CGun::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGun* p_gun = new CGun(pGraphic_Device);
	if(FAILED(p_gun->NativeConstruct_Prototype()))
	{
		Safe_Release(p_gun);
		return nullptr;
	}
	return p_gun;
}

CGameObject* CGun::Clone(void* pArg)
{
	CGun* p_gun = new CGun(*this);
	if (FAILED(p_gun->NativeConstruct(pArg)))
	{
		Safe_Release(p_gun);
		return nullptr;
	}
	return p_gun;
}

void CGun::Free()
{
	__super::Free();
}
