#include "..\Public\GameInstance.h"
IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pTimer_Manager(CTimer_Manager::GetInstance())
	, m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pCamera_Manager(CCamera_Manager::GetInstance())
	, m_pPicking(CPicking::GetInstance())
	, m_Sound_Manager(CSoundMgr::GetInstance())
	, m_Collision_Manager(CCollision_Manager::GetInstance())

{
	Safe_AddRef(m_pPicking);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pCamera_Manager);
	Safe_AddRef(m_Sound_Manager);
	Safe_AddRef(m_Collision_Manager);
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInstance, const CGraphic_Device::GRAPHICDESC & GraphicDesc, _uint iNumLevels, LPDIRECT3DDEVICE9* ppOut)
{
	if (nullptr == m_pGraphic_Device || 
		nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->InitDevice(GraphicDesc, ppOut)))
		return E_FAIL;

	if (nullptr != hInstance)
	{
		if (FAILED(m_pInput_Device->Ready_Input_Device(hInstance, GraphicDesc.hWnd)))
			return E_FAIL;
	}
	
	if (FAILED(m_pObject_Manager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pPicking->NativeConstruct(*ppOut, GraphicDesc.hWnd)))
		return E_FAIL;

	if (FAILED(m_Sound_Manager->NativeConstruct()))
		return E_FAIL;

	return S_OK;
}

_int CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager || 
		nullptr == m_pObject_Manager)
		return -1;

	if (FAILED(m_pInput_Device->SetUp_InputDeviceState()))
		return -1;

	if (0 > m_pObject_Manager->Tick(fTimeDelta))
		return -1;

	if (0 > m_pLevel_Manager->Tick(fTimeDelta))
		return -1;

	if (0 > m_pCamera_Manager->Tick(fTimeDelta))
		return -1;


	if (0 > m_pObject_Manager->LateTick(fTimeDelta))
		return -1;	

	if (0 > m_pLevel_Manager->LateTick(fTimeDelta))
		return -1;

	if (0 > m_pCamera_Manager->LateTick(fTimeDelta))
		return -1;


	if (FAILED(m_Collision_Manager->Collision(CCollision_Manager::COLLOBJTYPE_OBJ, CCollision_Manager::COLLOBJTYPE_OBJ)))
		return -1;

	if (FAILED(m_Collision_Manager->Collision(CCollision_Manager::COLLOBJTYPE_OBJ, CCollision_Manager::COLLOBJTYPE_STATIC)))
		return -1;

	m_pInput_Device->Tick_KeyState();

	return _int();
}

_int CGameInstance::Tick_Tool(_float fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	if (0 > m_pObject_Manager->Tick(fTimeDelta))
		return -1;

	if (0 > m_pCamera_Manager->Tick(fTimeDelta))
		return -1;


	if (0 > m_pObject_Manager->LateTick(fTimeDelta))
		return -1;

	if (0 > m_pCamera_Manager->LateTick(fTimeDelta))
		return -1;
	return _int();
}

HRESULT CGameInstance::Clear_LevelResource(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager || 
		nullptr == m_pComponent_Manager)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Clear_LevelObject(iLevelIndex)))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Clear_LevelObject(iLevelIndex)))
		return E_FAIL;

	if (FAILED(m_pCamera_Manager->Clear_Camera(iLevelIndex)))
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Render_Begin(void)
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->Render_Begin();
}

void CGameInstance::Render_End(HWND hWnd)
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->Render_End(hWnd);
}

_float CGameInstance::Get_TimeDelta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.f;

	return m_pTimer_Manager->Get_TimeDelta(pTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(pTimerTag);
}

HRESULT CGameInstance::OpenLevel(_uint iLevelIndex, CLevel * pNextLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->OpenLevel(iLevelIndex, pNextLevel);	
}

HRESULT CGameInstance::Render_Level()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Render();
}

CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iLevelIndex, pLayerTag, pComponentTag, iIndex);	
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject(iLevelIndex, pLayerTag, pPrototypeTag, pArg);
}

