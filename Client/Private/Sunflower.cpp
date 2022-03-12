#include "stdafx.h"
#include "Sunflower.h"

#include "Renderer.h"
#include "Transform.h"
#include "Shader.h"
#include "VIBuffer_Cube.h"
#include "BoxCollider.h"
#include "Texture.h"

#include "GameInstance.h"
#include "Impact.h"
#include "Player.h"

CSunflower::CSunflower(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CEnemy(m_pGraphic_Device)
{
}

CSunflower::CSunflower(const CSunflower& rhs)
	:CEnemy(rhs)
	,m_pTransform(rhs.m_pTransform)
	, m_pOnlyRotation(rhs.m_pOnlyRotation)
	,m_pShader(rhs.m_pShader)
	,m_pTexture(rhs.m_pTexture)
	,m_pBuffer(rhs.m_pBuffer)
	,m_pRenderer(rhs.m_pRenderer)
{
	Safe_AddRef(m_pBuffer);
	Safe_AddRef(m_pRenderer);
	Safe_AddRef(m_pShader);
	Safe_AddRef(m_pTexture);
	Safe_AddRef(m_pTransform);
	Safe_AddRef(m_pOnlyRotation);

}

HRESULT CSunflower::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CSunflower::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;


	if (FAILED(SetUp_Component()))
		return E_FAIL;

	return S_OK;
}

_int CSunflower::Tick(_float fTimeDelta)
{
	if (fTimeDelta <= 0 || !m_Valid)
		return 0;

	if (Get_Dead())
		return 0;

	if (0 > __super::Tick(fTimeDelta))
		return -1;


	Firing(fTimeDelta);

	return 0;
}

_int CSunflower::LateTick(_float fTimeDelta)
{
	if (Get_Dead() || !m_Valid)
		return 0;

	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return 0;
}

HRESULT CSunflower::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	m_pOnlyRotation->Bind_OnShader(m_pShader);
	m_pShader->SetUp_ValueOnShader("g_ColorStack", &g_ControlShader, sizeof(_float));
	m_pShader->SetUp_ValueOnShader("g_Color", m_Color, sizeof(_float4));

	m_pTexture->Bind_OnShader(m_pShader, "g_Texture", 0);
	m_pShader->Begin_Shader(SHADER_SKYBOX + 1);
	m_pBuffer->Render();
	m_pShader->End_Shader();
	m_pShader->SetUp_ValueOnShader("g_Color", _float4(0.f, 0.f, 0.f, 0.f),sizeof(_float4));

	return S_OK;
}

HRESULT CSunflower::SetUp_Component()
{
	CTransform::TRANSFORMDESC desc;
	desc.fRotationPerSec = 90.f;
	desc.fSpeedPerSec = 5.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransform, &desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, TEXT("OnlyRotation"), (CComponent**)&m_pOnlyRotation, &desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_SHADER_CUBE, COM_SHADER, (CComponent**)&m_pShader)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_CUBE, COM_BUFFER, (CComponent**)&m_pBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sunflower"), COM_TEXTURE, (CComponent**)&m_pTexture)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRenderer)))
		return E_FAIL;

	Set_Type(OBJ_ENEMY);

	m_pTransform->Scaled(m_NormalSize);

	m_EnemyType = ENEMY_NONE;
	m_Damage = 20;
	return S_OK;
}

void CSunflower::Add_HP(_int _add)
{
	
}

void CSunflower::Set_Valid(_bool _bool)
{
	m_Valid = _bool;
}

const _bool& CSunflower::Get_Valid() const
{
	return m_Valid;
}

void CSunflower::Fire()
{
	m_State = SF_FIRING;
}

void CSunflower::Set_Parent(CGameObject* _parent)
{
	m_Parent = _parent;
	m_ParentTransform = static_cast<CTransform*>(m_Parent->Get_Component(TEXT("OnlyRotation")));
}

void CSunflower::Set_Player(CPlayer* _player)
{
	m_pPlayer = _player;
	m_pPlayerTr = static_cast<CTransform*>(m_pPlayer->Get_Component(COM_TRANSFORM));
}

