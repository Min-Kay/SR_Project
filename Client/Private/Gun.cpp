#include "stdafx.h"
#include "Gun.h"
#include "GameInstance.h"
#include "UI.h"
#include "Camera_Player.h"

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

	Animate(fTimeDelta);
	m_fTickShoot += fTimeDelta;

	return 0;
}

_int CGun::LateTick(_float fTimeDelta)
{
	if (!m_Vaild)
		return 0;

	if (0 > __super::LateTick(fTimeDelta))
		return -1;
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
}

void CGun::Shoot(_float fTimeDelta)
{
	if(m_fTickShoot > 0.1f && m_iCurrBulletCount > 0 )
	{
		Fire();
		Rebound(fTimeDelta);
		--m_iCurrBulletCount;
		m_fTickShoot = 0.f;
	}
}

void CGun::Fire()
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->StopSound(CSoundMgr::SYSTEM_EFFECT2);
	p_instance->Play_Sound(TEXT("Rifle_Fire.mp3"), CSoundMgr::SYSTEM_EFFECT2,1.0f);
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
	 m_iCurrBulletCount = m_iFullBulletCount;
}

HRESULT CGun::SetUp_UI()
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	CUI::UIDESC desc;
	ZeroMemory(&desc,sizeof(desc));
	desc.WinCX = g_iWinCX;
	desc.WinCY = g_iWinCY;

	desc.Layer = 2;
	desc.FrameCount = 2;
	desc.Alpha = CUI::ALPHA_BLEND;
	desc.PosX = g_iWinCX * 0.8f;
	desc.PosY = g_iWinCY * 0.78f;
	desc.SizeX = 500.f;
	desc.SizeY = 350.f;
	desc.Style = CUI::STYLE_FIX;
	desc.Texture = TEXT("Prototype_Component_Texture_Gun_UI");


	m_fGun_fx = desc.PosX;
	m_fGun_fy = desc.PosY;


	if (FAILED(p_instance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Gun_UI"), PROTO_UI, &desc)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_pGun_UI = static_cast<CUI*>(p_instance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Gun_UI")));

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

	if (p_instance->Get_Mouse_Press(CInput_Device::MBS_LBUTTON) && m_iCurrBulletCount > 0)
	{
		m_fFrShoot += fTimeDelta * 2000.f;
	}
	else
	{
		m_fFrShoot -= fTimeDelta * 1800.f;
	}

	if (m_fFrShoot >= 180.f || m_fFrShoot <= 0.f)
		m_fFrShoot = 0.f;


	m_pGun_UI->Set_Pos(m_fGun_fx + sinf(D3DXToRadian(m_fFrShoot)) * m_fGun_fx * 0.05f, m_fGun_fy + sinf(D3DXToRadian(m_fFrShoot)) * m_fGun_fy * 0.05f + sinf(D3DXToRadian(m_fFrWalk)) * m_fGun_fy * 0.1f);

	RELEASE_INSTANCE(CGameInstance);
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
