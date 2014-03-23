
#include "stdafx.h"
#include "DlgBgScript.h"




IMPLEMENT_DYNAMIC(CDlgBgScript, CDialog)

CDlgBgScript::CDlgBgScript(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBgScript::IDD, pParent)
{

}

CDlgBgScript::~CDlgBgScript()
{
}

void CDlgBgScript::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditScript);
}


BEGIN_MESSAGE_MAP(CDlgBgScript, CDialog)
END_MESSAGE_MAP()



BOOL CDlgBgScript::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_EditScript.Init();
	m_EditScript.m_pAttach=m_Script;
	m_EditScript.m_DataLen=6;
	m_EditScript.UpdateText();

	return TRUE;
}
