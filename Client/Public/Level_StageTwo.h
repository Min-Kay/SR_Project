#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CLevel_StageTwo final : public CLevel
{
public:
	explicit CLevel_StageTwo(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_StageTwo() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_float fTimeDelta);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Map();
	HRESULT Open_Exit();

private:
	_int iLineX = 25;
	_int iLineY = 10;
	_int iLineZ = 25;
	_int iWaterZ = 5;

	_float fDoorSizeX = 3.f;
	_float fDoorSizeY = 3.f;


	_float	m_iBoxSize = 80.f; //<-여기 바꾸면 전체크기다 바뀜 
	_float HalfBoxSize = m_iBoxSize * 0.5f;
	_float WallSize = 1.f;//벽의 두께
	_float HalfWallSize = WallSize * 0.5;//벽의 두께


private:
	_bool m_setting = false;

public:
	static CLevel_StageTwo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END