#pragma once

#include "Main.h"
#include "afxwin.h"
#include "HexEdit.h"
#include "afxcmn.h"
#include "SqMa.h"

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
	SqMa* pMa;


	CDialogBar m_BarParamDecode;

	struct RelMctrlPreset_Binding
	{
		CButton* pButton;
		int EditId;
	};
	int CurEditId;
	CList<RelMctrlPreset_Binding> RelMctrlPresetListEntry;
#define IDC_BUTTON_OP_M_REL_BASE 3800
#define ID_MENU_OP_M_REL_BASE 3900
	void InitBarParamDecode();
	void BindRelMctrlPreset(int EditId);
	void CleanRelMctrlPreset();

	afx_msg void OnBnClicked_M_Rel(UINT ButtonId);
	afx_msg void OnMenu_M_Rel(UINT MenuId);

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
	CComboBoxEx m_ComboClass;
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

	afx_msg void OnTcnSelchange_M_1_Count(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChange_M_1_Rel0();
	afx_msg void OnEnChange_M_1_Rel1();
	afx_msg void OnEnChange_M_1_RopeLength();
	afx_msg void OnCbnSelchange_M_1_Style();

	afx_msg void OnBnClicked_M_2_Reverse();
	afx_msg void OnEnChange_M_2_HoldTime();
	afx_msg void OnCbnSelchange_M_2_StyleMain();
	afx_msg void OnCbnSelchange_M_2_StyleEx();

	afx_msg void OnBnClicked_M_3_Fall();
	afx_msg void OnBnClicked_M_3_Unk();

	afx_msg void OnCbnSelchange_M_5_Style();
	afx_msg void OnCbnSelchange_M_5_SubStyle();
	afx_msg void OnTcnSelchange_M_5_Count(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChange_M_5_DX0();
	afx_msg void OnEnChange_M_5_DY0();
	afx_msg void OnEnChange_M_5_DX1();
	afx_msg void OnEnChange_M_5_DY1();
	afx_msg void OnEnChange_M_5_DX2();
	afx_msg void OnEnChange_M_5_DY2();

	afx_msg void OnCbnSelchange_M_14_Style();
	afx_msg void OnBnClicked_M_14_Sup();
	afx_msg void OnEnChange_M_14_Rel();

	afx_msg void OnBnClicked_M_15_Sup();
	afx_msg void OnEnChange_M_15_Rel();

	afx_msg void OnBnClicked_M_17_Sup();
	afx_msg void OnBnClicked_M_17_Dust();
	afx_msg void OnEnChange_M_17_Rel();

	afx_msg void OnTcnSelchange_M_22_Count(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChange_M_22_Rel0();
	afx_msg void OnEnChange_M_22_Rel1();
	afx_msg void OnEnChange_M_22_Rel2();
	afx_msg void OnEnChange_M_22_Rel3();
	afx_msg void OnEnChange_M_22_Rel4();
	afx_msg void OnEnChange_M_22_Time0();
	afx_msg void OnEnChange_M_22_Time1();
	afx_msg void OnEnChange_M_22_Time2();
	afx_msg void OnEnChange_M_22_Time3();
	afx_msg void OnEnChange_M_22_Time4();
	afx_msg void OnBnClicked_M_22_WaitBoss();

	afx_msg void OnEnChange_M_23_Rel0();
	afx_msg void OnEnChange_M_23_Rel1();
	afx_msg void OnEnChange_M_23_Rel2();
	afx_msg void OnEnChange_M_23_Rel3();
	afx_msg void OnEnChange_M_23_Rel4();
	afx_msg void OnEnChange_M_23_Rel5();
	afx_msg void OnEnChange_M_23_Rel6();
	afx_msg void OnEnChange_M_23_Rel7();

	afx_msg void OnEnChange_M_24_Width();
	afx_msg void OnEnChange_M_24_Height();
	afx_msg void OnCbnSelchange_M_24_Style();

	afx_msg void OnCbnSelchange_M_Single();
	afx_msg void OnDestroy();
};

