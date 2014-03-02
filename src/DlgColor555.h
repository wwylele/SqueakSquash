#pragma once


class CDlgColor555 : public CDialog
{
	DECLARE_DYNAMIC(CDlgColor555)

public:
	CDlgColor555(CWnd* pParent = NULL);  
	virtual ~CDlgColor555();

	enum { IDD = IDD_COLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
