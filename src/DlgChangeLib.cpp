

#include "stdafx.h"
#include "DlgChangeLib.h"




IMPLEMENT_DYNAMIC(CDlgChangeLib, CDialog)

CDlgChangeLib::CDlgChangeLib(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChangeLib::IDD, pParent)
{

}

CDlgChangeLib::~CDlgChangeLib()
{
}

void CDlgChangeLib::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CHANGELIB_BG, m_ComboBg);
	DDX_Control(pDX, IDC_COMBO_CHANGELIB_BGL, m_ComboBGl);
	DDX_Control(pDX, IDC_COMBO_CHANGELIB_FGL, m_ComboFGl);
	DDX_Control(pDX, IDC_COMBO_CHANGELIB_PL, m_ComboPl);
}


BEGIN_MESSAGE_MAP(CDlgChangeLib, CDialog)
END_MESSAGE_MAP()




BOOL CDlgChangeLib::OnInitDialog()
{
	CDialog::OnInitDialog();
	char strn[16];
	CString str;
	for(u8 i=0;i<pMapSet->GetBgCount();++i)
	{
		pMapSet->GetBgName(i,strn);
		str.Format(FORMAT_A2T,strn);
		m_ComboBg.AddString(str);
		if(i==Bg)m_ComboBg.SelectString(-1,str);

	}
	for(u8 i=0;i<pMapSet->GetGlCount();++i)
	{
		pMapSet->GetGlName(i,strn);
		str.Format(FORMAT_A2T,strn);
		m_ComboBGl.AddString(str);
		m_ComboFGl.AddString(str);
		if(i==BGl)m_ComboBGl.SelectString(-1,str);
		if(i==FGl)m_ComboFGl.SelectString(-1,str);
	}
	for(u8 i=0;i<pMapSet->GetPlCount();++i)
	{
		pMapSet->GetPlName(i,strn);
		str.Format(FORMAT_A2T,strn);
		m_ComboPl.AddString(str);
		if(i==Pl)m_ComboPl.SelectString(-1,str);
	}
	str=_T("None");
	m_ComboPl.AddString(str);
	if(Pl==0xFF)m_ComboPl.SelectString(-1,str);
	

	return TRUE;
}

void CDlgChangeLib::OnOK()
{
	ASSERT((u8)m_ComboBg.GetCurSel()<pMapSet->GetBgCount());
	ASSERT((u8)m_ComboBGl.GetCurSel()<pMapSet->GetGlCount());
	ASSERT((u8)m_ComboFGl.GetCurSel()<pMapSet->GetGlCount());
	ASSERT((u8)m_ComboPl.GetCurSel()<pMapSet->GetPlCount()+1);
	Bg=(u8)m_ComboBg.GetCurSel();
	BGl=(u8)m_ComboBGl.GetCurSel();
	FGl=(u8)m_ComboFGl.GetCurSel();
	Pl=(u8)m_ComboPl.GetCurSel();
	if(Pl==pMapSet->GetPlCount())Pl=0xFF;
	CDialog::OnOK();
}
