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


	CString m_StrFileName;


	SqMapSet m_SqMapSet;
public:
	virtual ~CMainFrame();

	void FlushFileTree();


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


};


