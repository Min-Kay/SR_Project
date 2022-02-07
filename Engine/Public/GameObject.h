#pragma once

#include "Base.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_float fTimeDelta);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg = nullptr);

protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;

protected:
	CComponent*	Find_Component(const _tchar* pComponentTag);

protected:
	map<const _tchar*, CComponent*>			m_Components;
	typedef map<const _tchar*, CComponent*>	COMPONENTS;


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END