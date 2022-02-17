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
	/* 월드스페이스 영역까지 구해놓는다. */
	HRESULT Transform_ToWorldSpace();
	HRESULT Transform_ToLocalSpace(_float4x4 WorldMatrixInverse);

	_bool isPick(_float3* pLocalPoints, _float3* pOut = nullptr);

private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	HWND					m_hWnd;

private:
	_float3					m_vRayDir;
	_float3					m_vRayPos;

	_float3					m_vRayDirInLocal;
	_float3					m_vRayPosInLocal;


	

public:
	virtual void Free() override;
};

END
