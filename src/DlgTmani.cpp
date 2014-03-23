
#include "stdafx.h"
#include "DlgTmani.h"




IMPLEMENT_DYNAMIC(CDlgTmani, CDialog)

CDlgTmani::CDlgTmani(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTmani::IDD, pParent)
{

}

CDlgTmani::~CDlgTmani()
{
}

void CDlgTmani::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_TMANI_FRAME, m_SliderFrame);
	DDX_Control(pDX, IDC_EDIT_TMANI_TIMEDELTA0, m_EditTime[0]);
	DDX_Control(pDX, IDC_EDIT_TMANI_TIMEDELTA1, m_EditTime[1]);
	DDX_Control(pDX, IDC_EDIT_TMANI_TIMEDELTA2, m_EditTime[2]);
	DDX_Control(pDX, IDC_EDIT_TMANI_TIMEDELTA3, m_EditTime[3]);
	DDX_Control(pDX, IDC_EDIT_TMANI_TIMEDELTA4, m_EditTime[4]);
	DDX_Control(pDX, IDC_EDIT_TMANI_TIMEDELTA5, m_EditTime[5]);
	DDX_Control(pDX, IDC_EDIT_TMANI_TIMEDELTA6, m_EditTime[6]);
	DDX_Control(pDX, IDC_EDIT_TMANI_TIMEDELTA7, m_EditTime[7]);

	DDX_Control(pDX, IDC_EDIT_TMANI_TMINDEX, m_EditTmindex);
}


BEGIN_MESSAGE_MAP(CDlgTmani, CDialog)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


BOOL CDlgTmani::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str;

	m_SliderFrame.SetRange(1,8);
	m_SliderFrame.SetPos(m_GraScript.FrameCount);
	OnSliderFrameChange();

	m_EditTmindex.SetLimitText(4);
	str.Format(_T("%d"),m_GraScript.TexMappingIndex);
	m_EditTmindex.SetWindowText(str);

	CheckRadioButton(0,30000,m_GraScript.TexMappingPlane?
		IDC_RADIO_TMANI_PLANE1:IDC_RADIO_TMANI_PLANE0);

	for(int i=0;i<8;++i)
	{
		m_EditTime[i].SetLimitText(3);
		str.Format(_T("%d"),m_GraScript.TimeDelta[i]);
		m_EditTime[i].SetWindowText(str);
	}

	return TRUE;
}

void CDlgTmani::OnOK()
{

	CString str;
	m_GraScript.FrameCount=(u8)m_SliderFrame.GetPos();

	m_EditTmindex.GetWindowText(str);
	m_GraScript.TexMappingIndex=(u16)_ttoi(str);

	switch(GetCheckedRadioButton(0,65535))
	{
	case IDC_RADIO_TMANI_PLANE0:m_GraScript.TexMappingPlane=0;break;
	case IDC_RADIO_TMANI_PLANE1:m_GraScript.TexMappingPlane=1;break;
	}

	int j=0;
	for(int i=0;i<8;++i)
	{
		if(i<m_GraScript.FrameCount)
		{
			m_EditTime[i].GetWindowText(str);
			j=_ttoi(str);
			if(j>255)j=255;
			m_GraScript.TimeDelta[i]=(u8)j;
		}
		else
		{
			m_GraScript.TimeDelta[i]=0;
		}
	}

	CDialog::OnOK();
}
void CDlgTmani::OnSliderFrameChange()
{
	for(int i=0;i<8;++i)
	{
		m_EditTime[i].EnableWindow(i<m_SliderFrame.GetPos()?TRUE:FALSE);
	}
}
void CDlgTmani::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	OnSliderFrameChange();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
