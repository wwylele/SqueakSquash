#include "SqMapSet.h"

#pragma once

#ifdef _UNICODE
#define FORMAT_S2S L"%S"
#else
#define FORMAT_S2S "%s"
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
#define BMP_PRVW_W 512
#define BMP_PRVW_H 512

	SqMapSet m_SqMapSet;
public:
	virtual ~CMainFrame();

	void FlushFileTree();
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


