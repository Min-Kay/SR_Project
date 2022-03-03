#include "Camera_Manager.h"

IMPLEMENT_SINGLETON(CCamera_Manager);

CCamera_Manager::CCamera_Manager()
{
}

_int CCamera_Manager::Tick(_float fTimeDelta)
{
	for (auto& cam : m_CameraList)
	{
		if (0 > cam.second->Tick(fTimeDelta))
			return -1;
	}

	return _int();
}

_int CCamera_Manager::LateTick(_float fTimeDelta)
{
	for (auto& cam : m_CameraList)
	{
		if (0 > cam.second->LateTick(fTimeDelta))
			return -1;
	}
	return _int();
}

HRESULT CCamera_Manager::Add_Camera_Prototype(const _tchar* _tag, CCamera* cam)
{
	CCamera* pCam = Find_Camera_Prototype(_tag);

	if (nullptr != pCam)
		return E_FAIL;
	m_CameraPrototypeList.emplace(_tag, cam);
	return S_OK;
}

HRESULT CCamera_Manager::Add_Camera_Object(const _tchar* _PrototypeTag, const _tchar* _ObjectTag, void* pArg)
{
	CCamera* pCamProto = Find_Camera_Prototype(_PrototypeTag);
	if (nullptr == pCamProto)
		return E_FAIL;

	CCamera* pCamObject = Find_Camera_Object(_ObjectTag);
	if (nullptr != pCamObject)
		return E_FAIL;

	CGameObject* clone = pCamProto->Clone(pArg); 

	m_CameraList.emplace(_ObjectTag, static_cast<CCamera*>(clone));
	

	return S_OK;
}

HRESULT CCamera_Manager::Release_Camera(const _tchar* _tag)
{
	CCamera* pCam = Find_Camera_Object(_tag);
	if (nullptr == pCam)
		return E_FAIL;

	for (auto iter = m_CameraList.begin(); iter != m_CameraList.end();)
	{
		if (pCam == iter->second)
		{
			Safe_Release(iter->second);
			iter = m_CameraList.erase(iter);
			return S_OK;
		}
		++iter;
	}

	return E_FAIL;
}

HRESULT CCamera_Manager::Clear_Camera(_uint _level)
{

	for (map<const _tchar*, CCamera*>::iterator iter = m_CameraList.begin(); iter != m_CameraList.end();)
	{
		if (iter->second->Get_Level() == _level)
		{
			Safe_Release(iter->second);
			iter = m_CameraList.erase(iter);
		}
		else
			++iter;
	}

	return S_OK;
}

CCamera* CCamera_Manager::Find_Camera_Object(const _tchar* _ObjectTag)
{
	auto iter = find_if(m_CameraList.begin(), m_CameraList.end(), CTagFinder(_ObjectTag));

	if (iter == m_CameraList.end())
		return nullptr;

	return iter->second;
}

CCamera* CCamera_Manager::Find_Camera_Prototype(const _tchar* _tag)
{
	auto iter = find_if(m_CameraPrototypeList.begin(), m_CameraPrototypeList.end(), CTagFinder(_tag));

	if (iter == m_CameraPrototypeList.end())
		return nullptr;

	return iter->second;
}

map<const _tchar*, CCamera*>* CCamera_Manager::GetCameraList()
{
	return &m_CameraList;
}

void CCamera_Manager::Free()
{
	for (auto& cam : m_CameraList)
	{
		Safe_Release(cam.second);
	}

	for (auto& cam : m_CameraPrototypeList)
	{
		Safe_Release(cam.second);
	}

	m_CameraList.clear(); 
	m_CameraPrototypeList.clear();

}
