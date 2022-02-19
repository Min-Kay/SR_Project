#pragma once
#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CTransform; 

class CFont_Manager :
    public CBase
{
    DECLARE_SINGLETON(CFont_Manager);
private:
   explicit CFont_Manager();
   virtual ~CFont_Manager() = default;

public:
    HRESULT NativeConstruct(LPDIRECT3DDEVICE9 m_pGraphic_Device);
    _int Tick(_float fTimeDelta);
    _int LateTick(_float fTimeDelta);
    HRESULT Render();

public:
    HRESULT Print_Text(const _tchar* _text, _float _printTime);

private:
    HRESULT Set_Font(_tchar);

private:
    list<_tchar*> textList;

    CRenderer* m_pRenderer = nullptr;
    CTransform* m_pTextBoxTransform = nullptr;
    

   
public:
    virtual void Free() override;

   
};
END

