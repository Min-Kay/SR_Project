#pragma once
#include "Client_Defines.h"
#include "Camera.h"
BEGIN(Client)
class CPlayer;

class CCamera_Player :
	public CCamera
{
	explicit CCamera_Player(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Player(const CCamera_Player& rhs);
	virtual ~CCamera_Player() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_float fTimeDelta);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual HRESULT BeforeRender();
	virtual HRESULT AfterRender();

public:
	HRESULT Set_Player(CPlayer* _pPlayer);

private:
	CPlayer* m_pPlayer = nullptr;

public:
	static CCamera_Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

