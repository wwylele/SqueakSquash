
//
#pragma once

#ifndef __AFXWIN_H__
	#error "stdafx.h"
#endif

#include "../VS2008/resource.h"      
#include "afxwin.h"

void OpenConsole();
#define PrintLog printf
void GetProductVersion(CString *pstr,UINT *pu);


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
	virtual BOOL OnInitDialog();
	CEdit m_EditAbout;
};
