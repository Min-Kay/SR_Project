#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CUI;
END

/* 로더를 개체화하여 보관하낟. */

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
public:
	explicit CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Loading() = default;
public:
	virtual HRESULT NativeConstruct(LEVEL eNextLevel);
	virtual _int Tick(_float fTimeDelta);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready();

private:
	class CLoader*		m_pLoader = nullptr;
	LEVEL				m_eNextLevel = LEVEL_END;

public:
	static CLevel_Loading* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel);
	virtual void Free() override;
};

END