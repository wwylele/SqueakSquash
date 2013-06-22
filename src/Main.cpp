
#include "stdafx.h"
#include "Main.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE hConsole=0;
void OpenConsole()
{

	if (hConsole) return;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD fileType = GetFileType(hConsole);

	bool shouldRedirectStdout = fileType == FILE_TYPE_UNKNOWN;

	bool attached = false;
	if(AttachConsole(-1))
		attached = true;
	if(!attached)
	{
		if (!AllocConsole()) return;

		SetConsoleCP(GetACP());
		SetConsoleOutputCP(GetACP());
	}
	SetConsoleTitle(_T("SqueakSquash Console"));

	if(shouldRedirectStdout)
	{
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		freopen("CONIN$", "r", stdin);
	}

}

BEGIN_MESSAGE_MAP(CSqsqApp, CWinApp)

END_MESSAGE_MAP()




CSqsqApp::CSqsqApp()
{

}




CSqsqApp theApp;




BOOL CSqsqApp::InitInstance()
{
	
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	if (!AfxOleInit())
	{
		AfxMessageBox(_T("AfxOleInit() Failed"));
		return FALSE;
	}
	AfxEnableControlContainer();
	
	OpenConsole();
	PrintLog("SqueakSquash:\n");

	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;

	pFrame->Create(0,_T("KIRBY......Squeak--->Squash!!!"));


	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	return TRUE;
}





