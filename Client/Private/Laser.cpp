#include "stdafx.h"
#include "Laser.h"

#include "Transform.h"
#include "Renderer.h"
#include "BoxCollider.h"
#include "Texture.h"
#include "Shader.h"
#include "VIBuffer_Cube.h"

#include "GameInstance.h"
#include "Player.h"
CLaser::CLaser(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CEnemy(m_pGraphic_Device)
{
	
}

CLaser::CLaser(const CLaser& rhs)
	:CEnemy(rhs)
	,m_pShader(rhs.m_pShader)
	,m_pCollider(rhs.m_pCollider)
	,m_pTransform(rhs.m_pTransform)
	,m_pTexture(rhs.m_pTexture)
	,m_pBuffer(rhs.m_pBuffer)
	,m_pRenderer(rhs.m_pRenderer)
{
	Safe_AddRef(m_pShader);
	Safe_AddRef(m_pBuffer);
	Safe_AddRef(m_pCollider);
	Safe_AddRef(m_pRenderer);
	Safe_AddRef(m_pTexture);
	Safe_AddRef(m_pTransform);
}

HRESULT CLaser::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CLaser::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	return S_OK;
}

_int CLaser::Tick(_float fTimeDelta)
{
	if (fTimeDelta <= 0.f)
		return 0;

	m_fTimer += fTimeDelta;

	if(m_DeathTimer <= m_fTimer)
	{
		m_pCollider->Set_Dead(true);
		Set_Dead(true);
	}

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	Move(fTimeDelta);
	m_pCollider->Set_Collider();
	Check_Player_Hit(fTimeDelta);
	return 0;
}

_int CLaser::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return 0;
}

HRESULT CLaser::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	m_pTransform->Bind_OnShader(m_pShader);
	m_pShader->SetUp_ValueOnShader("g_ColorStack", &g_ControlShader, sizeof(_float));
	m_pTexture->Bind_OnShader(m_pShader, "g_Texture", 0);
	m_pShader->Begin_Shader(SHADER_SETCOLOR_BLEND);
	m_pBuffer->Render();
	m_pShader->End_Shader();
	

	return S_OK;
}

void CLaser::Move(_float fTimeDelta)
{
	switch(m_Dir)
	{
	case 0:
		m_pTransform->Go_Straight(fTimeDelta * m_Speed);
		break;
	case 1:
		m_pTransform->Go_BackWard(fTimeDelta * m_Speed);
		break;
	case 2:
		m_pTransform->Go_Left(fTimeDelta * m_Speed);
		break;
	case 3:
		m_pTransform->Go_Right(fTimeDelta * m_Speed);
		break;
	case 4:
		m_pTransform->Go_Up(fTimeDelta * m_Speed);
		break;
	case 5:
		m_pTransform->Go_Down(fTimeDelta * m_Speed);
		break;
	}
}

void CLaser::Check_Player_Hit(_float fTimeDelta)
{
	if (m_hit || !m_pPlayerCollider)
		return;

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	if(p_instance->Get_Collide(m_pCollider,m_pPlayerCollider))
	{
		m_hit = true;
		m_pPlayer->Add_Hp(-m_Damage);
	}
	
	RELEASE_INSTANCE(CGameInstance);

}

HRESULT CLaser::SetUp_Component()
{
	CTransform::TRANSFORMDESC desc;
	desc.fRotationPerSec = 90.f;
	desc.fSpeedPerSec = 5.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransform, &desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pCollider)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_CUBE, COM_BUFFER, (CComponent**)&m_pBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRenderer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Laser"), COM_TEXTURE, (CComponent**)&m_pTexture)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_SHADER_CUBE, COM_SHADER, (CComponent**)&m_pShader)))
		return E_FAIL;

	Set_Type(OBJ_ENEMY);

	m_pCollider->Set_ParentInfo(this);
	m_pCollider->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, 1.f, 1.f));
	m_pCollider->Set_CollStyle(CCollider::COLLSTYLE_TRIGGER);

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_OBJ,m_pCollider);
	RELEASE_INSTANCE(CGameInstance);

	m_Damage = 3;

	return S_OK;
}

void CLaser::Add_HP(_int _add)
{
	
}

void CLaser::Set_Player(CPlayer* _player)
{
	m_pPlayer = _player;
	m_pPlayerCollider = static_cast<CBoxCollider*>(m_pPlayer->Get_Component(COM_COLLIDER));
}

void CLaser::Set_Direction(_uint _dir)
{

	if (_dir >= 6)
		return;

	m_Dir = _dir;
}

CLaser* CLaser::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CLaser* p_laser = new CLaser(m_pGraphic_Device);

	if(FAILED(p_laser->NativeConstruct_Prototype()))
	{
		Safe_Release(p_laser);
		return nullptr;
	}

	return p_laser;
}

CGameObject* CLaser::Clone(void* pArg)
{
	CLaser* p_laser = new CLaser(*this);

	if (FAILED(p_laser->NativeConstruct(pArg)))
	{
		Safe_Release(p_laser);
		return nullptr;
	}

	return p_laser;
}

void CLaser::Free()
{
	__super::Free();

	Safe_Release(m_pShader);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pCollider);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pTransform);


}
