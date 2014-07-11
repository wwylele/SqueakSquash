

#include "stdafx.h"
#include "DlgRomInfo.h"




IMPLEMENT_DYNAMIC(CDlgRomInfo, CDialog)

CDlgRomInfo::CDlgRomInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRomInfo::IDD, pParent)
{

}

CDlgRomInfo::~CDlgRomInfo()
{
}

void CDlgRomInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ROM_TITLE, m_EditTitle);
}


BEGIN_MESSAGE_MAP(CDlgRomInfo, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgRomInfo::OnBnClickedOk)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


BOOL CDlgRomInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	drawing=false;

	curcolor=0;

	m_EditTitle.SetLimitText(120);

	CString str;
#ifdef _UNICODE
	str=(wchar_t*)(m_RomInfo.Title_text[0]);
#else
	str.Format(_T("%S"),(wchar_t*)(m_RomInfo.Title_text[0]));
#endif
	str.Replace(_T("\n"),_T("\r\n"));

	m_EditTitle.SetWindowText(str);


	return TRUE;
}

void CDlgRomInfo::OnBnClickedOk()
{
	CString str;
	CStringW strw;
	m_EditTitle.GetWindowText(str);
#ifdef _UNICODE
	strw=str;
#else
	strw.Format(_T("%S"),(char*)str);
#endif
	ZeroMemory(m_RomInfo.Title_text,256*6);
	for(int i=0;i<6;++i)
	{
		wcscpy((wchar_t*)(m_RomInfo.Title_text[i]),strw);
	}
	OnOK();
}
void CDlgRomInfo::PaintIcon(CDC* pDC)
{
	u8 pixl;
	CBrush brush[16];
	for(int bi=0;bi<16;bi++)brush[bi].CreateSolidBrush(Nitro::Color15to24(m_RomInfo.Title_icon_palette[bi]));
	for(int by=0;by<4;by++)for(int bx=0;bx<4;bx++)
	for(int ty=0;ty<8;ty++)for(int tx=0;tx<8;tx++)
	{
		if(tx&1)
		{
			pixl=m_RomInfo.Title_icon_pixel[by][bx][ty][tx/2]>>4;
		}
		else
		{
			pixl=m_RomInfo.Title_icon_pixel[by][bx][ty][tx/2]&0xF;
		}
		pDC->FillRect(&CRect(
			((bx<<3)+tx)<<3,((by<<3)+ty)<<3,
			((bx<<3)+tx+1)<<3,((by<<3)+ty+1)<<3),&brush[pixl]);
		pDC->SetPixel((bx<<3)+tx+256,(by<<3)+ty,Nitro::Color15to24(m_RomInfo.Title_icon_palette[pixl]));
		
	}
	pDC->FillRect(&CRect(256,128,256+32,256),&brush[0]);
	for(int bi=1;bi<16;bi++){
		pDC->FillRect(&CRect(
			256,128+(bi<<3),
			256+24,136+(bi<<3)),&brush[bi]);
	}
	
	POINT pt[3];
	pt[0].x=256+24;pt[0].y=132+(curcolor<<3);
	pt[1].x=256+31;pt[1].y=129+(curcolor<<3);
	pt[2].x=256+31;pt[2].y=135+(curcolor<<3);
	pDC->Polygon(pt,3);
}
void CDlgRomInfo::OnPaint()
{
	CPaintDC dc(this);
	PaintIcon(&dc);
}

void CDlgRomInfo::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(point.x>256 && point.x<256+32 &&
		point.y>128 && point.y<256)
	{
		curcolor=(u8)((point.y-128)>>3);
		CDC* pDC=GetDC();
		PaintIcon(pDC);
		ReleaseDC(pDC);
	}
	if(point.x>=0 && point.x<256 && point.y>=0 && point.y<256)
	{
		SetCapture();
		drawing=true;
		OnMouseMove(0,point);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgRomInfo::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(point.x>256 && point.x<256+32 &&
		point.y>128 && point.y<256)
	{
		CColorDialog dlg;
		dlg.m_cc.Flags|=CC_RGBINIT|CC_FULLOPEN;
		dlg.m_cc.rgbResult=Nitro::Color15to24(
			m_RomInfo.Title_icon_palette[curcolor]);
		if(dlg.DoModal()==IDOK){
			m_RomInfo.Title_icon_palette[curcolor]=Nitro::Color24to15(dlg.m_cc.rgbResult);
			CDC* pDC=GetDC();
			PaintIcon(pDC);
			ReleaseDC(pDC);
		}
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CDlgRomInfo::OnMouseMove(UINT nFlags, CPoint point)
{
	u8 x,y,bx,by;
	if(drawing && point.x>=0 && point.x<256 && point.y>=0 && point.y<256)
	{
		x=(u8)(point.x>>3);
		y=(u8)(point.y>>3);
		bx=x>>3;
		by=y>>3;
		u8 &op=m_RomInfo.Title_icon_pixel[by][bx][y&7][(x>>1)&3];
		if(x&1)
		{
			op&=0xF;
			op|=curcolor<<4;
		}
		else
		{
			op&=0xF0;
			op|=curcolor;
		}

		CDC* pDC=GetDC();
		PaintIcon(pDC);
		ReleaseDC(pDC);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CDlgRomInfo::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	drawing=false;

	CDialog::OnLButtonUp(nFlags, point);
}
