#include "stdafx.h"
#include "PortalControl.h"
#include "Camera_Player.h"
#include "GameInstance.h"
#include "Cam_Portal.h"
#include "Impact.h"

CPortalControl* CPortalControl::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CPortalControl* pInstance = new CPortalControl(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
    return pInstance;
}

CGameObject* CPortalControl::Clone(void* pArg)
{
	CPortalControl* pInstance = new CPortalControl(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CPortalControl::Free()
{
	Safe_Release(m_pPortal_Orange);
	Safe_Release(m_pPortal_Blue);
	__super::Free();
}

CPortalControl::CPortalControl(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CPortalControl::CPortalControl(const CPortalControl& rhs)
	:CGameObject(rhs)
{
}

HRESULT CPortalControl::NativeConstruct_Prototype()
{
    return S_OK;
}

HRESULT CPortalControl::NativeConstruct(void* pArg)
{
	if (FAILED(SetUp_UI()))
		return E_FAIL;

    return S_OK;
}

_int CPortalControl::Tick(_float fTimeDelta)
{
	if (!m_Vaild)
		return 0;

	Animate_Gun(fTimeDelta);

	
	if(m_pPortal_Blue && m_pPortal_Orange)
	{
		m_pPortal_Blue->Set_Cam_Angle(m_camera->Get_CameraTransform());
		m_pPortal_Orange->Set_Cam_Angle(m_camera->Get_CameraTransform());
	}

    return _int();
}

_int CPortalControl::LateTick(_float fTimeDelta)
{
	if (!m_Vaild)
		return 0;

    return _int();
}

HRESULT CPortalControl::Render()
{
	if (!m_Vaild)
		return S_OK;

    return S_OK;
}

HRESULT CPortalControl::Spawn_Portal(CPortal::PORTALCOLOR iIndex)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_float3 m_vRayDirCH, m_vRayPosCH;
	_float4		vTargetPos = { 0.f,0.f,0.f,1.f };

	_float4x4		ProjMatrix;
	const CCamera::CAMERADESC camDesc = m_camera->Get_Desc();
	D3DXMatrixPerspectiveFovLH(&ProjMatrix, camDesc.fFovy, camDesc.fAspect, camDesc.fNear, camDesc.fFar);
	D3DXMatrixInverse(&ProjMatrix, nullptr, &ProjMatrix);
	D3DXVec4Transform(&vTargetPos, &vTargetPos, &ProjMatrix);
	memcpy(&m_vRayDirCH, &(vTargetPos - _float4(0.f, 0.f, 0.f, 1.f)), sizeof(_float3));

	m_vRayPosCH = _float3(0.f, 0.f, 0.f);

	/* ???????????? ???? ?????? ????????. */
	/* ???????? * ???? */
	const _float4x4		ViewMatrixInverse = m_camera->Get_CameraTransform()->Get_WorldMatrix();
	D3DXVec3TransformNormal(&m_vRayDirCH, &m_vRayDirCH, &ViewMatrixInverse);
	D3DXVec3TransformCoord(&m_vRayPosCH, &m_vRayPosCH, &ViewMatrixInverse);

	D3DXVec3Normalize(&m_vRayDirCH, &m_vRayDirCH);

	pGameInstance->StopSound(CSoundMgr::EFFECT2);
	pGameInstance->Play_Sound(iIndex == CPortal::PORTAL_ORANGE ? TEXT("Portal_Orange_Fire.mp3") : TEXT("Portal_Blue_Fire.mp3"), CSoundMgr::EFFECT2, 1.f);

	list<CCollision_Manager::COLLPOINT> hitList = pGameInstance->Get_Ray_Collision_List(m_vRayDirCH, m_vRayPosCH, 150.f, true);

	if (hitList.empty() || (hitList.size() == 1 && hitList.front().CollObj->Get_Type() == OBJ_PLAYER))
	{
		RELEASE_INSTANCE(CGameInstance);
		return S_OK;
	} 

	_float3 point, normal;
	for (auto& target : hitList)
	{
		if (target.CollObj->Get_Type() == CGameObject::OBJ_PLAYER)
			continue;

		if (target.CollObj->Get_Type() !=  CGameObject::OBJ_STATIC || static_cast<CBoxCollider*>(target.CollObj->Get_Component(COM_COLLIDER))->Get_CollStyle() == CCollider::COLLSTYLE_ONLYRAY)
		{
			RELEASE_INSTANCE(CGameInstance);
			return S_OK;
		}

		point = target.Point;
		normal = target.NormalVec;

		if(iIndex == CPortal::PORTAL_ORANGE)
			m_pPortal_Orange_UI->Set_CurrFrameIndex(1);
		else
			m_pPortal_Blue_UI->Set_CurrFrameIndex(1);

		break;
	}

	if (iIndex == CPortal::PORTAL_ORANGE)
	{
		if (nullptr != m_pPortal_Orange)
		{
			if (nullptr != m_pPortal_Blue)
				m_pPortal_Blue->Link_Portal(nullptr);

			m_pPortal_Orange->Link_Portal(nullptr);
			Safe_Release(m_pPortal_Orange);
			pGameInstance->Release_GameObject(g_CurrLevel, TEXT("Portal_Orange"), m_pPortal_Orange);
			m_pPortal_Orange = nullptr;
		}
	}
	else
	{
		if (nullptr != m_pPortal_Blue)
		{
			if (nullptr != m_pPortal_Orange)
				m_pPortal_Orange->Link_Portal(nullptr);

			m_pPortal_Blue->Link_Portal(nullptr);
			Safe_Release(m_pPortal_Blue);
			pGameInstance->Release_GameObject(g_CurrLevel, TEXT("Portal_Blue"), m_pPortal_Blue);
			m_pPortal_Blue = nullptr;
		}
	}

	CPortal::PORTALDESC portalDesc;
	portalDesc.iLevel = g_CurrLevel;
	portalDesc.iPortalColor = iIndex;
	portalDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	portalDesc.vEye = point + normal * 0.01f;
	portalDesc.vAt = point - normal;
	portalDesc.portalCam = iIndex == CPortal::PORTAL_ORANGE ? TEXT("Portal_Orange") : TEXT("Portal_Blue");

	if (FAILED(pGameInstance->Add_GameObject(g_CurrLevel, portalDesc.portalCam, TEXT("Prototype_GameObject_Portal"), &portalDesc)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	if(iIndex == CPortal::PORTAL_ORANGE)
	{
		m_pPortal_Orange = static_cast<CPortal*>(pGameInstance->Get_GameObject(g_CurrLevel, portalDesc.portalCam, 0));
		Safe_AddRef(m_pPortal_Orange);
	}
	else
	{
		m_pPortal_Blue = static_cast<CPortal*>(pGameInstance->Get_GameObject(g_CurrLevel, portalDesc.portalCam, 0));
		Safe_AddRef(m_pPortal_Blue);
	}

	RELEASE_INSTANCE(CGameInstance);

	Spawn_Effect(iIndex, portalDesc.vEye);


	if (FAILED(Synchronize_Camera_Angle()))
		return E_FAIL;

    return S_OK;
}

HRESULT CPortalControl::Erase_Portal()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr != m_pPortal_Blue)
	{
		if (nullptr != m_pPortal_Orange)
		{
			m_pPortal_Orange->Link_Portal(nullptr);
		}
		m_pPortal_Blue->Link_Portal(nullptr);
		Safe_Release(m_pPortal_Blue);
		pGameInstance->Release_GameObject(g_CurrLevel, TEXT("Portal_Blue"), m_pPortal_Blue);
		m_pPortal_Blue = nullptr;
	}

	if (nullptr != m_pPortal_Orange)
	{
		if (nullptr != m_pPortal_Blue)
		{
			m_pPortal_Blue->Link_Portal(nullptr);
		}
		m_pPortal_Orange->Link_Portal(nullptr);
		Safe_Release(m_pPortal_Orange);
		pGameInstance->Release_GameObject(g_CurrLevel, TEXT("Portal_Orange"), m_pPortal_Orange);
		m_pPortal_Orange = nullptr;
	}

	m_pPortal_Blue_UI->Set_CurrFrameIndex(0);
	m_pPortal_Orange_UI->Set_CurrFrameIndex(0);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CPortalControl::Set_Camera(CCamera* _cam)
{
	m_camera = _cam;	
}

HRESULT CPortalControl::SetUp_UI()
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	CUI::UIDESC desc, desc2, desc3;
	ZeroMemory(&desc, sizeof(desc));
	ZeroMemory(&desc2, sizeof(desc2));
	ZeroMemory(&desc3, sizeof(desc3));

	desc.WinCX = g_iWinCX;
	desc.WinCY = g_iWinCY;
	desc.Layer = 2;
	desc.FrameCount = 2;
	desc.Alpha = CUI::ALPHA_BLEND;
	desc.PosX = g_iWinCX * 0.45f;
	desc.PosY = g_iWinCY * 0.50f;
	desc.SizeX = 70.f;
	desc.SizeY = 70.f;
	desc.Style = CUI::STYLE_FIX;
	desc.Texture = TEXT("Prototype_Component_Texture_Portal_Orange_UI");
	desc.Shader_Style = SHADER_SETCOLOR_BLEND;
	desc.Shader_Control = &g_ControlShader;

	if (FAILED(p_instance->Add_GameObject(LEVEL_STAGEONE, TEXT("Portal_Orange_UI"), PROTO_UI, &desc)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_pPortal_Orange_UI = static_cast<CUI*>(p_instance->Get_GameObject(LEVEL_STAGEONE, TEXT("Portal_Orange_UI")));

	desc2.WinCX = g_iWinCX;
	desc2.WinCY = g_iWinCY;
	desc2.Layer = 2;
	desc2.FrameCount = 2;
	desc2.Alpha = CUI::ALPHA_BLEND;
	desc2.PosX = g_iWinCX * 0.55f;
	desc2.PosY = g_iWinCY * 0.50f;
	desc2.SizeX = 70.f;
	desc2.SizeY = 70.f;
	desc2.Style = CUI::STYLE_FIX;
	desc2.Texture = TEXT("Prototype_Component_Texture_Portal_Blue_UI");
	desc2.Shader_Style = SHADER_SETCOLOR_BLEND;
	desc2.Shader_Control = &g_ControlShader;

	if (FAILED(p_instance->Add_GameObject(LEVEL_STAGEONE, TEXT("Portal_Blue_UI"), PROTO_UI, &desc2)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_pPortal_Blue_UI = static_cast<CUI*>(p_instance->Get_GameObject(LEVEL_STAGEONE, TEXT("Portal_Blue_UI")));


	desc3.WinCX = g_iWinCX;
	desc3.WinCY = g_iWinCY;
		
	desc3.Layer = 2;
	desc3.Alpha = CUI::ALPHA_BLEND;
	desc3.PosX = g_iWinCX / 2;//g_iWinCX * 0.90f;
	desc3.PosY = g_iWinCY / 2;//g_iWinCY * 0.82f;
	desc3.SizeX = 500.f;
	desc3.SizeY = 350.f;
	desc3.Style = CUI::STYLE_FIX;
	desc3.Texture = TEXT("Prototype_Component_Texture_Portal_Gun_UI");
	desc3.Shader_Style = SHADER_SETCOLOR_BLEND;
	desc3.Shader_Control = &g_ControlShader;


	m_fGun_fx = desc3.PosX;
	m_fGun_fy = desc3.PosY;

	if (FAILED(p_instance->Add_GameObject(LEVEL_STAGEONE, TEXT("Portal_Gun_UI"), PROTO_UI, &desc3)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_pPortal_Gun_UI = static_cast<CUI*>(p_instance->Get_GameObject(LEVEL_STAGEONE, TEXT("Portal_Gun_UI")));

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPortalControl::Synchronize_Camera_Angle()
{
	if (m_pPortal_Orange && m_pPortal_Blue)
	{
		if (!m_pPortal_Orange->Get_Link_Portal())
		{
			m_pPortal_Orange->Link_Portal(m_pPortal_Blue);
			m_pPortal_Blue->Link_Portal(m_pPortal_Orange);
		}
	}
	return S_OK;
}

void CPortalControl::Animate_Gun(_float fTimeDelta)
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	if(p_instance->Get_Key_Press(DIK_W) || p_instance->Get_Key_Press(DIK_D) || p_instance->Get_Key_Press(DIK_A) || p_instance->Get_Key_Press(DIK_S))
	{
		m_fFrWalk += fTimeDelta * 400.f; 
	}
	else
	{
		m_fFrWalk -= fTimeDelta * 400.f;
	}

	if (m_fFrWalk >= 180.f || m_fFrWalk <= 0.f)
		m_fFrWalk = 0.f;

	if (p_instance->Get_Mouse_Down(CInput_Device::MBS_RBUTTON) || p_instance->Get_Mouse_Down(CInput_Device::MBS_LBUTTON))
	{
		m_fFrShoot += fTimeDelta * 700.f;
	}
	else
	{
		m_fFrShoot -= fTimeDelta * 200.f;
	}

	if (m_fFrShoot >= 180.f || m_fFrShoot <= 0.f)
		m_fFrShoot = 0.f;


	m_pPortal_Gun_UI->Set_Pos(m_fGun_fx + sinf(D3DXToRadian(m_fFrShoot)) * m_fGun_fx * 0.3f, m_fGun_fy + sinf(D3DXToRadian(m_fFrShoot)) * m_fGun_fy * 0.3f + sinf(D3DXToRadian(m_fFrWalk)) * m_fGun_fy * 0.1f);

	RELEASE_INSTANCE(CGameInstance);
}

void CPortalControl::Spawn_Effect(_uint _index, _float3 _pos)
{
	CImpact::IMPACT Impact1;
	ZeroMemory(&Impact1, sizeof(Impact1));
	Impact1.Position = _pos;
	Impact1.Size = _float3(0.05f, 0.05f, 0.05f);
	Impact1.RandomDirection = 5.f;
	Impact1.SpreadSpeed = 10.f;
	Impact1.DeleteTime = 0.3f;//rand() % 5 + 2;
	Impact1.Change = true;
	if (_index == CPortal::PORTAL_ORANGE)
	{
		Impact1.Color = _float4(1.0f, 0.8f, 0.0f, 0.0f);
		Impact1.EndColor = _float4(0.0f, 0.05f, 0.0f, 0.0f);
	}

	else
	{
		Impact1.Color = _float4(0.0f, 0.2f, 1.f, 0.0f);
		Impact1.EndColor = _float4(0.0f, 0.05f, 0.0001f, 0.0f);
	}

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	for (int i = 0; i < rand() % 10 + 25; ++i)
	{
		if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Impact"), TEXT("Prototype_GameObject_Impact"), &Impact1)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
	}
	RELEASE_INSTANCE(CGameInstance);
}

void CPortalControl::Set_Vaild(_bool _bool)
{
	m_Vaild = _bool;
	m_pPortal_Orange_UI->Set_Vaild(m_Vaild);
	m_pPortal_Blue_UI->Set_Vaild(m_Vaild);
	m_pPortal_Gun_UI->Set_Vaild(m_Vaild);
}
