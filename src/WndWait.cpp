
//

#include "stdafx.h"
#include "WndWait.h"


// CWndWait

IMPLEMENT_DYNCREATE(CWndWait, CFrameWnd)

CWndWait CWndWait::WndWait;
bool CWndWait::wwshowed=false;

CWndWait::CWndWait()
{
}

CWndWait::~CWndWait()
{
}
DWORD WINAPI CWndWait::WndWaitThread(LPVOID lpThreadParameter)
{
	int p=0;
	CDC* pDC=WndWait.GetDC();
	COLORREF ca=RGB(10,10,10),cb=RGB(220,220,220);
	while(1)if(wwshowed)
	{
		pDC->TextOut(0,0,_T("WAIT..."),7);
		pDC->FillRect((LPRECT)&CRect(16,18,30,32),&CBrush(p==0?ca:cb));
		pDC->FillRect((LPRECT)&CRect(32,18,46,32),&CBrush(p==1?ca:cb));
		pDC->FillRect((LPRECT)&CRect(16,34,30,48),&CBrush(p==3?ca:cb));
		pDC->FillRect((LPRECT)&CRect(32,34,46,48),&CBrush(p==2?ca:cb));
		++p;if(p>3)p=0;
		Sleep(100);
	}
	WndWait.ReleaseDC(pDC);
}
#define WAITWND_W 64
#define WAITWND_H 64

void CWndWait::InitWndWait()
{
	WndWait.Create(0,_T("Wait"),WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_POPUP,
		(RECT)CRect(
		(GetSystemMetrics(SM_CXSCREEN)-WAITWND_W)/2,
		(GetSystemMetrics(SM_CYSCREEN)-WAITWND_H)/2,
		(GetSystemMetrics(SM_CXSCREEN)-WAITWND_W)/2+WAITWND_W,
		(GetSystemMetrics(SM_CYSCREEN)-WAITWND_H)/2+WAITWND_H));
	CreateThread(0,0,WndWaitThread,0,0,0);
}
void CWndWait::ShowWndWait()
{
	WndWait.ShowWindow(SW_SHOW);
	
	wwshowed=true;
	
}
void CWndWait::HideWndWait()
{
	WndWait.ShowWindow(SW_HIDE);
	wwshowed=false;
}

BEGIN_MESSAGE_MAP(CWndWait, CFrameWnd)
END_MESSAGE_MAP()

