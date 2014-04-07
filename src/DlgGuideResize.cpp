
#include "stdafx.h"
#include "DlgGuideResize.h"



IMPLEMENT_DYNAMIC(CDlgGuideResize, CDialog)

CDlgGuideResize::CDlgGuideResize(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGuideResize::IDD, pParent)
{

}

CDlgGuideResize::~CDlgGuideResize()
{
}

void CDlgGuideResize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_GUIDE_RESIZE, m_Edit);
}


BEGIN_MESSAGE_MAP(CDlgGuideResize, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgGuideResize::OnBnClickedOk)
END_MESSAGE_MAP()


void CDlgGuideResize::OnBnClickedOk()
{
	CString str;
	m_Edit.GetWindowText(str);

	int i=(u16)_ttoi(str);
	if(i<0 || i>200)
	{
		m_Edit.SetFocus();
		m_Edit.SetSel(0,-1);
		MessageBeep(MB_OK);
		return;
	}
	m_GuideCount=(u16)i;

	OnOK();
}

BOOL CDlgGuideResize::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Edit.SetLimitText(3);
	CString str;
	str.Format(_T("%d"),m_GuideCount);
	m_Edit.SetWindowText(str);

	return TRUE;
}