void CSunflower::Firing(_float fTimeDelta)
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	Synchronize_Parent(fTimeDelta);
	if (m_State == SF_CHARGING)
	{

		m_Color = m_NormalColor;
		p_instance->StopSound(CSoundMgr::ENEMY_EFFECT1);
		RELEASE_INSTANCE(CGameInstance);
		return;
	}


	m_fTimer += fTimeDelta;

	m_pOnlyRotation->Turn(m_pOnlyRotation->Get_State(CTransform::STATE_LOOK),m_fTimer * 10.f);

	_float3 vScale = m_pOnlyRotation->Get_Scale();

	if(!m_Charging)
	{
		p_instance->Play_Sound(TEXT("Sunflower_Charging.wav"), CSoundMgr::ENEMY_EFFECT1, 1.0f);
		m_Color = m_NormalColor;

		m_pOnlyRotation->Scaled(_float3(m_NormalSize.x + sinf(D3DXToRadian(m_fTimer * 30.f)), m_NormalSize.y + sinf(D3DXToRadian(m_fTimer * 30.f)), m_NormalSize.z));


		if(m_ChargingTime <= m_fTimer)
		{
			m_fTimer = 0.f;
			m_pOnlyRotation->Scaled(m_FiringSize);
			p_instance->StopSound(CSoundMgr::ENEMY_EFFECT1);
			m_Charging = true;
		}
	}
	else if(!m_Firing)
	{
		m_AttackTick += fTimeDelta;
		p_instance->Play_Sound(TEXT("Sunflower_Fire.wav"), CSoundMgr::ENEMY_EFFECT1, 1.0f);
		m_Color = m_ChargingColor;
		m_Color.y -= m_fTimer;

		m_pOnlyRotation->Scaled(_float3(vScale.x + m_fTimer, vScale.y + m_fTimer, vScale.z));


		_float3 vLook = m_pOnlyRotation->Get_State(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&vLook, &vLook);

		_float3 lineStart = m_pOnlyRotation->Get_State(CTransform::STATE_POSITION) - vLook * vScale.z * 0.5f;

		_float3 lineEnd = m_pOnlyRotation->Get_State(CTransform::STATE_POSITION) + vLook * vScale.z * 0.5f;

		_float3 line = lineEnd - lineStart;
		D3DXVec3Normalize(&line, &line);

		if (m_AttackTick >= 0.3f)
		{

			_float3 playerPos = m_pPlayerTr->Get_State(CTransform::STATE_POSITION);

			_float3 lineToPlayer = playerPos - lineStart;

			_float StartToPlayer = D3DXVec3Length(&lineToPlayer);
			D3DXVec3Normalize(&lineToPlayer, &lineToPlayer);

			_float length = abs(sinf(acos(D3DXVec3Dot(&line, &lineToPlayer))) * StartToPlayer);

			if (length <= (vScale.x + m_fTimer) * 0.5f)
			{
				m_pPlayer->Add_Hp(-m_Damage);
				m_AttackTick = 0.f;
			}
		}

		list<CCollision_Manager::COLLPOINT> collList = p_instance->Get_Ray_Collision_List(line, lineStart, 100.f, true);

		if (!collList.empty())
		{
			auto iter = collList.begin();

			if (iter->CollObj == m_Parent)
				++iter;

			if(iter != collList.end())
				Spawn_Impact(iter->Point);
		}


		if (vScale.x + m_fTimer * 0.5f >= m_FiringSize.x)
		{
			m_pOnlyRotation->Scaled(m_FiringSize);
		}

		if (m_FiringTime <= m_fTimer)
		{
			m_fTimer = 0.f;
			m_Firing = true;
			p_instance->StopSound(CSoundMgr::ENEMY_EFFECT1);
		}
	}
	else
	{
		m_Color = m_ChargingColor;
		m_Color += _float4(m_fTimer, m_fTimer,m_fTimer, m_fTimer);

		m_pOnlyRotation->Scaled(_float3(vScale.x - m_fTimer * 3.f, vScale.y - m_fTimer * 3.f, vScale.z));

		if(vScale.x-m_fTimer*5.f < m_NormalSize.x)
		{
			m_pOnlyRotation->Scaled(m_NormalSize);
		}

		if (m_EndingTime <= m_fTimer)
		{
			m_fTimer = 0.f;
			m_State = SF_CHARGING;
			m_pOnlyRotation->Scaled(m_NormalSize);
			m_Firing = false;
			m_Charging = false;
		}
	}
	RELEASE_INSTANCE(CGameInstance);
}

void CSunflower::Synchronize_Parent(_float fTimeDelta)
{
	_float3 vRight, vUp, vLook, vPos,vScale;

	vRight = m_ParentTransform->Get_State(CTransform::STATE_RIGHT);
	vUp = m_ParentTransform->Get_State(CTransform::STATE_UP);
	vLook = m_ParentTransform->Get_State(CTransform::STATE_LOOK);
	vPos = m_ParentTransform->Get_State(CTransform::STATE_POSITION);
	vScale = m_pTransform->Get_Scale();

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	_float3 position = vPos + vLook * (vScale.z * 0.5f)  + vLook * m_ParentTransform->Get_Scale().z * 0.5f;

	m_pTransform->Set_State(CTransform::STATE_RIGHT,vRight * vScale.x );
	m_pTransform->Set_State(CTransform::STATE_UP,vUp * vScale.y );
	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook * vScale.z );
	m_pTransform->Set_State(CTransform::STATE_POSITION, position);

	m_pOnlyRotation->Set_WorldMatrix(m_pTransform->Get_WorldMatrix());

}

void CSunflower::Spawn_Impact(_float3 pos)
{
	CImpact::IMPACT desc;
	ZeroMemory(&desc,sizeof(desc));

	desc.SpreadSpeed = 10.f;
	desc.RandomDirection = 10.f;
	desc.DeleteTime = 0.5f;
	desc.Color = m_Color;
	desc.Position = pos;
	desc.Size = _float3(0.1f,0.1f,0.1f);


	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	for(_uint i = 0; i < rand() % 5 + 5; ++i)
	{
		if(FAILED(p_instance->Add_GameObject(g_CurrLevel,TEXT("Impact_Sunflower"),TEXT("Prototype_GameObject_Impact"),&desc)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
	}
	RELEASE_INSTANCE(CGameInstance);

}

CSunflower* CSunflower::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CSunflower* p_sunflower = new CSunflower(m_pGraphic_Device);

	if(FAILED(p_sunflower->NativeConstruct_Prototype()))
	{
		Safe_Release(p_sunflower);
		return nullptr;
	}

	return p_sunflower;
}

CGameObject* CSunflower::Clone(void* pArg)
{
	CSunflower* p_sunflower = new CSunflower(*this);

	if (FAILED(p_sunflower->NativeConstruct(pArg)))
	{
		Safe_Release(p_sunflower);
		return nullptr;
	}

	return p_sunflower;
}

void CSunflower::Free()
{
	__super::Free();
	Safe_Release(m_pBuffer);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pTransform);
	Safe_Release(m_pOnlyRotation);
}
