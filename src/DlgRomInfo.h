#pragma once

#include "Main.h"
#include "SqMapSet.h"
#include "afxwin.h"


class CDlgRomInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgRomInfo)

public:
	CDlgRomInfo(CWnd* pParent = NULL); 
	virtual ~CDlgRomInfo();


	enum { IDD = IDD_ROMINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()
public:
	SqMapSet::SqmsRomInfo m_RomInfo;
	CEdit m_EditTitle;
	u8 curcolor;
	bool drawing;
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();
	void PaintIcon(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