CGameObject* CGameInstance::Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iObjectIndex)
{
	return m_pObject_Manager->Get_GameObject(iLevelIndex,pLayerTag,iObjectIndex);
}

HRESULT CGameInstance::Release_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, CGameObject* pTarget)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;
	return m_pObject_Manager->Release_GameObject(iLevelIndex,pLayerTag,pTarget);
}

list<CGameObject*> CGameInstance::Get_Layer(_uint iLevelIndex, const _tchar* layerTag)
{
	return m_pObject_Manager->Get_Layer(iLevelIndex, layerTag);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

_bool CGameInstance::Get_Key_Down(_ubyte eKeyID)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Get_Key_Down(eKeyID);
}

_bool CGameInstance::Get_Key_Up(_ubyte eKeyID)
{
	if (nullptr == m_pInput_Device)
		return false;
	return m_pInput_Device->Get_Key_Up(eKeyID);

}

_bool CGameInstance::Get_Key_Press(_ubyte eKeyID)
{
	if (nullptr == m_pInput_Device)
		return false;
	return m_pInput_Device->Get_Key_Press(eKeyID);
}

_bool CGameInstance::Get_Mouse_Down(CInput_Device::MOUSEBUTTONSTATE eMouseID)
{
	if (nullptr == m_pInput_Device)
		return false;
	return  m_pInput_Device->Get_Mouse_Button_Down(eMouseID);
}

_bool CGameInstance::Get_Mouse_Up(CInput_Device::MOUSEBUTTONSTATE eMouseID)
{
	if (nullptr == m_pInput_Device)
		return false;
	return  m_pInput_Device->Get_Mouse_Button_Up(eMouseID);

}

_bool CGameInstance::Get_Mouse_Press(CInput_Device::MOUSEBUTTONSTATE eMouseID)
{
	if (nullptr == m_pInput_Device)
		return false;
	return  m_pInput_Device->Get_Mouse_Button_Press(eMouseID);

}

_long CGameInstance::Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseMoveState(eMouseMoveState);
}

HRESULT CGameInstance::Render_Camera(CRenderer* renderer)
{
	if (nullptr == m_pCamera_Manager)
		return E_FAIL;

	if (nullptr == renderer)
		return E_FAIL;

	map<const _tchar*, CCamera*> camList = *m_pCamera_Manager->GetCameraList();

	for (auto& cam : camList)
	{
		if (!cam.second->Get_Vaild())
			continue;

		if (FAILED(cam.second->BeforeRender()))
			return E_FAIL;

		Render_Begin();
		renderer->Render(cam.second->Get_Exception());
		Render_Level();
		Render_End(cam.second->Get_Handle());

		if (FAILED(cam.second->AfterRender()))
			return E_FAIL;
	}

	renderer->Clear_RenderObjects();

	return S_OK;
}

HRESULT CGameInstance::Add_Camera_Object(const _tchar* _Prototypetag, const _tchar* _ObjectTag, void* pArg)
{
	return m_pCamera_Manager->Add_Camera_Object(_Prototypetag,_ObjectTag,pArg);
}

HRESULT CGameInstance::Add_Camera_Prototype(const _tchar* _tag, CCamera* cam)
{
	return m_pCamera_Manager->Add_Camera_Prototype(_tag,cam);
}

HRESULT CGameInstance::Release_Camera(const _tchar* _tag)
{
	return m_pCamera_Manager->Release_Camera(_tag);
}

CCamera* CGameInstance::Find_Camera_Object(const _tchar* _ObjTag)
{
	return m_pCamera_Manager->Find_Camera_Object(_ObjTag);
}

int CGameInstance::VolumeUp(CSoundMgr::CHANNELID eID, _float _vol)
{
	if (nullptr == m_Sound_Manager)
		return 0;

	return m_Sound_Manager->VolumeUp(eID,_vol);
}

int CGameInstance::VolumeDown(CSoundMgr::CHANNELID eID, _float _vol)
{
	if (nullptr == m_Sound_Manager)
		return 0;

	return m_Sound_Manager->VolumeDown(eID, _vol);
}

