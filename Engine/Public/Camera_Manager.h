#pragma once
#include "Base.h"
#include "Camera.h"

BEGIN(Engine)
class CCamera_Manager :
    public CBase
{
    DECLARE_SINGLETON(CCamera_Manager);

private:
    explicit CCamera_Manager();
    virtual ~CCamera_Manager() = default;

public:
    _int Tick(_float fTimeDelta);
    _int LateTick(_float fTimeDelta);

public:
    HRESULT Add_Camera_Prototype(const _tchar* _PrototypeTag, CCamera* cam);
    HRESULT Add_Camera_Object(const _tchar* _PrototypeTag, const _tchar* _ObjectTag, void* pArg = nullptr);

    HRESULT Release_Camera(const _tchar* _tag);
    CCamera* Find_Camera_Prototype(const _tchar* _PrototypeTag);
    CCamera* Find_Camera_Object(const _tchar* _ObjectTag);


public:
    map<const _tchar*, CCamera*>* GetCameraList(); 


private:
    map<const _tchar*, CCamera*> m_CameraPrototypeList; 
    map<const _tchar*, CCamera*> m_CameraList;


public:
    // CBase을(를) 통해 상속됨
    virtual void Free() override;
};
END
