#pragma once

#include "Main.h"
#include "SqMa.h"
#include "afxwin.h"

class CGuideTable : public CFrameWnd
{
public:
	SqMa *m_pMa;
#define IDC_EDIT_GUIDE 32109
	CEdit m_Edit;
#define IDC_BUTTON_GUIDE_RESIZE 32110
	CButton m_ButtomResize;
	enum{N,I,X,Y,U,M} m_cur;
	int m_curi,m_curj;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEditChange();
	afx_msg void OnGuideResize();
};
class CDlgGuide : public CDialog
{
	DECLARE_DYNAMIC(CDlgGuide)

public:
	CDlgGuide(CWnd* pParent = NULL);
	virtual ~CDlgGuide();

	enum { IDD = IDD_GUIDE };

	SqMa *m_pMa;
	void SetScroll();

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void OnWndScroll(UINT nSBCode, UINT nPos,int nBar);

	CGuideTable* m_pGuideTable;

	afx_msg void OnGuideResize();

};
