#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;

protected:
	_bool				m_isCloned = false;

public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END