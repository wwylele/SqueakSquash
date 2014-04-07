#pragma once
#include "afxwin.h"
#include "afxcmn.h"




class CDlgImportBg : public CDialog
{
	DECLARE_DYNAMIC(CDlgImportBg)

public:
	CDlgImportBg(CWnd* pParent = NULL);  
	virtual ~CDlgImportBg();

	enum { IDD = IDD_IMPORTBG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_ComboBgStyle;
	CImage *m_ImageIn;
	u8 *m_DataBuf;
	u32 m_DataLen;
protected:
	static DWORD WINAPI Thread_ImportBg(LPVOID lpThreadParameter);
	void Thread_ImportBg_Entry();
public:
	CProgressCtrl m_Progress;
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnCbnSelchangeComboImportbg();
	afx_msg void OnBnClickedButtonImportbg();
protected:
	virtual void OnOK();
};
