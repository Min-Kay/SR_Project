#include "stdafx.h"
#include "..\Public\Player.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "PortalControl.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Scaled(_float3(2.f, 2.f, 2.f));

	return S_OK;
}

_int CPlayer::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_fFrame += 12.0f * fTimeDelta;

	if (m_fFrame >= 12.0f)
		m_fFrame = 0.f;
	
	if (pGameInstance->Get_Key_Press(DIK_W))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (pGameInstance->Get_Key_Press(DIK_S))
	{
		m_pTransformCom->Go_BackWard(fTimeDelta);
	}

	if (pGameInstance->Get_Key_Press(DIK_A))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (pGameInstance->Get_Key_Press(DIK_D))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	if (nullptr == m_pPortalCtrl)
	{
		if (pGameInstance->Get_Key_Down(DIK_I))
		{
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("PortalCtrl"), TEXT("Prototype_GameObject_PortalCtrl"))))
				return E_FAIL;

			m_pPortalCtrl = static_cast<CPortalControl*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("PortalCtrl"), 0));
			m_pPortalCtrl->Set_Player(m_pTransformCom);
			Safe_AddRef(m_pPortalCtrl);
		}
	}

	if (nullptr != m_pPortalCtrl)
	{
		if (pGameInstance->Get_Mouse_Up(CInput_Device::MBS_LBUTTON))
		{
			m_pPortalCtrl->Spawn_Portal(LEVEL_GAMEPLAY, m_Camera->Get_CameraTransform(), CPortalControl::PORTAL_ORANGE);
		}

		if (pGameInstance->Get_Mouse_Down(CInput_Device::MBS_RBUTTON))
		{
			m_pPortalCtrl->Spawn_Portal(LEVEL_GAMEPLAY, m_Camera->Get_CameraTransform(), CPortalControl::PORTAL_BLUE);
		}

		if (pGameInstance->Get_Key_Up(DIK_SPACE))
		{
			m_pPortalCtrl->Erase_Portal(LEVEL_GAMEPLAY);
		}
	}


	if (m_Camera)
	{
		m_Camera->Get_CameraTransform()->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	}

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CPlayer::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	/*if (FAILED(SetUp_OnTerrain()))
		return -1;*/

	if(m_Camera)
	{
		_float3 vRight, vUp , vLook;

		vRight = m_Camera->Get_CameraTransform()->Get_State(CTransform::STATE_RIGHT);
		D3DXVec3Normalize(&vRight, &vRight);

		vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
		vLook = *D3DXVec3Cross(&vLook, &vRight, &vUp);
		D3DXVec3Normalize(&vLook,&vLook);
		_float3 vScale = m_pTransformCom->Get_Scale();

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT,vRight * vScale.x);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CPlayer::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(_uint(m_fFrame))))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Set_Cam(CCamera* cam)
{
	m_Camera = cam;
	static_cast<CCamera_Player*>(m_Camera)->Set_Player(this);
	return S_OK;
}

HRESULT CPlayer::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RECT, COM_BUFFER, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer::SetUp_OnTerrain()
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	CVIBuffer_Terrain*	pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), COM_BUFFER);
	if (nullptr == pVIBuffer_Terrain)
		return E_FAIL;

	_float3		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = pVIBuffer_Terrain->Compute_Y(vPosition);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK; 
}

HRESULT CPlayer::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CPlayer::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*	pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone(void* pArg )
{
	/* 새로운객체를 복제하여 생성한다. */
	CPlayer*	pInstance = new CPlayer(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pPortalCtrl);
	Safe_Release(m_pTextureCom); 
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
