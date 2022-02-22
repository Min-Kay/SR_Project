#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCamera;
END

BEGIN(Client)
class CFPS :
    public CGameObject
{
protected:
	explicit CFPS(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CFPS(const CFPS& rhs);
	virtual ~CFPS() = default;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	static CFPS* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CFPS* Clone(void* pArg) override;
	virtual void Free() override;

private:
    //CCamera* m_pCamera = nullptr;
};

END
