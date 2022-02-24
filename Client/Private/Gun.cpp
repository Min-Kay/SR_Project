#include "stdafx.h"
#include "Gun.h"

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

	return S_OK;
}

_int CGun::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;
	return 0;
}

_int CGun::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;
	return 0;
}

HRESULT CGun::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
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
