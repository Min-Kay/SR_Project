#include "stdafx.h"
#include "..\Public\Camera_Dynamic.h"
#include "GameInstance.h"

CCamera_Dynamic::CCamera_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Dynamic::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Dynamic::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CCamera_Dynamic::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
	{
		m_pTransform->Go_Straight(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
	{
		m_pTransform->Go_BackWard(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
	{
		m_pTransform->Go_Left(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
	{
		m_pTransform->Go_Right(fTimeDelta);
	}

	_long		MouseMove = 0;

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_X))
	{
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMove * 0.1f);
	}

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y))
	{
		m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
	}

	RELEASE_INSTANCE(CGameInstance);

	return __super::Tick(fTimeDelta);		
}

_int CCamera_Dynamic::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	return _int();
}

HRESULT CCamera_Dynamic::Render()
{
	return S_OK;
}

CCamera_Dynamic * CCamera_Dynamic::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Dynamic*	pInstance = new CCamera_Dynamic(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CCamera_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Dynamic::Clone(void * pArg)
{
	CCamera_Dynamic*	pInstance = new CCamera_Dynamic(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CCamera_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Dynamic::Free()
{
	__super::Free();



}
