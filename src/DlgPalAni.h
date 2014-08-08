#pragma once
#include "SqPl1.h"
#include "afxwin.h"


class CDlgPalAni : public CDialog
{
	DECLARE_DYNAMIC(CDlgPalAni)

public:
	CDlgPalAni(CWnd* pParent = NULL); 
	virtual ~CDlgPalAni();

	SqPl1 sqpl;

	u8 curline;
	u8 curframe;

	enum { IDD = IDD_PAL_ANI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	CListBox m_ListFrame;
	void UpdateListFrame();
	afx_msg void OnLbnSelchangeListPalFrame();
	CEdit m_EditTime;
	afx_msg void OnEnChangeEditPalTime();
	afx_msg void OnBnClickedButtonPalNew();
	afx_msg void OnBnClickedButtonPalDelete();
};
