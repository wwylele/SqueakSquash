#pragma once
#include "Main.h"
#include "SqMa.h"
#include "SqDe.h"
#include "Evc.h"
#include "SqB.h"
#include "Canvas.h"
#include "afxwin.h"

#define FORMAT_BGM _T("%d:%s")
#define FORMAT_BOSS _T("%d")

#define MAX_GUIDE_COUNT 200
#define GUIDE_COLOR_COUNT MAX_GUIDE_COUNT
class GuideColorTable
{
public:
	struct floatrgb
	{
		float r,g,b;
	}Color[GUIDE_COLOR_COUNT];
	void LoadDefault();
	void Load();
	void Save();
};

#define ID_TIMER_REDRAW_MAP 35000
#define ID_NEWOBJ_FOE 35001
#define ID_NEWOBJ_SUP 35002
#define ID_NEWOBJ_DOOR 35003
#define ID_NEWOBJ_MCTRL 35004
class CDlgMapEdit : public CDialog
{
	DECLARE_DYNAMIC(CDlgMapEdit)

public:
	CDlgMapEdit(CWnd* pParent = NULL);
	virtual ~CDlgMapEdit();


	enum { IDD = IDD_MAP };

	SqMa m_Ma;
	SqDe m_De;
	SqB m_Fb,m_Bb;
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

protected:
	//SqMctrlPack m_Mctrl;
	CCanvas m_CanMap;
	CCanvas m_CanStock;
	CCanvas m_CanCurGrid;

	u8 cur_grid_x,cur_grid_y;
	u8 cur_stock_x,cur_stock_y;
	u32 cursel_stock;
	u8 cursel_stock_x,cursel_stock_y;
	u8 cursel_stock_x2,cursel_stock_y2;
	u32 cursel_stock2;

	enum MODE{
		MPT_GRID,
		MPT_OBJ,
		MPT_BOSS
	}mode;

	bool grid_drawing;
public:
	CScrollBar m_HScroll;
	CScrollBar m_VScroll;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	virtual BOOL OnInitDialog();
	inline COLORREF ColorToLi(COLORREF color,u8 bosszone);

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
	void OnMapLButtonDown_Obj(u16 x,u16 y);
	void OnMapLButtonUp_Obj();
	void OnMapLButtonUp(u8 x,u8 y);
	void OnMapDraw(u8 x,u8 y);
	void OnMapMouseMove(u8 x,u8 y);
	void OnMapMouseMove_Obj(u16 x,u16 y);
	void OnStockMouseMove(u8 x,u8 y);
	void OnStockLButtonDown(u8 x,u8 y);
	void OnStockRButtonDown(u8 x,u8 y);
	afx_msg void OnMapDbClick();
	

	CStatic m_StaticCurCoord;
	CEdit m_EditCurGrid0;
	CEdit m_EditCurGrid1;
	CEdit m_EditCurGrid2;
	CEdit m_EditCurGridBoss;

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
	afx_msg void OnBnClickedRadioMapPtGrid();
	afx_msg void OnBnClickedRadioMapPtObj();
	CListCtrl m_ListObj;
	void UpdateObjList();
	//void UpdateGuideList();
	afx_msg void OnBnClickedRadioMapPtBoss();
	
	CButton m_CheckHideFoe;
	CButton m_CheckHideSup;
	CButton m_CheckHideDoor;
	CButton m_CheckHideMctrl;
	afx_msg void OnBnClickedCheckHideObj();

	enum PICKOBJ_GROUP{FOE,SUP,DOOR,MCTRL};
	u16 PickObj(u16 x,u16 y,PICKOBJ_GROUP *retPickGroup);
	PICKOBJ_GROUP cur_objgroup,cur_sel_objgroup;
	u16 cur_obj,cur_sel_obj;
	bool IsListObjChangeFromSoftWare;
	afx_msg void OnLvnItemchangedListMapObj(NMHDR *pNMHDR, LRESULT *pResult);
	bool obj_moving;
	int obj_moving_ox,obj_moving_oy;
	int obj_moving_dx,obj_moving_dy;

	int Obj_GameIdtoListId(u16 id,PICKOBJ_GROUP group);


	afx_msg void OnDestroy();
	CButton m_CheckObjMovingAlign;
	CButton m_ButtonObjUp;
	CButton m_ButtonObjDown;
	CButton m_ButtonObjNew;
	CButton m_ButtonObjDelete;
	CButton m_ButtonObjProperty;
	afx_msg void OnBnClickedButtonObjUp();
	afx_msg void OnBnClickedButtonObjDown();
	afx_msg void OnBnClickedButtonObjNew();
	afx_msg void OnBnClickedButtonObjDelete();
	afx_msg void OnBnClickedButtonObjProperty();
	afx_msg void OnNMDblclkListMapObj(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNewFoe();
	afx_msg void OnNewSup();
	afx_msg void OnNewDoor();
	afx_msg void OnNewMctrl();
	afx_msg void OnBnClickedButtonMapCancel();
	afx_msg void OnBnClickedButtonMapResize();
	afx_msg void OnBnClickedButtonZeroCell();
	afx_msg void OnBnClickedButtonSwapPal();
	CButton m_ButtonBackgroundScript;
	afx_msg void OnBnClickedButtonBackgroundScript();
	void UpdateBackgroundScript();

	CComboBox m_ComboUnk10;
	CButton m_CheckUnk9;
	afx_msg void OnBnClickedCheckMapUnk9();
	afx_msg void OnCbnSelchangeComboMapUnk10();
	CButton m_ButtonGuide;
	afx_msg void OnBnClickedButtonGuide();
};
