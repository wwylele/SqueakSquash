

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
	DDX_Control(pDX, IDC_COMBO_CHANGELIB_BGL_CTM, m_ComboBglCtm);
	DDX_Control(pDX, IDC_COMBO_CHANGELIB_FGL_CTM, m_ComboFglCtm);
}


BEGIN_MESSAGE_MAP(CDlgChangeLib, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANGELIB_BGL, &CDlgChangeLib::OnCbnSelchangeComboChangelibBgl)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANGELIB_FGL, &CDlgChangeLib::OnCbnSelchangeComboChangelibFgl)
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

	OnCbnSelchangeComboChangelibBgl();
	OnCbnSelchangeComboChangelibFgl();
	

	return TRUE;
}

void CDlgChangeLib::OnOK()
{
	char n1[16],n2[16];
	ASSERT((u8)m_ComboBg.GetCurSel()<pMapSet->GetBgCount());
	ASSERT((u8)m_ComboBGl.GetCurSel()<pMapSet->GetGlCount());
	ASSERT((u8)m_ComboFGl.GetCurSel()<pMapSet->GetGlCount());
	ASSERT((u8)m_ComboPl.GetCurSel()<pMapSet->GetPlCount()+1);
	Bg=(u8)m_ComboBg.GetCurSel();
	BGl=(u8)m_ComboBGl.GetCurSel();
	FGl=(u8)m_ComboFGl.GetCurSel();
	Pl=(u8)m_ComboPl.GetCurSel();
	pMapSet->GetGlName(BGl,n1);
	pMapSet->GetGlName(FGl,n2);
	if(strcmp(n1,TEXTURE__CHAINBOMB)==0 || strcmp(n2,TEXTURE__CHAINBOMB)==0)
	{
		MessageBox(_T("\"chainbomb.bin\" can't be chose."),_T("Squeak Squash"),MB_ICONERROR);
		return;
	}
	if(Pl==pMapSet->GetPlCount())Pl=0xFF;
	RetBGlCtm=m_ComboBglCtm.GetItemData(m_ComboBglCtm.GetCurSel());
	RetFGlCtm=m_ComboFglCtm.GetItemData(m_ComboFglCtm.GetCurSel());
	CDialog::OnOK();
}
#define COMBOCTM_NO _T("---")
void CDlgChangeLib::OnCbnSelchangeComboChangelibBgl()
{
	m_ComboBglCtm.ResetContent();
	CString str;
	BGl=(u8)m_ComboBGl.GetCurSel();
	u8 bb;
	u8 el,es;
	m_ComboBglCtm.SetItemData(m_ComboBglCtm.AddString(COMBOCTM_NO),0xFFFFFFFF);
	for(u32 i=0;i<pMapSet->GetStageCount();++i)
		for(u16 j=0;j<pMapSet->GetStepCount(i);++j)
			if(i!=StageIdx && j!=StepIdx)
			{
				pMapSet->GetStepInfo(i,j,0,&bb,0,0);
				if(bb==BGl)
				{
					pMapSet->GetStageEntry(i,&el,&es);
					str.Format(_T("Level%d:Stage%d:Step%02d"),el,es,j);
					m_ComboBglCtm.SetItemData(m_ComboBglCtm.AddString(str),
						(i<<16)|j);
				}
			}
	m_ComboBglCtm.SelectString(-1,COMBOCTM_NO);
}

void CDlgChangeLib::OnCbnSelchangeComboChangelibFgl()
{
	m_ComboFglCtm.ResetContent();
	FGl=(u8)m_ComboFGl.GetCurSel();
	CString str;
	u8 fb;
	u8 el,es;
	m_ComboFglCtm.SetItemData(m_ComboFglCtm.AddString(COMBOCTM_NO),0xFFFFFFFF);
	for(u32 i=0;i<pMapSet->GetStageCount();++i)
		for(u16 j=0;j<pMapSet->GetStepCount(i);++j)
			if(i!=StageIdx && j!=StepIdx)
			{
				pMapSet->GetStepInfo(i,j,0,0,&fb,0);
				if(fb==FGl)
				{
					pMapSet->GetStageEntry(i,&el,&es);
					str.Format(_T("Level%d:Stage%d:Step%02d"),el,es,j);
					m_ComboFglCtm.SetItemData(m_ComboFglCtm.AddString(str),
						(i<<16)|j);
				}
			}
	m_ComboFglCtm.SelectString(-1,COMBOCTM_NO);
}
