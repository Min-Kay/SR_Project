#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CLevel_StageOne final : public CLevel
{
public:
	explicit CLevel_StageOne(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_StageOne() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_float fTimeDelta);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Map();

private:
	_int iLineX = 25;
	_int iLineY = 10;
	_int iLineZ = 25;
	_int iWaterZ = 5;

	_float fDoorSizeX = 3.f;
	_float fDoorSizeY = 3.f;


public:
	static CLevel_StageOne* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END