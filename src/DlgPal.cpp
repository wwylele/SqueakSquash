

#include "stdafx.h"
#include "DlgPal.h"



IMPLEMENT_DYNAMIC(CDlgPal, CDialog)

CDlgPal::CDlgPal(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPal::IDD, pParent)
{

}

CDlgPal::~CDlgPal()
{
}

void CDlgPal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPal, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


void CDlgPal::OnPaint()
{
	CPaintDC dc(this); 
	for(int i=0;i<256;++i)
	{
		dc.FillRect(&CRect((i&15)<<4,(i>>4)<<4,((i&15)<<4)+16,((i>>4)<<4)+16),
			&CBrush(Nitro::Color15to24(PalData[i])));
	}
}

void CDlgPal::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(point.x<256 && point.y<256 && point.x>=0 && point.y>=0)
	{
		int i=(point.x>>4)+((point.y>>4)<<4);
		CColorDialog dlg;
		dlg.m_cc.Flags|=CC_RGBINIT|CC_FULLOPEN;
		dlg.m_cc.rgbResult=Nitro::Color15to24(PalData[i]);
		if(dlg.DoModal()==IDOK){
			PalData[i]=Nitro::Color24to15(dlg.m_cc.rgbResult);
			RedrawWindow();
		}
	}

	CDialog::OnLButtonUp(nFlags, point);
}
