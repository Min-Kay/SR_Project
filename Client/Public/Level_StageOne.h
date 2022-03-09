#pragma once

#include "ChangeLevel.h"
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CTile_Cube;
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
	HRESULT Ready_Layer_Event();
	HRESULT Ready_Layer_Second_Entrance();
	HRESULT Ready_Layer_Monster_Map();
	HRESULT Ready_Layer_Next_Stage();
	HRESULT Ready_Layer_Ball_Map();
	HRESULT Ready_Layer_Gravity_Map();
private:
	HRESULT Open_Exit();
	HRESULT Close_Exit_Open_Door2();
	HRESULT Open_Exit2();
private:
	_int iLineX = 25;
	_int iLineY = 10;
	_int iLineZ = 25;
	_int iWaterZ = 5;

	_float fDoorSizeX = 3.f;
	_float fDoorSizeY = 3.f;

	_float	m_iBoxSize = 20.f; //<-여기 바꾸면 전체크기다 바뀜 
	_float HalfBoxSize = m_iBoxSize * 0.5f;
	_float WallSize = 5.f;//벽의 두께
	_float HalfWallSize = WallSize * 0.5f;//벽의 두께


	_float	m_iGravityBoxSize = 30.f; //<-여기 바꾸면 전체크기다 바뀜 
	_float GravityhalfBoxSize = m_iGravityBoxSize * 0.5f;
	_float GravityWallSize = 5.f;//벽의 두께
	_float HalfGravityWallSize = GravityWallSize * 0.5f;//벽의 두께

private:
	_bool m_setting = false;

private:
	_bool m_Open_1 = false;
	_bool m_Close_1 = false;
	_bool m_Open_2 = false;
	_bool m_Open_3 = false;

private:
	CTile_Cube* m_EventCube1 = nullptr;
	CTile_Cube* m_EventCube2 = nullptr;
	CTile_Cube* m_EventCube3 = nullptr;

	CChangeLevel* m_Change = nullptr;

	CPlayer* m_player = nullptr;

public:
	static CLevel_StageOne* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END