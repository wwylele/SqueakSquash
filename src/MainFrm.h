#include "SqMapSet.h"

#pragma once

#define EXFEX_MXP "sqmxp" //"mxp"
#define EXFEX_DOE "sqdoe" //"doe"
#define EXFEX_BIN "sqbin" //"bin"
#define EXFEX_PAL "sqpal" //"pal"
#define EXFEX_NSBTX "nsbtx" //"nsbtx"(Nitro common)


class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	CToolBar m_ToolBar;
	CTreeCtrl m_FileTree;
#define ID_FILETREE 20001
	HTREEITEM m_htiRom,m_htiMap,m_htiBg,m_htiGl,m_htiPl;
	CArray<HTREEITEM> m_htiMapi;
	enum {
		FTS_0,FTS_ROM,FTS_STAGE,FTS_STEP,FTS_BG,FTS_GL,FTS_PL,
		FTS_MAP,FTS_BGF,FTS_GLF,FTS_PLF
	}m_TreeSelCls;
	u32 m_TreeSelIdx,m_TreeSelIdx2;

	CStatic m_StaticDesc;
	CStatic m_StaticPrvw;
#define ID_STATIC_PRVW 20002

	CButton m_ButtonOption;
#define ID_BUTTON_OPERATION 20003
#define ID_BOPTM_ROM		30001
#define ID_BOPTM_ADDSTEP	30002
#define ID_BOPTM_EDIT		30003
#define ID_BOPTM_CHANGELIB	30004
#define ID_BOPTM_TEXMAP		30005
#define ID_BOPTM_DELETE		30006
#define ID_BOPTM_MOVE		30007
#define ID_BOPTM_COPY		30008
#define ID_BOPTM_RENAME		30009
#define ID_BOPTM_EXPORT		30010
#define ID_BOPTM_IMPORT		30011

	CString m_StrFileName;
	CDC m_DCPrvw;
	CBitmap m_BmpPrvw;
//#define BMP_PRVW_W 1024
//#define BMP_PRVW_H 512
	int BMP_PRVW_W,BMP_PRVW_H;

	SqMapSet m_SqMapSet;

	CBitmap m_BmpDet0;
	//CBrush m_BrushDet0;
public:
	virtual ~CMainFrame();

	void FlushFileTree();
	void PaintRomPrvw();
	void PaintStepPrvw(u32 Stage,u16 Step);
	void PaintBgPrvw(u32 index);
	void PaintGlPrvw(u32 index);
	void PaintPlPrvw(u32 index);


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAbout();
	afx_msg void OnTbOpen();
	afx_msg void OnTbSave();
	afx_msg void OnTbSaveas();
	afx_msg void OnTbMake();
	afx_msg void OnTbTestGame();
	afx_msg void OnButtonOperation();
	afx_msg BOOL OnTtnNeedText(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFileTreeSelChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFileTreeDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	afx_msg void OnBoptmRename();
	afx_msg void OnBoptmChangeLib();
	afx_msg void OnBoptmDelete();
	afx_msg void OnBoptmCopy();
	afx_msg void OnBoptmEdit();
	afx_msg void OnBoptmExport();
	afx_msg void OnBoptmImport();
};


