//UTF8♔


#include "stdafx.h"
#include "Main.h"
#include "CheckForUpdates.h"
#include <wininet.h>
#pragma comment(lib,"wininet.lib")

static bool GetInternetURLText(LPSTR lpcInternetURL, char* buff,ULONG MaxLen)
{   
    HINTERNET hSession;   
    hSession = InternetOpen(_T("WinInet"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);   
	__try
    {       
       if(hSession != NULL)        
       {          
           HINTERNET hRequest;            
           hRequest = InternetOpenUrlA(hSession,lpcInternetURL, NULL,0, INTERNET_FLAG_RELOAD, 0);        
           __try         
           {                
              if(hRequest != NULL)        
              {           
                  DWORD dwBytesRead;                  
                  char *szBuffer = new char[MaxLen];
				  ZeroMemory(szBuffer,MaxLen);
 
                  if(InternetReadFile(hRequest, szBuffer, MaxLen, &dwBytesRead))           
                  {                 
                     MoveMemory(buff, szBuffer, MaxLen);   
                     delete[] szBuffer;
                     return true;              
                  }
				  delete[] szBuffer;
              }           
           }__finally     
           {              
              InternetCloseHandle(hRequest);  
           }       
       }   
    }__finally    
    {       
       InternetCloseHandle(hSession);  
    }   
    return false;
}
bool CheckForUpdates()
{
	char buf[1024];
	if(!GetInternetURLText(
		"https://raw.github.com/wwylele/SqueakSquash/master/VS/ver",
		buf,1024))return false;
	UINT VerI,VerT;
	int a,b,c,d;
	sscanf(buf,"%d,%d,%d,%d",&a,&b,&c,&d);
	VerI=(a<<24)|(b<<16)|(c<<8)|d;
	CString strVerT;
	GetProductVersion(&strVerT,&VerT);
	if(VerT<VerI)
	{
		CDlgUpdate dlg;
		dlg.m_strUpdateInfo.Format(_T("当前版本:SqueakSquash %s\r\n可更新至:SqueakSquash %d.%d.%d.%d"),
			(const TCHAR*)strVerT,a,b,c,d);
		dlg.DoModal();
		return true;
	}
	return false;
	
}


IMPLEMENT_DYNAMIC(CDlgUpdate, CDialog)

CDlgUpdate::CDlgUpdate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUpdate::IDD, pParent)
{

}

CDlgUpdate::~CDlgUpdate()
{
}

void CDlgUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditUpdateInfo);
}


BEGIN_MESSAGE_MAP(CDlgUpdate, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DL_TIEBA, &CDlgUpdate::OnBnClickedButtonDlTieba)
	ON_BN_CLICKED(IDC_BUTTON_DL_GITHUB, &CDlgUpdate::OnBnClickedButtonDlGithub)
END_MESSAGE_MAP()



void CDlgUpdate::OnBnClickedButtonDlTieba()
{
	ShellExecute(0,_T("open"),
		_T("http://tieba.baidu.com/p/2519591614"),
		0,0,SW_SHOWNORMAL);
}

void CDlgUpdate::OnBnClickedButtonDlGithub()
{
	ShellExecute(0,_T("open"),
		_T("https://github.com/wwylele/SqueakSquash/releases"),
		0,0,SW_SHOWNORMAL);
}

BOOL CDlgUpdate::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_EditUpdateInfo.SetWindowText(m_strUpdateInfo);

	return TRUE;
}
