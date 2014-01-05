#pragma once
#include "afxwin.h"
bool CheckForUpdates();


class CDlgUpdate : public CDialog
{
	DECLARE_DYNAMIC(CDlgUpdate)

public:
	CDlgUpdate(CWnd* pParent = NULL); 
	virtual ~CDlgUpdate();


	enum { IDD = IDD_UPDATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUpdateInfo;
	CEdit m_EditUpdateInfo;
	afx_msg void OnBnClickedButtonDlTieba();
	afx_msg void OnBnClickedButtonDlGithub();
	virtual BOOL OnInitDialog();
};
