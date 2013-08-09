#pragma once
#include "Main.h"
#include "afxwin.h"
#include "SqMa.h"
#include "SqB.h"
#include "Canvas.h"


class CDlgMapEdit : public CDialog
{
	DECLARE_DYNAMIC(CDlgMapEdit)

public:
	CDlgMapEdit(CWnd* pParent = NULL);
	virtual ~CDlgMapEdit();


	enum { IDD = IDD_MAP };

	SqMa m_Ma;
	SqB m_Fb,m_Bb;
	CCanvas m_CanMap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	CScrollBar m_HScroll;
	CScrollBar m_VScroll;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	
public:
	virtual BOOL OnInitDialog();
	void PaintMap();
	void SetScroll();
	CButton m_CheckPlane2;
	afx_msg void OnBnClickedCheckMapPlane2();
	CButton m_CheckPlane0;
	afx_msg void OnBnClickedCheckMapPlane0();
	CButton m_CheckDet;
	afx_msg void OnBnClickedCheckMapDet();
};
