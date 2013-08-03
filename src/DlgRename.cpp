

#include "stdafx.h"
#include "DlgRename.h"


IMPLEMENT_DYNAMIC(CDlgRename, CDialog)

CDlgRename::CDlgRename(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRename::IDD, pParent)
{

}

CDlgRename::~CDlgRename()
{
}

void CDlgRename::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RENAME, m_EditRename);
}


BEGIN_MESSAGE_MAP(CDlgRename, CDialog)
END_MESSAGE_MAP()


BOOL CDlgRename::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_EditRename.SetWindowText(m_NameIO);
	m_EditRename.SetLimitText(15);

	return TRUE;  
}

void CDlgRename::OnOK()
{
	m_EditRename.GetWindowText(m_NameIO);

	CDialog::OnOK();
}
