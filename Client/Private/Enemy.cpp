#include "stdafx.h"
#include "Enemy.h"

Client::CEnemy::CEnemy(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CGameObject(m_pGraphic_Device)

{
}

Client::CEnemy::CEnemy(const CEnemy& rhs)
	:CGameObject(rhs)
{

}

HRESULT Client::CEnemy::NativeConstruct_Prototype()
{
	return CGameObject::NativeConstruct_Prototype();
}

HRESULT Client::CEnemy::NativeConstruct(void* pArg)
{
	return CGameObject::NativeConstruct(pArg);
}

_int Client::CEnemy::Tick(_float fTimeDelta)
{
	return CGameObject::Tick(fTimeDelta);
}

_int Client::CEnemy::LateTick(_float fTimeDelta)
{
	return CGameObject::LateTick(fTimeDelta);
}

HRESULT Client::CEnemy::Render()
{
	return CGameObject::Render();
}

void Client::CEnemy::Set_HP(_int _hp)
{
	m_Hp = _hp;
}

void Client::CEnemy::Add_HP(_int _add)
{
	m_Hp += _add;
}

const _int& Client::CEnemy::Get_HP() const
{
	return m_Hp;
}

const _int& Client::CEnemy::Get_Damage() const
{
	return m_Damage;
}

void CEnemy::Set_Portaling(_bool _bool)
{
	m_Portaling = _bool;
}

const _bool& CEnemy::Get_Portaliing() const
{
	return m_Portaling; 
}

_bool Client::CEnemy::Check_HP()
{
	if (m_Hp <= 0)
		return true;
	return false;
}

void CEnemy::Free()
{
	__super::Free();
}
