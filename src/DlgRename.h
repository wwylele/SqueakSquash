#pragma once
#include "Main.h"



class CDlgRename : public CDialog
{
	DECLARE_DYNAMIC(CDlgRename)

public:
	CDlgRename(CWnd* pParent = NULL);   
	virtual ~CDlgRename();

	enum { IDD = IDD_RENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	CEdit m_EditRename;
	DECLARE_MESSAGE_MAP()
public:
	CString m_NameIO;
	
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
