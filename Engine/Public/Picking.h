#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking final : public CBase
{
	DECLARE_SINGLETON(CPicking)
private:
	CPicking();
	virtual ~CPicking() = default;

public:
	HRESULT NativeConstruct(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd);

public:
	enum TYPE {TYPE_MOUSE,TYPE_CROSSHAIR,TYPE_END};

public:	
	/* 월드스페이스 영역까지 구해놓는다. */
	HRESULT Transform_ToWorldSpace_Mouse();
	HRESULT Transform_ToWorldSpace_Crosshair();
	HRESULT Transform_ToLocalSpace(TYPE _type, _float4x4 WorldMatrixInverse);


	_bool isPick(TYPE _type, _float3* pLocalPoints, _float3* pOut = nullptr);

private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	HWND					m_hWnd;

private:
	// for Mouse
	_float3					m_vRayDir;
	_float3					m_vRayPos;

	_float3					m_vRayDirInLocal;
	_float3					m_vRayPosInLocal;

	// for Crosshair
	_float3					m_vRayDirCH;
	_float3					m_vRayPosCH;

	_float3					m_vRayDirInLocalCH;
	_float3					m_vRayPosInLocalCH;


public:
	virtual void Free() override;
};

END
