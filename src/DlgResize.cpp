

#include "stdafx.h"
#include "DlgResize.h"




IMPLEMENT_DYNAMIC(CDlgResize, CDialog)

CDlgResize::CDlgResize(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgResize::IDD, pParent)
{

}

CDlgResize::~CDlgResize()
{
}

void CDlgResize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RESIZE_W, m_EditW);
	DDX_Control(pDX, IDC_EDIT_RESIZE_H, m_EditH);
}


BEGIN_MESSAGE_MAP(CDlgResize, CDialog)
END_MESSAGE_MAP()



void CDlgResize::OnOK()
{
	CString str;
	int h;

	m_EditW.GetWindowText(str);
	h=_ttoi(str);
	if(h<1||h>250)
	{
		m_EditW.SetFocus();
		m_EditW.SetSel(0,-1);
		MessageBeep(MB_OK);
		return;
	}
	m_W=(u8)h;

	m_EditH.GetWindowText(str);
	h=_ttoi(str);
	if(h<1||h>250)
	{
		m_EditH.SetFocus();
		m_EditH.SetSel(0,-1);
		MessageBeep(MB_OK);
		return;
	}
	m_H=(u8)h;
	switch(GetCheckedRadioButton(0,65535))
	{
	case IDC_RADIO_RESIZE_UL:m_Ox=m_Oy=0;break;
	case IDC_RADIO_RESIZE_UR:m_Ox=(int)m_W-m_Wo;m_Oy=0;break;
	case IDC_RADIO_RESIZE_DL:m_Ox=0;m_Oy=(int)m_H-m_Ho;break;
	case IDC_RADIO_RESIZE_DR:m_Ox=(int)m_W-m_Wo;m_Oy=(int)m_H-m_Ho;break;
	}

	CDialog::OnOK();
}

BOOL CDlgResize::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_EditW.SetLimitText(3);
	m_EditH.SetLimitText(3);
	CString str;
	str.Format(_T("%d"),m_Wo=m_W);
	m_EditW.SetWindowText(str);
	str.Format(_T("%d"),m_Ho=m_H);
	m_EditH.SetWindowText(str);
	m_Ox=m_Oy=0;
	CheckRadioButton(0,65535,IDC_RADIO_RESIZE_UL);

	return TRUE;
}
