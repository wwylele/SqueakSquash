#pragma once
#include "Main.h"
#include "afxwin.h"
#include "SqMa.h"
#include "SqB.h"
#include "Canvas.h"

#define FORMAT_BGM _T("%d:%s")
#define FORMAT_BOSS _T("%d")

#define ID_TIMER_REDRAW_MAP 35000
class CDlgMapEdit : public CDialog
{
	DECLARE_DYNAMIC(CDlgMapEdit)

public:
	CDlgMapEdit(CWnd* pParent = NULL);
	virtual ~CDlgMapEdit();


	enum { IDD = IDD_MAP };

	SqMa m_Ma;
	SqB m_Fb,m_Bb;
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

protected:
	CCanvas m_CanMap;
	CCanvas m_CanStock;
	CCanvas m_CanCurGrid;

	u8 cur_grid_x,cur_grid_y;
	u8 cur_stock_x,cur_stock_y;
	u32 cursel_stock;
	u8 cursel_stock_x,cursel_stock_y;

	bool grid_drawing;
public:
	CScrollBar m_HScroll;
	CScrollBar m_VScroll;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	virtual BOOL OnInitDialog();
	void PaintMap(u8 onlyx=0xFF,u8 onlyy=0xFF);
	void PaintStock();
	void SetScroll();
	void SetStockScroll();
	CButton m_CheckPlane2;
	afx_msg void OnBnClickedCheckMapPlane2();
	CButton m_CheckPlane0;
	afx_msg void OnBnClickedCheckMapPlane0();
	CButton m_CheckDet;
	afx_msg void OnBnClickedCheckMapDet();
	CScrollBar m_VScrollStock;
	CScrollBar m_HScrollStock;
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	
	void OnMapLButtonDown(u8 x,u8 y);
	void OnMapLButtonUp(u8 x,u8 y);
	void OnMapDraw(u8 x,u8 y);
	void OnMapMouseMove(u8 x,u8 y);
	void OnStockMouseMove(u8 x,u8 y);
	void OnStockLButtonDown(u8 x,u8 y);

	CStatic m_StaticCurCoord;
	CEdit m_EditCurGrid0;
	CEdit m_EditCurGrid1;
	CEdit m_EditCurGrid2;

	struct OPT_STACK_NOTE
	{
		u8 x,y;
		u8 plane;
		bool det;
		u32 oldcode;
	};
	CList<OPT_STACK_NOTE> m_StackUndo,m_StackRedo;
	CComboBox m_ComboBgm;
	afx_msg void OnCbnSelchangeComboBgm();
	CComboBox m_ComboBoss;
	afx_msg void OnCbnSelchangeComboBoss();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckMapAni();
	CButton m_CheckMapAni;
};
