#pragma once

#include "Main.h"
#include "afxwin.h"

class CDlgResize : public CDialog
{
	DECLARE_DYNAMIC(CDlgResize)

public:
	CDlgResize(CWnd* pParent = NULL);
	virtual ~CDlgResize();


	enum { IDD = IDD_RESIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
	u8 m_Wo,m_Ho;
public:
	CEdit m_EditW;
	CEdit m_EditH;
	u8 m_W,m_H;
	int m_Ox,m_Oy;
protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};
