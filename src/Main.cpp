﻿//UTF8♔


#include "stdafx.h"
#include "Main.h"
#include "CheckForUpdates.h"
#include "MainFrm.h"

#pragma comment(lib,"Version.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CString ProfilePath=_T(".\\SqueakSquash.ini");

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
void PrintLogDummy(const char*,...)
{
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
				if(pu)*pu=((pFileInfo->dwProductVersionMS >> 16)<<24)|
					((pFileInfo->dwProductVersionMS & 0xFFFF)<<16)|
					((pFileInfo->dwProductVersionLS >> 16)<<8)|
					(pFileInfo->dwProductVersionLS & 0xFFFF);
			}  
		// Delete the data buffer  
		delete[] pData;  
	}  
	// Get rid of the allocated string buffer  
	delete[] pBuffer;  
}  

void OnScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
   int minpos;
   int maxpos;
   pScrollBar->GetScrollRange(&minpos, &maxpos); 
   maxpos = pScrollBar->GetScrollLimit();
   int curpos = pScrollBar->GetScrollPos();
   switch (nSBCode)
   {
   case SB_LEFT:curpos = minpos;break;
   case SB_RIGHT:curpos = maxpos;break;
   case SB_LINELEFT:
      if (curpos > minpos)
         curpos--;
      break;
   case SB_LINERIGHT:
      if (curpos < maxpos)
         curpos++;
      break;
   case SB_PAGELEFT:
   {
      SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);
      if (curpos > minpos)
      curpos = max(minpos, curpos - (int) info.nPage);
   }
      break;
   case SB_PAGERIGHT:
   {
      SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);
      if (curpos < maxpos)
         curpos = min(maxpos, curpos + (int) info.nPage);
   }
      break;
   case SB_THUMBPOSITION: 
      curpos = nPos;     
      break;
   case SB_THUMBTRACK:   
      curpos = nPos;     
      break;
   }
   pScrollBar->SetScrollPos(curpos);
}
BEGIN_MESSAGE_MAP(CSqsqApp, CWinApp)

END_MESSAGE_MAP()




CSqsqApp::CSqsqApp()
{

}

bool regCreateFileType(const char *ext, const char *type)
{
  DWORD disp = 0;
  HKEY key;
  LONG res = RegCreateKeyExA(HKEY_CLASSES_ROOT,
                            ext,
                            0,
                            "",
                            REG_OPTION_NON_VOLATILE,
                            KEY_ALL_ACCESS,
                            NULL,
                            &key,
                            &disp);
  if(res == ERROR_SUCCESS) {
    res = RegSetValueExA(key,
                        "",
                        0,
                        REG_SZ,
                        (const UCHAR *)type,
                        strlen(type)+1);
    RegCloseKey(key);
    return true;
  }
  return false;
}

bool regAssociateType(const char *type, const char *desc, const char *application)
{
  DWORD disp = 0;
  HKEY key;
  LONG res = RegCreateKeyExA(HKEY_CLASSES_ROOT,
                            type,
                            0,
                            "",
                            REG_OPTION_NON_VOLATILE,
                            KEY_ALL_ACCESS,
                            NULL,
                            &key,
                            &disp);
  if(res == ERROR_SUCCESS) {
    res = RegSetValueExA(key,
                        "",
                        0,
                        REG_SZ,
                        (const UCHAR *)desc,
                        strlen(desc)+1);
    HKEY key2;
    res = RegCreateKeyExA(key,
                         "Shell\\Open\\Command",
                         0,
                         "",
                         REG_OPTION_NON_VOLATILE,
                         KEY_ALL_ACCESS,
                         NULL,
                         &key2,
                         &disp);
    if(res == ERROR_SUCCESS) {
      res = RegSetValueExA(key2,
                          "",
                          0,
                          REG_SZ,
                          (const UCHAR *)application,
                          strlen(application)+1);
      RegCloseKey(key2);
      RegCloseKey(key);
      return true;
    }
    
    RegCloseKey(key);
  }
  return false;
}
void CSqsqApp::SetFileAssociate()
{

	TCHAR applicationPath[4096];  
	GetModuleFileName(NULL,applicationPath,4095);
	CStringA commandPath;
#ifdef _UNICODE
	commandPath.Format("\"%S\" \"%%1\"", (TCHAR*)applicationPath);
#else
	commandPath.Format("\"%s\" \"%%1\"", (TCHAR*)applicationPath);
#endif
	regAssociateType("SqueakSquash.MapSet",
                     "SqueakSquash MapSet File",
                     commandPath);
	regCreateFileType(".sqms","SqueakSquash.MapSet");
  
}

