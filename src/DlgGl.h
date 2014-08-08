#pragma once
#include "Nitro.h"
#include "Canvas.h"
#include "afxwin.h"

class CDlgGl : public CDialog
{
	DECLARE_DYNAMIC(CDlgGl)

public:
	CDlgGl(CWnd* pParent = NULL); 
	virtual ~CDlgGl();


	enum { IDD = IDD_GL };

	Nitro::Tile Tile[0x480];
	u8 PalLine[0x480];
	bool extend;
	Nitro::Color15 Pal[256];

	CCanvas Can;

	int cursel,curclr;
	bool drawing;

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	void PaintTile();
	afx_msg void OnBnClickedButtonGlPal();
	afx_msg void OnBnClickedCheckGlExtend();
	CButton m_CheckExtend;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
