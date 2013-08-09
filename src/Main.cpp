
#include "stdafx.h"
#include "Main.h"
#include "MainFrm.h"

#pragma comment(lib,"Version.lib")


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
void GetProductVersion(CString *pstr,UINT *pu)
{  
	int nMaxPathName=4096; // Max length of file name/path  
	TCHAR *pBuffer;  
	UINT nItemLength;  
	void* pData,*lpBuffer;  
	CString sVersion;  
	DWORD dwInfoSize,dwHandle;  
	VS_FIXEDFILEINFO *pFileInfo;  
	// Get the file path and name  
	pBuffer=new TCHAR[nMaxPathName];  
	GetModuleFileName(NULL,pBuffer,nMaxPathName-1);  
  
	// Get File Version Info size  
	dwInfoSize=GetFileVersionInfoSize(pBuffer,&dwHandle);  
	if(dwInfoSize>0)  
	{  
		pData=new TCHAR[dwInfoSize];  
		if(GetFileVersionInfo(pBuffer,dwHandle,dwInfoSize,pData))  
			if(VerQueryValue(pData,_T("\\"),&lpBuffer,&nItemLength))  
			{  
				pFileInfo=(VS_FIXEDFILEINFO*)lpBuffer;  
				if(pstr)pstr->Format(_T("%d.%d.%d.%d"),   
					pFileInfo->dwProductVersionMS >> 16,   
					pFileInfo->dwProductVersionMS & 0xFFFF,   
					pFileInfo->dwProductVersionLS >> 16,  
					pFileInfo->dwProductVersionLS & 0xFFFF);      
				if(pu)*pu=(pFileInfo->dwProductVersionMS >> 16)*1000+
					(pFileInfo->dwProductVersionMS & 0xFFFF)*100+
					(pFileInfo->dwProductVersionLS >> 16)*10+
					(pFileInfo->dwProductVersionLS & 0xFFFF);
			}  
		// Delete the data buffer  
		delete[] pData;  
	}  
	// Get rid of the allocated string buffer  
	delete[] pBuffer;  
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

	CString strVer,strTitle;
	GetProductVersion(&strVer,0);
	strTitle.Format(_T("KIRBY Squeak Squash[Version:%s]    by wwylele"),(const TCHAR*)strVer);
	pFrame->Create(0,strTitle);


	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	return TRUE;
}




IMPLEMENT_DYNAMIC(CDlgAbout, CDialog)

CDlgAbout::CDlgAbout(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAbout::IDD, pParent)
{

}

CDlgAbout::~CDlgAbout()
{
}

void CDlgAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ABOUT, m_EditAbout);
}


BEGIN_MESSAGE_MAP(CDlgAbout, CDialog)
END_MESSAGE_MAP()




BOOL CDlgAbout::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strVer;
	GetProductVersion(&strVer,0);
	CString str;
	str.Format(_T("Squeak Squash\r\n")
		_T("Version:%s\r\n")
		_T("Time:")_T(__DATE__)_T(" ")_T(__TIME__)_T("\r\n")
		_T("Author:wwylele\r\n")

		_T("------======------")

		,strVer);
	m_EditAbout.SetWindowText(str);
	return TRUE;
}
