#pragma once

#include "Base.h"

/* 객체들의 원형을 생성해서 모아놓는다. */
/* 원형을 찾아서 복제한후, 모아서 관리한다. */

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
public:
	CObject_Manager();
	virtual ~CObject_Manager() = default;
public:
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex);
public:
	HRESULT Reserve_Container(_uint iNumLevels);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype); /* 원형객체를 추가한다. */
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg); /* 사본(게임내에 출현해야할 객체) 객체를 추가한다. */
	HRESULT Clear_LevelObject(_uint iLevelIndex);
public:
	_int Tick(_float fTimeDelta);
	_int LateTick(_float fTimeDelta);
private:
	map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef map<const _tchar*, class CGameObject*>	PROTOTYPES;
private:
	map<const _tchar*, class CLayer*>*			m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>	LAYERS;

	_uint			m_iNumLevels = 0;

private:
	class CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	virtual void Free() override;
};

END