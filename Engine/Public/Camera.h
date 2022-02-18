#pragma once

#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		_float3		vEye, vAt, vAxisY;
		_float		fFovy, fAspect, fNear, fFar;
		CTransform::TRANSFORMDESC TransformDesc;
		_uint		iLevel;
	}CAMERADESC;
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual HRESULT BeforeRender();
	virtual HRESULT AfterRender();
	HRESULT Use_Pick(_uint _type);
public:
	void Set_Level(_uint iLevel = 0);
	void Set_Handle(HWND _hWnd);
	void Set_Vaild(_bool _bool);
	void Set_State(const CCamera::CAMERADESC& desc);

public:
	HRESULT Add_Exception(_uint iIndex);
	void Release_Exception(_uint iIndex);

public:
	vector<_uint>& Get_Exception();
	const _uint& Get_Level() const;
	const HWND& Get_Handle() const;
	CTransform* Get_CameraTransform();
	const _bool Get_Vaild() const;

protected:
	class CTransform*			m_pTransform = nullptr;
	HWND						hWnd = nullptr;
	_uint						m_Level = 0;
	_bool						isVaild = true;
	vector<_uint>				exceptions;

	CAMERADESC					m_CameraDesc;
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END