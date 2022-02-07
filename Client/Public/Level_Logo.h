#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Logo final : public CLevel
{
public:
	explicit CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Logo() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_float fTimeDelta);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Prototype_GameObject();
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);

public:
	static CLevel_Logo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END