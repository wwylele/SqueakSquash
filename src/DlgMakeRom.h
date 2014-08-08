#pragma once
#include "afxwin.h"
#include "Main.h"
#include "SqMapSet.h"
#include "WndWait.h"

class CDlgMakeRom : public CDialog
{
	DECLARE_DYNAMIC(CDlgMakeRom)

public:
	CDlgMakeRom(CWnd* pParent = NULL);  
	virtual ~CDlgMakeRom();

	enum { IDD = IDD_MAKEROM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()
public:
	SqMapSet *m_pMapSet;
	CEdit m_EditRom;
	CEdit m_EditNewRom;
	afx_msg void OnMrRom();
	afx_msg void OnMrNewrom();
	afx_msg void OnMakeRom();
	CEdit m_EditMapSet;
	virtual BOOL OnInitDialog();
	CEdit m_EditNdsEmu;
	CStatic m_StaticNdsEmu;
	afx_msg void OnBnClickedButtonNdsemu();
	CButton m_ButtonBeginEmu;
	afx_msg void OnBnClickedButtonBeginemu();
	CButton m_CheckV2;
};
