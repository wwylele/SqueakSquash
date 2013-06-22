#include "SqMapSet.h"

#pragma once



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
	HTREEITEM m_htiRom,m_htiMap,m_htiBg,m_htiGl,m_htiPl;
	CArray<HTREEITEM> m_htiMapi;


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
	afx_msg BOOL OnTtnNeedText(UINT id, NMHDR *pNMHDR, LRESULT *pResult);



};


