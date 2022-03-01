#pragma once
#include "Effect.h"
class CEffect_Alert :
    public CEffect
{
protected:
	explicit CEffect_Alert(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CEffect_Alert(const CEffect_Alert& rhs);
	virtual ~CEffect_Alert() = default;
public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;
protected:
	HRESULT SetUp_Components() override;
	HRESULT SetUp_RenderState() override;
	HRESULT Release_RenderState() override;

public:
	void Set_CamPos(CTransform* _tr);
	void Set_TargetPos(CTransform* _tr);

private:
	CTransform* m_pCameraTr = nullptr;
	CTransform* m_pTargetTr = nullptr;
	_float m_AdditionalPos = 2.f;

public:
	static CEffect_Alert* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device) ;
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

