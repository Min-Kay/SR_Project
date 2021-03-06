#include "..\Public\GameObject.h"
#include "GameInstance.h"
#include "Component.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag)
{
	return Find_Component(pComponentTag);	
}

HRESULT CGameObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::NativeConstruct(void * pArg)
{
	return S_OK;
}

_int CGameObject::Tick(_float fTimeDelta)
{
	if (m_Dead)
		return 0;
	return _int();
}

_int CGameObject::LateTick(_float fTimeDelta)
{
	if (m_Dead)
		return 0;

	return _int();
}

HRESULT CGameObject::Render()
{
	if (m_Dead)
		return S_OK;

	return S_OK;
}

HRESULT CGameObject::Compute_Distance(_float3 _obj, _float3 _target)
{
	if (nullptr == _target)
		return E_FAIL;

	_float3		vDir = _target - _obj;

	m_fDis = D3DXVec3Length(&vDir);

	return S_OK;
}

const _float CGameObject::Get_Distance() const
{
	return m_fDis;
}

void CGameObject::Set_Layer(_uint _layer)
{
	m_Layer = _layer;
}

const _uint CGameObject::Get_Layer() const
{
	return m_Layer;
}

void CGameObject::Set_Type(OBJTYPE _type)
{
	if (_type >= OBJ_END)
		return;

	m_Type = _type; 
}

const CGameObject::OBJTYPE& CGameObject::Get_Type() const
{
	return m_Type; 
}

CGameObject::OBJCULLING CGameObject::Get_CullingInfo()
{
	CTransform* tr = static_cast<CTransform*>(Get_Component(COM_TRANSFORM));

	OBJCULLING info;
	ZeroMemory(&info, sizeof(OBJCULLING));

	if (!tr)
		return info;

	info.Position = tr->Get_State(CTransform::STATE_POSITION);

	_float3 vScale = tr->Get_Scale();
	if(vScale.x > vScale.z)
		info.Radius = vScale.x * 0.5f;
	else
		info.Radius = vScale.z * 0.5f;

	return info; 
}


HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg)
{
	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CComponent*			pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_Components.emplace(pComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTagFinder(pComponentTag));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;	
}

void CGameObject::Free()
{
	Safe_Release(m_pGraphic_Device);

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();
}
