
//

#include "stdafx.h"
#include "WndWait.h"


// CWndWait

IMPLEMENT_DYNCREATE(CWndWait, CFrameWnd)

CWndWait CWndWait::WndWait;
HANDLE CWndWait::hEvent=0,CWndWait::hEventStop=0,CWndWait::hEventStopRet=0;

CWndWait::CWndWait()
{
}

CWndWait::~CWndWait()
{
	SetEvent(hEventStop);
	SetEvent(hEvent);
	WaitForSingleObject(hEventStopRet,INFINITE);
	CloseHandle(hEvent);
	CloseHandle(hEventStop);
	CloseHandle(hEventStopRet);
}
DWORD WINAPI CWndWait::WndWaitThread(LPVOID lpThreadParameter)
{
	int p=0;
	CDC* pDC=WndWait.GetDC();
	CBrush ba(RGB(10,10,10)),bb(RGB(220,220,220));

	while(1)
	{
		WaitForSingleObject(hEvent,INFINITE);
		if(WaitForSingleObject(hEventStop,1)!=WAIT_TIMEOUT)
		{	
			break;
		}
		pDC->TextOut(0,0,_T("WAIT..."),7);
		pDC->FillRect((LPRECT)&CRect(16,18,30,32),p==0?&ba:&bb);
		pDC->FillRect((LPRECT)&CRect(32,18,46,32),p==1?&ba:&bb);
		pDC->FillRect((LPRECT)&CRect(16,34,30,48),p==3?&ba:&bb);
		pDC->FillRect((LPRECT)&CRect(32,34,46,48),p==2?&ba:&bb);
		++p;if(p>3)p=0;
		Sleep(100);
	}

	WndWait.ReleaseDC(pDC);
	SetEvent(hEventStopRet);
	return 0;
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
	hEvent=CreateEvent(0,TRUE,FALSE,0);
	hEventStop=CreateEvent(0,TRUE,FALSE,0);
	hEventStopRet=CreateEvent(0,TRUE,FALSE,0);
	CreateThread(0,0,WndWaitThread,0,0,0);
}
void CWndWait::ShowWndWait()
{
	WndWait.ShowWindow(SW_SHOW);
	SetEvent(hEvent);
	
}
void CWndWait::HideWndWait()
{
	WndWait.ShowWindow(SW_HIDE);
	ResetEvent(hEvent);
}

BEGIN_MESSAGE_MAP(CWndWait, CFrameWnd)
END_MESSAGE_MAP()

