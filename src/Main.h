

#pragma once

#ifndef __AFXWIN_H__
	#error "stdafx.h"
#endif

#include "../VS2008/resource.h"      
#include "afxwin.h"

void OpenConsole();
void PrintLogDummy(const char*,...);
#ifdef _DEBUG
#define PrintLog printf
#else
#define PrintLog PrintLogDummy
#endif

#ifdef _DEBUG
#define RES_PATH _T("..\\..\\..\\res\\")
#else 
#define RES_PATH _T("res\\")
#endif

void GetProductVersion(CString *pstr,UINT *pu);

void OnScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);


extern const CString ProfilePath;

class CSqsqApp : public CWinApp
{
public:
	CSqsqApp();

	void SetFileAssociate();

public:
	virtual BOOL InitInstance();



public:

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

#ifdef _UNICODE
#define FORMAT_A2T L"%S"
#define FORMAT_W2T L"%s"
#else
#define FORMAT_A2T "%s"
#define FORMAT_W2T "%S"
#endif
#pragma once




class CDlgAbout : public CDialog
{
	DECLARE_DYNAMIC(CDlgAbout)

public:
	CDlgAbout(CWnd* pParent = NULL);
	virtual ~CDlgAbout();

	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	CImage m_BmpLogo;
#define ID_TIMER_LOGO 5555
	virtual BOOL OnInitDialog();
	CEdit m_EditAbout;
	afx_msg void OnBnClickedButtonCheckForUpdates();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
