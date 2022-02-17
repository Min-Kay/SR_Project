#include "stdafx.h"
#include "..\Public\Runtime.h"
#include "GameInstance.h"
#include "ToolView.h"

CRuntime::CRuntime()
{
}


CRuntime::~CRuntime()
{
}

BOOL CRuntime::OnIdle(LONG lCount)
{

	CToolView* pToolView;
	pToolView = (CToolView*)AfxGetApp();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (this->m_pMainWnd->IsIconic())
	{
		return FALSE;
	}
	else
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		

		/* For.Timer_Default */
		if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_Default"))))
			return FALSE;

		/* For.Timer_60 */
		if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_60"))))
			return FALSE;

		_float			fTimeAcc = 0.f;


		fTimeAcc += pGameInstance->Get_TimeDelta(TEXT("Timer_Default"));

		if (fTimeAcc > (1.f / 60.0f))
		{
			fTimeAcc = 0.f;

			pToolView->Tick(pGameInstance->Get_TimeDelta(TEXT("Timer_60")));
			pToolView->OnDraw(nullptr);
		}

		RELEASE_INSTANCE(CGameInstance);
		return TRUE;
	}

		return CWinApp::OnIdle(lCount);
}
