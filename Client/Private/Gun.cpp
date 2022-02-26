#include "stdafx.h"
#include "Gun.h"
#include "GameInstance.h"
#include "UI.h"
#include "Camera_Player.h"
#include "Ball.h"

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

	mt19937 ranX(rd());
	mt19937 ranY(rd());

	const uniform_int_distribution<_uint> spread(0, m_iCurrSpread); 

	_float randomPos[2];

	randomPos[0] = spread(ranX);
	randomPos[1] = spread(ranY);

	for(auto& i : randomPos)
	{
		if ((int)i % 2 == 0)
			i *= -1.f; 
	}

	m_iCurrSpread += m_iSpread;

	_float4		vTargetPos = { randomPos[0] * 0.001f,randomPos[1] * 0.001f,0.f,1.f};
	//_float4		vTargetPos = { 0.5f,0.5f,0.f,1.f };

	/* 뷰스페이스 상의 위치로 변환한다. */
	/* 로컬위치 * 월드 * 뷰 */

	_float4x4		ProjMatrix;
	const CCamera::CAMERADESC camDesc = m_camera_->Get_Desc();
	D3DXMatrixPerspectiveFovLH(&ProjMatrix, camDesc.fFovy, camDesc.fAspect, camDesc.fNear, camDesc.fFar);
	D3DXMatrixInverse(&ProjMatrix, nullptr, &ProjMatrix);
	D3DXVec4Transform(&vTargetPos, &vTargetPos, &ProjMatrix);
	memcpy(&m_vRayDirCH, &(vTargetPos - _float4(0.f, 0.f, 0.f, 1.f)), sizeof(_float3));

	m_vRayPosCH = _float3(0.f, 0.f, 0.f);

	/* 월드스페이스 상의 위치로 변환한다. */
	/* 로컬위치 * 월드 */
	const _float4x4		ViewMatrixInverse = m_camera_->Get_CameraTransform()->Get_WorldMatrix();
	D3DXVec3TransformNormal(&m_vRayDirCH, &m_vRayDirCH, &ViewMatrixInverse);
	D3DXVec3TransformCoord(&m_vRayPosCH, &m_vRayPosCH, &ViewMatrixInverse);

	D3DXVec3Normalize(&m_vRayDirCH, &m_vRayDirCH);

	p_instance->StopSound(CSoundMgr::SYSTEM_EFFECT2);
	p_instance->Play_Sound(TEXT("Rifle_Fire.mp3"), CSoundMgr::SYSTEM_EFFECT2, 1.0f);

	list<CGameObject*>* hitList = p_instance->Get_Ray_Collision_List(m_vRayDirCH, m_vRayPosCH, m_fRange);

	if (hitList->empty())
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	const _float3 pos = static_cast<CTransform*>(hitList->front()->Get_Component(COM_TRANSFORM))->Get_State(CTransform::STATE_POSITION);

	//충돌 처리하기
	if (FAILED(p_instance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Bullet"), TEXT("Prototype_GameObject_Ball"))))
		return;

	CGameObject* p_ball = p_instance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Bullet"), m_test++);

	CTransform* tr = static_cast<CTransform*>(p_ball->Get_Component(COM_TRANSFORM));
	tr->Scaled(_float3(0.5f,0.5f,0.5f));
	tr->Set_State(CTransform::STATE_POSITION, pos);

	hitList->clear();
	delete hitList;

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
