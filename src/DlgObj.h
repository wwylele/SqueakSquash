#pragma once

#include "Main.h"
#include "afxwin.h"
class CHexEdit : public CEdit
{
public:
	DECLARE_MESSAGE_MAP()
public:
	CHexEdit();
	virtual ~CHexEdit();
	void *m_pAttach;
	DWORD m_DataLen;
	CString m_charBreak;
	void UpdateText();
private:
	CFont m_Font;
public:
	void Init();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

class CDlgObj : public CDialog
{
	DECLARE_DYNAMIC(CDlgObj)

public:
	CDlgObj(CWnd* pParent = NULL);   
	virtual ~CDlgObj();

	enum { IDD = IDD_OBJ };

	enum OBJ_GROUP{FOE,SUP,DOOR,MCTRL} objgroup;
	u16 x,y;
	u16 class_id;
	u8 param[70];
	CString title;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	CHexEdit m_EditObjParam;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	CEdit m_EditObjX;
	CEdit m_EditObjXp;
	CEdit m_EditObjY;
	CEdit m_EditObjYp;
	CComboBox m_ComboClass;
	afx_msg void OnCbnSelchangeComboObjClass();
};

