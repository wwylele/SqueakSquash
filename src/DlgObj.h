#pragma once

#include "Main.h"
#include "afxwin.h"
#include "HexEdit.h"
#include "afxcmn.h"

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
	u8 mctrl_param[2];
	CString title;
	CDialogBar m_BarParamDecode;

	void InitBarParamDecode();
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
	CEdit m_EditMctrl1;
	CButton m_CheckMctrl2;
	afx_msg void OnCbnSelchangeComboObjClass();
	CTabCtrl m_TabParam;
	afx_msg void OnTcnSelchangeTabParam(NMHDR *pNMHDR, LRESULT *pResult);
	void ParamDecode();

	afx_msg void OnBnClicked_S_Invisivle();
	afx_msg void OnBnClicked_S_Bubble();
	afx_msg void OnBnClicked_S_Sqnow();
	afx_msg void OnBnClicked_S_FF();
	afx_msg void OnBnClicked_S_Treasure();
	afx_msg void OnCbnSelchange_S_Treasure();

	afx_msg void OnEnChange_D_Step();
	afx_msg void OnEnChange_D_Door();
	
	afx_msg void OnTcnSelchange_M_0_Count(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChange_M_0_Rel0();
	afx_msg void OnEnChange_M_0_Rel1();
	afx_msg void OnEnChange_M_0_Rel2();

	afx_msg void OnBnClicked_M_3_Fall();
	afx_msg void OnBnClicked_M_3_Unk();
};

