#pragma once


#include "Main.h"
#include "afxwin.h"

class CDlgTmResize : public CDialog
{
	DECLARE_DYNAMIC(CDlgTmResize)

public:
	CDlgTmResize(CWnd* pParent = NULL); 
	virtual ~CDlgTmResize();

	u16 m_TexMappingCount;


	enum { IDD = IDD_TMRESIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditTexMappingCount;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
