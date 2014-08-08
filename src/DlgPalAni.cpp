
#include "stdafx.h"
#include "Main.h"
#include "DlgPalAni.h"



IMPLEMENT_DYNAMIC(CDlgPalAni, CDialog)

CDlgPalAni::CDlgPalAni(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPalAni::IDD, pParent)
{

}

CDlgPalAni::~CDlgPalAni()
{
}

void CDlgPalAni::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PAL_FRAME, m_ListFrame);
	DDX_Control(pDX, IDC_EDIT_PAL_TIME, m_EditTime);
}


BEGIN_MESSAGE_MAP(CDlgPalAni, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_LBN_SELCHANGE(IDC_LIST_PAL_FRAME, &CDlgPalAni::OnLbnSelchangeListPalFrame)
	ON_EN_CHANGE(IDC_EDIT_PAL_TIME, &CDlgPalAni::OnEnChangeEditPalTime)
	ON_BN_CLICKED(IDC_BUTTON_PAL_NEW, &CDlgPalAni::OnBnClickedButtonPalNew)
	ON_BN_CLICKED(IDC_BUTTON_PAL_DELETE, &CDlgPalAni::OnBnClickedButtonPalDelete)
END_MESSAGE_MAP()


void CDlgPalAni::OnPaint()
{
	CPaintDC dc(this); 

	int framec;
	for(u8 line=0;line<16;++line){
		framec=sqpl.GetFrameCount(line);
		if(framec!=0){
			for(u8 frame=0;frame<min(framec,16);++frame)
				for(u8 i=0;i<16;++i)
				{
					dc.FillRect(&CRect( (i<<4),(line<<4)+frame,
						(i<<4)+16,(line<<4)+frame+1),
						&CBrush(Nitro::Color15to24(sqpl.GetFrame(line,frame).Pl[i])));
				}
		}
	}
	dc.FillRect(&CRect(256,0,280,256),&CBrush(RGB(200,200,200)));
	POINT pt[3];
	pt[0].x=257;pt[0].y=(curline<<4)+0;
	pt[1].x=277;pt[1].y=(curline<<4)+0;
	pt[2].x=277;pt[2].y=(curline<<4)+14;
	dc.Polygon(pt,3);

	if(curframe<sqpl.GetFrameCount(curline))for(u8 i=0;i<16;++i)
	{
		dc.FillRect(&CRect( (i<<4)+280,0,
			(i<<4)+296,16),
			&CBrush(Nitro::Color15to24(sqpl.GetFrame(curline,curframe).Pl[i])));
	}
}

BOOL CDlgPalAni::OnInitDialog()
{
	CDialog::OnInitDialog();

	curline=curframe=0;

	m_EditTime.SetLimitText(3);

	m_ListFrame.SetCurSel(0);
	OnLbnSelchangeListPalFrame();

	return TRUE;
}

void CDlgPalAni::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(point.x>=0 && point.x<280 && point.y>=0 &&point.y<256){
		curline=(u8)(point.y>>4);
		curframe=0;
		UpdateListFrame();
		m_ListFrame.SetCurSel(0);
		OnLbnSelchangeListPalFrame();
		RedrawWindow();
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgPalAni::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(point.x>=280 && point.x<280+256 && point.y>=0 &&point.y<16){
		if(curframe<sqpl.GetFrameCount(curline)){
			int i=(point.x-280)>>4;
			CColorDialog dlg;
			dlg.m_cc.Flags|=CC_RGBINIT|CC_FULLOPEN;
			dlg.m_cc.rgbResult=Nitro::Color15to24(sqpl.GetFrame(curline,curframe).Pl[i]);
			if(dlg.DoModal()==IDOK){
				sqpl.GetFrame(curline,curframe).Pl[i]=Nitro::Color24to15(dlg.m_cc.rgbResult);
				RedrawWindow();
			}
		}
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgPalAni::UpdateListFrame()
{
	m_ListFrame.ResetContent();

	CString str;
	for(u8 frame=0;frame<sqpl.GetFrameCount(curline);++frame)
	{
		str.Format(_T("[%d]Time=%d"),frame,sqpl.GetFrame(curline,frame).Time);
		m_ListFrame.AddString(str);
	}
}
void CDlgPalAni::OnLbnSelchangeListPalFrame()
{
	curframe=(u8)m_ListFrame.GetCurSel();
	if(curframe<sqpl.GetFrameCount(curline))
	{
		CString str;
		str.Format(_T("%d"),sqpl.GetFrame(curline,curframe).Time);
		m_EditTime.SetWindowText(str);
	}
	RedrawWindow();
}

void CDlgPalAni::OnEnChangeEditPalTime()
{
	if(curframe<sqpl.GetFrameCount(curline))
	{
		CString str;
		m_EditTime.GetWindowText(str);
		sqpl.GetFrame(curline,curframe).Time=(u8)_ttoi(str);
		str.Format(_T("[%d]Time=%d"),curframe,sqpl.GetFrame(curline,curframe).Time);
		m_ListFrame.DeleteString(curframe);
		m_ListFrame.InsertString(curframe,str);
		m_ListFrame.SetCurSel(curframe);
	}
}

void CDlgPalAni::OnBnClickedButtonPalNew()
{
	if(curframe<sqpl.GetFrameCount(curline) || sqpl.GetFrameCount(curline)==0)
	{
		if(sqpl.GetFrameCount(curline)>250){
			MessageBox(_T("Too many frames!"),_T("Error"),MB_ICONERROR);
			return;
		}
		sqpl.InsertFrame(curline,curframe);
		UpdateListFrame();
		m_ListFrame.SetCurSel(curframe);
		OnLbnSelchangeListPalFrame();
		RedrawWindow();
	}
}

void CDlgPalAni::OnBnClickedButtonPalDelete()
{
	if(curframe<sqpl.GetFrameCount(curline))
	{
		sqpl.DeleteFrame(curline,curframe);
		UpdateListFrame();
		m_ListFrame.SetCurSel(curframe);
		OnLbnSelchangeListPalFrame();
		RedrawWindow();
	}
}
