#include "stdafx.h"
#include "..\Public\Missile.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "Renderer.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"
#include "Player.h"
#include "Arm.h"
#include "Impact.h"
#include "Tageting.h"

CMissile::CMissile(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CGameObject(m_pGraphic_Device)
{
}

CMissile::CMissile(const CMissile & rhs)
	: CGameObject(rhs)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pBoxCollider(rhs.m_pBoxCollider)
	, m_pRendererCom(rhs.m_pRendererCom)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pVIBufferCom(rhs.m_pVIBufferCom)
{
	Safe_AddRef(m_pTransformCom);
	Safe_AddRef(m_pBoxCollider);
	Safe_AddRef(m_pRendererCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pVIBufferCom);

}
HRESULT CMissile::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMissile::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
	{
		MSGBOX("미사일 셋업오류")
		return E_FAIL;
	}

	m_pTransformCom->Scaled(_float3(0.2f, 0.2f, 0.2f));
	CTransform* PlayerTrans = (CTransform*)m_pPlayer->Get_Component(COM_TRANSFORM);
	PlayerPos = PlayerTrans->Get_State(CTransform::STATE_POSITION);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));
	MissilePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	/*int i = (rand() % 10)%2 +1;
	switch (i)
	{
	case 1:
	RandPos1 = _float3(MissilePos.x + rand() % 30 + 10, MissilePos.y + 3, MissilePos.z + rand() % 20 + 5);
	break;
	case 2:
	RandPos1 = _float3(MissilePos.x - rand() % 30 + 10, MissilePos.y + 3, MissilePos.z - rand() % 20 + 5);
	break;
	}*/
	RandPos1 = _float3(MissilePos.x + rand() % 30 + 10, MissilePos.y, MissilePos.z);
	RandPos2 = _float3(PlayerPos.x - rand() % 30 - 10, PlayerPos.y, PlayerPos.z);
	CreateCount = 0;


	return S_OK;
}

_int CMissile::Tick(_float fTimeDelta)
{
	if (Get_Dead())
		return 0;

	if (0 > __super::Tick(fTimeDelta))
		return -1;
	m_pBoxCollider->Set_Collider();

	timer += fTimeDelta * 0.5f;

	if (timer / 5 >= 1.0f)
	{
		m_bDEAD = true;
		timer = 0;
		return 0;
	}
	else
	{
		testPos = TestTturning(MissilePos, RandPos1, RandPos2, PlayerPos, timer / 5);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, testPos);
		m_pTransformCom->LookAt(PlayerPos);
	}




	


	return _int();
}

_int CMissile::LateTick(_float fTimeDelta)
{
	if (Get_Dead())
		return 0;

	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	
	m_ftimer += fTimeDelta;

	if (m_ftimer >= 2)
	{
		m_ftimer = 0.f;
		//CreateCount = 0;
	}
	else
	{
		//if (CreateCount == 0)
		//{
		CImpact::IMPACT Impact1;
		ZeroMemory(&Impact1, sizeof(Impact1));
		Impact1.Pos = testPos;
		Impact1.Pos.z = testPos.z - 0.5f;;
		Impact1.Size = _float3(0.05f, 0.05f, 0.05f);
		Impact1.randomPos = 3;
		Impact1.Speed = 5;
		Impact1.deleteCount = 2;//rand() % 5 + 2;
		Impact1.DeleteImpact = false;

		Impact1.Gradation = CImpact::GRADATION_DOWN;
		Impact1.Color = D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.0f);
		Impact1.ChangeColor = D3DXCOLOR(0.0f, 0.1f, 0.0f, 0.0f);

		//++CreateCount;

		//for (int i = 0; i < rand() % 5 + 2; ++i)
		//{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		if (FAILED(p_instance->Add_GameObject(LEVEL_STAGETWO, TEXT("Impact"), TEXT("Prototype_GameObject_Impact"), &Impact1)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return E_FAIL;
		}
		RELEASE_INSTANCE(CGameInstance);
		
	}
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	if (CreateCount == 0)
	{
		if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Missile"), TEXT("Prototype_GameObject_Tageting") )))
		{
			RELEASE_INSTANCE(CGameInstance);
			return -1;
		}
		++CreateCount;
	}
	RELEASE_INSTANCE(CGameInstance);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}



HRESULT CMissile::Render()
{
	if (Get_Dead())
		return 0;


	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (m_bDEAD)
		Set_Dead(true);

	return S_OK;

}


HRESULT CMissile::SetUp_Component()
{
	CTransform::TRANSFORMDESC desc;
	desc.fRotationPerSec = 90.f;
	desc.fSpeedPerSec = 5.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransformCom, &desc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pBoxCollider)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_CUBE, COM_BUFFER, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CubeAttack"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	m_pBoxCollider->Set_ParentInfo(this);
	Set_Type(OBJ_INTERACTION);
	m_pBoxCollider->Set_CollStyle(CCollider::COLLSTYLE_TRIGGER);
	m_pBoxCollider->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, 1.f, 1.f));
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	m_pArm = static_cast<CArm*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("Arm")));
	m_pPlayer = static_cast<CPlayer*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("Layer_Player")));
	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

_float3 CMissile::TestTturning(_float3 P0, _float3 P1, _float3 P2, _float3 P3, _float time)
{

	_float3 vTesult1;
	_float3 vTesult2;
	_float3 vTesult3;
	_float3 vTesult4;
	_float3 vTesult5;
	_float3 vTesult6;

	D3DXVec3Lerp(&vTesult1, &P0, &P1, time);
	D3DXVec3Lerp(&vTesult2, &P1, &P2, time);
	D3DXVec3Lerp(&vTesult3, &P2, &P3, time);

	D3DXVec3Lerp(&vTesult4, &vTesult1, &vTesult2, time);
	D3DXVec3Lerp(&vTesult5, &vTesult2, &vTesult3, time);

	D3DXVec3Lerp(&vTesult6, &vTesult4, &vTesult5, time);
	return vTesult6;
}

CMissile * CMissile::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CMissile* pMissile = new CMissile(m_pGraphic_Device);
	if (FAILED(pMissile->NativeConstruct_Prototype()))
	{
		MSGBOX("Fail Create Missle")
			Safe_Release(pMissile);
		return nullptr;
	}
	return pMissile;
}

CGameObject * CMissile::Clone(void * pArg)
{
	CMissile* pMissile = new CMissile(*this);
	if (FAILED(pMissile->NativeConstruct(pArg)))
	{
		MSGBOX("Fail Create Missle_Clone")
			Safe_Release(pMissile);
		return nullptr;
	}
	return pMissile;
}

void CMissile::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