int CGameInstance::BGMVolumeUp(_float _vol)
{
	if (nullptr == m_Sound_Manager)
		return 0;

	return m_Sound_Manager->BGMVolumeUp(_vol);
}

int CGameInstance::BGMVolumeDown(_float _vol)
{
	if (nullptr == m_Sound_Manager)
		return 0;

	return m_Sound_Manager->BGMVolumeDown(_vol);
}

int CGameInstance::Pause(CSoundMgr::CHANNELID eID)
{
	if (nullptr == m_Sound_Manager)
		return 0;

	return m_Sound_Manager->Pause(eID);
}

void CGameInstance::Play_Sound(TCHAR* pSoundKey, CSoundMgr::CHANNELID eID, _float _vol)
{
	if (nullptr == m_Sound_Manager)
		return;

	return m_Sound_Manager->Play_Sound(pSoundKey,eID,_vol);
}

void CGameInstance::PlayBGM(TCHAR* pSoundKey)
{
	if (nullptr == m_Sound_Manager)
		return;

	return m_Sound_Manager->PlayBGM(pSoundKey);
}

void CGameInstance::StopSound(CSoundMgr::CHANNELID eID)
{
	if (nullptr == m_Sound_Manager)
		return;

	return m_Sound_Manager->StopSound(eID);
}

void CGameInstance::StopAll()
{
	if (nullptr == m_Sound_Manager)
		return;

	return m_Sound_Manager->StopAll();
}

HRESULT CGameInstance::Add_Collider(CCollision_Manager::COLLOBJTYPE _type, CBoxCollider* collider)
{
	if (nullptr == m_Collision_Manager)
	{
		MSGBOX("Empty m_pCollision_Manager in CGameInstance");
		return E_FAIL;
	}

	return m_Collision_Manager->Add_Collider(_type, collider);
}

HRESULT CGameInstance::Release_Collider(CCollision_Manager::COLLOBJTYPE _type, CBoxCollider* collider)
{
	if (nullptr == m_Collision_Manager)
		return E_FAIL;

	return m_Collision_Manager->Release_Collider(_type,collider);

}

HRESULT CGameInstance::Release_ColliderList()
{
	if (nullptr == m_Collision_Manager)
		return E_FAIL;

	return m_Collision_Manager->Release_ColliderList();
}

list<CGameObject*> CGameInstance::Get_Collision_List(CBoxCollider* target)
{
	return m_Collision_Manager->Get_Collision_List(target);
}

list<CCollision_Manager::COLLPOINT> CGameInstance::Get_Ray_Collision_List(_float3 dir, _float3 pos, _float dis)
{
	return m_Collision_Manager->Get_Ray_Collision_List(dir, pos, dis);
}


void CGameInstance::SetMouseMode(_bool setting, HWND _hwnd)
{
	if (setting == true)
	{
		ShowCursor(true);
		ClipCursor(NULL);
	}
	else
	{
		RECT rc;
		GetWindowRect(_hwnd,&rc);
		rc.top = rc.top + GetSystemMetrics(SM_CYCAPTION) + 10;
		rc.left = rc.left + 10;
		rc.right = rc.right - 10;
		rc.bottom = rc.bottom - 10;
		ShowCursor(false);
		ClipCursor(&rc);
	}
	
}


void CGameInstance::Release_Engine()
{
	if (0 != CGameInstance::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CGameInstance ");

	if (0 != CCollision_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete Collision_Manager ");

	if (0 != CSoundMgr::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CSoundMgr ");

	if (0 != CTimer_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CTimer_Manager ");

	if (0 != CComponent_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CComponent_Manager ");

	if (0 != CObject_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CObject_Manager ");

	if (0 != CLevel_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CLevel_Manager ");

	if (0 != CPicking::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CPicking ");

	if (0 != CCamera_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete Camera_Manager ");

	if (0 != CInput_Device::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CInput_Device ");

	if (0 != CGraphic_Device::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CGraphic_Device ");

}

void CGameInstance::Free()
{
	Safe_Release(m_Sound_Manager);
	Safe_Release(m_pPicking);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_Collision_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pCamera_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pGraphic_Device);

}
