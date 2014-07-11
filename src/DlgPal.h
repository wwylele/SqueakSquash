#pragma once


#include "Main.h"
#include "Nitro.h"

class CDlgPal : public CDialog
{
	DECLARE_DYNAMIC(CDlgPal)

public:
	CDlgPal(CWnd* pParent = NULL); 
	virtual ~CDlgPal();

	Nitro::Color15 PalData[256];

	enum { IDD = IDD_PAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
