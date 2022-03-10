#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CUI;
END

BEGIN(Client)
class CCamera_Player;
class CGun :
    public CGameObject
{
protected:
	explicit CGun(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGun(const CGun& rhs);
	virtual ~CGun() = default; 

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;

public:
	void Set_Vaild(_bool _bool);
	void Set_OnFire(_bool _bool);
public:
	void Shoot(_float fTimeDelta);
	void Rebound(_float fTimeDelta);
	void Reload();

private:
	void Fire();
	void SynchroBullet();

	void Spark(_float3 _point);
	void Spawn_BulletHole(_float3 _point, _float3 _nor);
private:
	HRESULT SetUp_UI();
	void Animate(_float fTimeDelta);

private:
	CCamera_Player* m_camera_ = nullptr;
private:
	CUI*	m_pGun_UI = nullptr;
	CUI*	m_pMuzzle_UI = nullptr;
	CUI*	m_pGunIcon = nullptr;

	_float m_fGun_fx = 0.f;
	_float m_fGun_fy = 0.f;

	_bool m_Vaild = true;



	CUI* m_pCurrBullet_UI_1 = nullptr;
	CUI* m_pCurrBullet_UI_2 = nullptr;
	CUI* m_pFullBullet_UI_1 = nullptr;
	CUI* m_pFullBullet_UI_2 = nullptr;
	CUI* m_pSlash_UI = nullptr;

	_uint m_iCurrUnits = 0;
	_uint m_iCurrTens = 0;

private:
	_float m_fFrWalk = 0.f; 
	_float m_fFrShoot = 0.f;

	_uint m_iCurrBulletCount = 0;
	_uint m_iFullBulletCount = 60;

	_int m_iCurrSpread = 0;
	_uint m_iSpread = 3;

	_float m_fRange = 30.f; 

	_float m_fRebound = 1.2f;

	_float m_fTickShoot = 0.f;
	_float m_fTickSpread = 0.f;

	_float m_fTickReload = 0.f;

	_bool m_Reloading = false;
	_bool m_OnFire = false;

private:
	_int m_iDamage = 5;
private:
	_float3 m_vRayDirCH;
	_float3 m_vRayPosCH; 


private:
	random_device rd;

public:
	static CGun* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END

