#pragma once

#include "Main.h"
#include "afxwin.h"

class CDlgGuideResize : public CDialog
{
	DECLARE_DYNAMIC(CDlgGuideResize)

public:
	CDlgGuideResize(CWnd* pParent = NULL);
	virtual ~CDlgGuideResize();

	u16 m_GuideCount;

	enum { IDD = IDD_GUIDE_RESIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CEdit m_Edit;
};