CSqsqApp theApp;


DWORD WINAPI CheckForUpdates_Thread(LPVOID lpThreadParameter)
{
	CheckForUpdates();
	return 0;
}

BOOL CSqsqApp::InitInstance()
{
	srand((unsigned int)time(0));
	
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	SetFileAssociate();

	CWinApp::InitInstance();


	if (!AfxOleInit())
	{
		AfxMessageBox(_T("AfxOleInit() Failed"));
		return FALSE;
	}
	AfxEnableControlContainer();
	
#ifdef _DEBUG
	OpenConsole();
#endif
	PrintLog("SqueakSquash:\n");

	::CreateThread(0,0,CheckForUpdates_Thread,0,0,0);

	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;

	CString strVer,strTitle;
	GetProductVersion(&strVer,0);
	strTitle.Format(_T("KIRBY Squeak Squash[Version:%s]    by wwylele"),(const TCHAR*)strVer);
#ifdef _DEBUG
	strTitle=_T("<DEBUG>")+strTitle;
#endif
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
	ON_BN_CLICKED(IDC_BUTTON_CHECK_FOR_UPDATES, &CDlgAbout::OnBnClickedButtonCheckForUpdates)
	ON_WM_TIMER()
END_MESSAGE_MAP()




BOOL CDlgAbout::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_BmpLogo.LoadFromResource(AfxGetApp()->m_hInstance,IDB_LOGO);

	CString strVer;
	GetProductVersion(&strVer,0);
	CString str;
	str.Format(_T("Squeak Squash\r\n")
#ifdef _DEBUG
		_T("<DEBUG>\r\n")
#endif
		_T("Version:%s\r\n")
		_T("Time:")_T(__TIME__)_T(" ")_T(__DATE__)_T("\r\n")
		_T("Author:wwylele\r\n")
		_T("------======------\r\n")
		_T("氧气已用完，一秒后超度一生")

		,strVer);
	m_EditAbout.SetWindowText(str);
	m_EditAbout.SetSel(0,0,0);
	SetTimer(ID_TIMER_LOGO,100,0);
	return FALSE;
}

void CDlgAbout::OnBnClickedButtonCheckForUpdates()
{
	if(!CheckForUpdates())MessageBox(_T("未发现软件更新"),_T("更新"));
}

void CDlgAbout::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==ID_TIMER_LOGO)
	{
		CDC *pDC=GetDC();
		int x,y,r,g,b;
		COLORREF clr;
		for(int i=0;i<100;++i)
		{
			x=rand()%(m_BmpLogo.GetWidth()+32);
			x-=32;
			y=rand()%m_BmpLogo.GetHeight();
			r=rand()%255;
			g=rand()%255;
			b=rand()%255;
			if(x>-1)
			{
				if(m_BmpLogo.GetPixel(x,y))
					clr=RGB((r+255)/2,(g+255)/2,(b+255)/2);
				else
					clr=RGB((r+0)/2,(g+0)/2,(b+0)/2);
				pDC->SetPixel(32+x,y,clr);
			}
			else if(y>31)
			{
				clr=RGB((r+255)/2,(g+255)/2,(b+255)/2);
				pDC->SetPixel(32+x,y,clr);
			}
			
		}


		ReleaseDC(pDC);
	}

	CDialog::OnTimer(nIDEvent);
}

int CSqsqApp::ExitInstance()
{
	//...

	return CWinApp::ExitInstance();
}
