#pragma once

#include "Client_Defines.h"
#include "Level.h"
#

BEGIN(Client)
class CTile_Cube;
class CBoss;
class CTileCollider;
class CPlayer;
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
	HRESULT Ready_Layer_JumpMap();
	HRESULT Open_Exit();
	HRESULT Save_Point();

	HRESULT Close_Exit();


private:
	void Spawn_Boss_Tile(_float fTimeDelta);
	void Change_Boss_Tile(_float fTimeDelta);
	void Gradiant(_float fTimeDelta);


private:
	_int iLineX = 25;
	_int iLineY = 10;
	_int iLineZ = 25;
	_int iWaterZ = 5;

	_float fDoorSizeX = 20.f;
	_float fDoorSizeY = 20.f;


	_float	m_iBoxSize = 80.f; //<-여기 바꾸면 전체크기다 바뀜 
	_float HalfBoxSize = m_iBoxSize * 0.5f;
	_float WallSize = 5.f;//벽의 두께
	_float HalfWallSize = WallSize * 0.5f;//벽의 두께

										  //점프맵 크기 설정
	_float    m_iJumpBoxSize = 20.f; //<-여기 바꾸면 전체크기다 바뀜 
	_float JumpHalfBoxSize = m_iJumpBoxSize * 0.5f;
	_float JumpWallSize = 1.f;//벽의 두께
	_float JumpHalfWallSize = (_float)JumpWallSize * 0.5f;//벽의 두께
private:
	_bool m_setting = false;

	CTile_Cube* m_EventCube_Save_Exit = nullptr;
	CTile_Cube* m_EventCube_Open = nullptr;

	_bool m_Open_2 = false;
	_bool m_Save = false;
	_bool m_BossSpone = false;


private:
	_bool m_ChangedTile = false;
	_bool m_GradianChangeTile = false;

	_bool m_ChangedTile2 = false;
	_bool m_Changed = false;

	_float m_Timer = 0.f;
	_float m_CubeTimer = 0.f;

	_float m_Gradiant = false;
	_bool m_GrowColor = false;
	_float m_GradiantTime = 0.f;


	CBoss* m_pBoss = nullptr;
	CPlayer* m_pPlayer = nullptr;
	list<CTileCollider*> m_TileList;

public:
	static CLevel_StageTwo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END