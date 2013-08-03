#pragma once
#include "Main.h"
#include "SqMapSet.h"
#include "afxwin.h"


class CDlgChangeLib : public CDialog
{
	DECLARE_DYNAMIC(CDlgChangeLib)

public:
	CDlgChangeLib(CWnd* pParent = NULL);
	virtual ~CDlgChangeLib();

	SqMapSet* pMapSet;
	u8 Bg,BGl,FGl,Pl;
	

	enum { IDD = IDD_CHANGELIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_ComboBg;
	CComboBox m_ComboBGl;
	CComboBox m_ComboFGl;
	CComboBox m_ComboPl;
protected:
	virtual void OnOK();
};
