#pragma once

#include "Main.h"
#include "SqMapSet.h"
#include "SqMa.h"
#include "SqB.h"
#include "Canvas.h"
#include "afxwin.h"

#define ID_TIMER_ANI_DISP 35001
#define ID_MENU_TMCOPY_BASE 40000
class CDlgTexMapping : public CDialog
{
	DECLARE_DYNAMIC(CDlgTexMapping)

public:
	CDlgTexMapping(CWnd* pParent = NULL);   
	virtual ~CDlgTexMapping();

	SqMa m_Ma;
	SqB m_Fb,m_Bb;

	u8 m_FbIndex,m_BbIndex;
	u32 m_StageIdx;
	u16 m_StepIdx;
	SqMapSet *m_pMapSet;

	CCanvas m_Can,m_CanTile;


	enum { IDD = IDD_TEXMAPPING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CScrollBar m_HScroll;
	CScrollBar m_VScroll;
	void SetScroll();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioTexmappingA();
	afx_msg void OnBnClickedRadioTexmappingB();
	bool m_TmF;
	bool m_AniDisp;
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	void PaintTexMapping();
	void PaintTile();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonTexmappingCopy();
	afx_msg void OnMenuTmcopy(UINT nID);

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void OnTexMappingMouseMove(u16 tmindex,u8 stm);
	void OnTexMappingLButtonDown(u16 tmindex,u8 stm);
	u16 cur_tmindex;
	u8 cur_stm;
	u16 cursel_tmindex;
	u8 cursel_stm;
	CStatic m_StaticTmDetail;
	CButton m_CheckClr0;
	afx_msg void OnBnClickedCheckTexmappingClr0();
	CEdit m_EditTileAni0;
	CEdit m_EditTileAni1;
	CEdit m_EditTileAni2;
	CEdit m_EditTileAni3;
	afx_msg void OnEnChangeEditTileAni0();
	afx_msg void OnEnChangeEditTileAni1();
	afx_msg void OnEnChangeEditTileAni2();
	afx_msg void OnEnChangeEditTileAni3();
	afx_msg void OnBnClickedButtonTexmappingFx();
	afx_msg void OnBnClickedButtonTexmappingFy();
	CListBox m_ListAni;
	void UpdateAniList();
	afx_msg void OnBnClickedButtonTexmappingAniRemove();
	afx_msg void OnBnClickedButtonTexmappingAniProperty();
	afx_msg void OnLbnDblclkListTexmappingAni();
	afx_msg void OnBnClickedButtonTexmappingAniNew();
	afx_msg void OnBnClickedButtonTexmappingResize();
	afx_msg void OnBnClickedButtonTexmappingCancel();
	afx_msg void OnBnClickedButtonTexmappingPlane0Ref();
};
