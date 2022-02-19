#include "Font_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CFont_Manager);

CFont_Manager::CFont_Manager()
{
}

HRESULT CFont_Manager::NativeConstruct(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	/* For.Prototype_Component_Texture_Font */
	if (FAILED(pInstance->Add_Prototype(0, TEXT("Prototype_Component_Texture_Font"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Font/%d.jpg"),38))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TextBox*/
	if (FAILED(pInstance->Add_Prototype(0, TEXT("Prototype_Component_Texture_Font"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Font/TextBox.png")))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

_int CFont_Manager::Tick(_float fTimeDelta)
{
	return _int();
}

_int CFont_Manager::LateTick(_float fTimeDelta)
{
	return _int();
}

HRESULT CFont_Manager::Render()
{
	return S_OK;
}

HRESULT CFont_Manager::Print_Text(const _tchar* _text, _float _printTime)
{
	return S_OK;
}

void CFont_Manager::Free()
{
}
