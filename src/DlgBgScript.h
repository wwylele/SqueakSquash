#pragma once
#include "Main.h"
#include "HexEdit.h"
#include "afxwin.h"

class CDlgBgScript : public CDialog
{
	DECLARE_DYNAMIC(CDlgBgScript)

public:
	CDlgBgScript(CWnd* pParent = NULL);
	virtual ~CDlgBgScript();

	enum { IDD = IDD_BACKGROUND_SCRIPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()
public:
	CHexEdit m_EditScript;
	u8 m_Script[6];
	virtual BOOL OnInitDialog();
};
