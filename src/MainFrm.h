#include "SqMapSet.h"

#pragma once

#ifdef _UNICODE
#define FORMAT_A2T L"%S"
#define FORMAT_W2T L"%s"
#else
#define FORMAT_A2T "%s"
#define FORMAT_W2T "%S"
#endif

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

	CStatic m_StaticDesc;
	CStatic m_StaticPrvw;
#define ID_STATIC_PRVW 20002

	CString m_StrFileName;
	CDC m_DCPrvw;
	CBitmap m_BmpPrvw;
//#define BMP_PRVW_W 1024
//#define BMP_PRVW_H 512
	int BMP_PRVW_W,BMP_PRVW_H;

	SqMapSet m_SqMapSet;
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
	afx_msg void OnTbOpen();
	afx_msg void OnTbSave();
	afx_msg void OnTbSaveas();
	afx_msg BOOL OnTtnNeedText(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickFileTreeFile(NMHDR *pNMHDR, LRESULT *pResult);


	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};


