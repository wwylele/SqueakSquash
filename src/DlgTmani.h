#pragma once

#include "Main.h"
#include "SqMa.h"
#include "afxcmn.h"
#include "afxwin.h"
class CDlgTmani : public CDialog
{
	DECLARE_DYNAMIC(CDlgTmani)

public:
	CDlgTmani(CWnd* pParent = NULL); 
	virtual ~CDlgTmani();

	enum { IDD = IDD_TMANI };
	SqMa::SqGraScript m_GraScript;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	CSliderCtrl m_SliderFrame;
	CEdit m_EditTime[8];
	CEdit m_EditTmindex;
	void OnSliderFrameChange();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
