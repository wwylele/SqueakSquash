

#include "stdafx.h"
#include "DlgTmResize.h"




IMPLEMENT_DYNAMIC(CDlgTmResize, CDialog)

CDlgTmResize::CDlgTmResize(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTmResize::IDD, pParent)
{

}

CDlgTmResize::~CDlgTmResize()
{
}

void CDlgTmResize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TEXMAPPING_COUNT, m_EditTexMappingCount);
}


BEGIN_MESSAGE_MAP(CDlgTmResize, CDialog)
END_MESSAGE_MAP()



BOOL CDlgTmResize::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString str;
	str.Format(_T("%d"),m_TexMappingCount);
	m_EditTexMappingCount.SetLimitText(4);
	m_EditTexMappingCount.SetWindowText(str);

	return TRUE;
}

void CDlgTmResize::OnOK()
{
	CString str;
	m_EditTexMappingCount.GetWindowText(str);

	m_TexMappingCount=(u16)_ttoi(str);
	m_TexMappingCount&=0xFFC0;
	if(m_TexMappingCount==0)
	{
		m_EditTexMappingCount.SetFocus();
		m_EditTexMappingCount.SetSel(0,-1);
		MessageBeep(MB_OK);
		return;
	}

	CDialog::OnOK();
}
