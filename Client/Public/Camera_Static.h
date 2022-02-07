#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)
class CCamera_Static :
    public CCamera
{
	explicit CCamera_Static(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Static(const CCamera_Static& rhs);
	virtual ~CCamera_Static() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_float fTimeDelta);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	static CCamera_Static* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

