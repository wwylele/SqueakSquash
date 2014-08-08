
#include "stdafx.h"
#include "Main.h"
#include "DlgGl.h"
#include "DlgPal.h"



IMPLEMENT_DYNAMIC(CDlgGl, CDialog)

CDlgGl::CDlgGl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGl::IDD, pParent)
{
	ZeroMemory(Tile,sizeof(Tile));
	ZeroMemory(PalLine,sizeof(PalLine));

}

CDlgGl::~CDlgGl()
{
}

void CDlgGl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_GL_EXTEND, m_CheckExtend);
}


BEGIN_MESSAGE_MAP(CDlgGl, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_GL_PAL, &CDlgGl::OnBnClickedButtonGlPal)
	ON_BN_CLICKED(IDC_CHECK_GL_EXTEND, &CDlgGl::OnBnClickedCheckGlExtend)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



BOOL CDlgGl::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_CheckExtend.SetCheck(extend?TRUE:FALSE);
	cursel=curclr=0;
	drawing=false;

	Can.Create(256,288);
	PaintTile();
	return TRUE;
}

void CDlgGl::OnPaint()
{
	CPaintDC dc(this);
	Can.Present(dc.GetSafeHdc(),
		0,0,256,extend?288:256,
		0,0);

	for(u8 bx=0;bx<8;++bx)for(u8 by=0;by<8;++by){
		dc.FillRect(&CRect((bx<<4)+256,by<<4,(bx<<4)+272,(by<<4)+16),
			&CBrush(Color15to24(
				Pal[ (PalLine[cursel]<<4) | Tile[cursel].Get(bx,by)])));
	}

	for(u8 by=0;by<16;++by){
		
		for(u8 bx=0;bx<16;++bx){
			
			dc.FillRect(&CRect((bx<<1)+400,(by<<3)+0,(bx<<1)+402,(by<<3)+8),
				&CBrush(Color15to24(Pal[ bx | (by<<4)])));
		}

		dc.FillRect(&CRect((by<<3)+256,140,(by<<3)+264,160),
			&CBrush(Color15to24(
				Pal[ (PalLine[cursel]<<4) | by])));
	}

	dc.FillRect(&CRect(390,0,400,128),&CBrush(RGB(200,200,200)));
	dc.FillRect(&CRect(256,130,384,140),&CBrush(RGB(200,200,200)));
	POINT pt[3];
	pt[0].x=390;pt[0].y=(PalLine[cursel]<<3)+0;
	pt[1].x=399;pt[1].y=(PalLine[cursel]<<3)+4;
	pt[2].x=390;pt[2].y=(PalLine[cursel]<<3)+8;
	dc.Polygon(pt,3);
	pt[0].x=(curclr<<3)+256;pt[0].y=130;
	pt[1].x=(curclr<<3)+264;pt[1].y=130;
	pt[2].x=(curclr<<3)+260;pt[2].y=139;
	dc.Polygon(pt,3);

	static CPen PenCur(PS_SOLID,1,RGB(255,0,0));
	CPen *pOldPen;
	CBrush NullBrush,*pOldBrush;
	NullBrush.CreateStockObject(NULL_BRUSH);
	pOldPen=dc.SelectObject(&PenCur);
	pOldBrush=dc.SelectObject(&NullBrush);
	int x=(cursel&31)<<3;
	int y=(cursel>>5)<<3;
	
	dc.Rectangle(x,y,x+8,y+8);


	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
	NullBrush.DeleteObject();
}

void CDlgGl::PaintTile()
{
	for(int i=0;i<(extend?0x480:0x400);++i)
	{
		int x,y;
		x=(i&31)<<3;
		y=(i>>5)<<3;
		for(u8 bx=0;bx<8;++bx)for(u8 by=0;by<8;++by)
		{
			Can.SetPixel(x+bx,y+by,Nitro::Color15to24(
				Pal[ (PalLine[i]<<4) | Tile[i].Get(bx,by)]));
		}
	}

}
void CDlgGl::OnBnClickedButtonGlPal()
{
	CDlgPal dlg;
	memcpy(dlg.PalData,Pal,0x200);
	if(dlg.DoModal()==IDOK){
		memcpy(Pal,dlg.PalData,0x200);
		PaintTile();
		RedrawWindow();
	}
}

void CDlgGl::OnBnClickedCheckGlExtend()
{
	extend=m_CheckExtend.GetCheck()?true:false;
	if(cursel>=0x400)cursel=0;
	PaintTile();
	RedrawWindow();
}

void CDlgGl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(drawing)
	{
		drawing=false;
		ReleaseCapture();
		PaintTile();
		RedrawWindow();
		return;
	}
	CDialog::OnLButtonUp(nFlags, point);
}
void CDlgGl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(drawing)
	{
	}
	else if(point.x>=256 && point.y>=0 && point.x<384 && point.y<128)
	{
		drawing=true;
		SetCapture();
		OnMouseMove(nFlags,point);
	}
	else if(point.x>=0 && point.y>=0 && point.x<256 && point.y<(extend?288:256))
	{
		cursel=(point.x>>3)|((point.y>>3)<<5);
		PaintTile();
		RedrawWindow();
	}
	else if(point.x>=390 && point.y>=0 && point.x<432 && point.y<128)
	{
		PalLine[cursel]=(u8)(point.y>>3);
		PaintTile();
		RedrawWindow();
	}
	else if(point.x>=256 && point.y>=130 && point.x<384 && point.y<160)
	{
		curclr=(point.x-256)>>3;
		RedrawWindow(&CRect(256,130,384,160));
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgGl::OnMouseMove(UINT nFlags, CPoint point)
{
	if(drawing && point.x>=256 && point.y>=0 && point.x<384 && point.y<128)
	{
		int x,y;
		x=(point.x-256)>>4;
		y=point.y>>4;
		Tile[cursel].Set( (u8)x,(u8)y,curclr);
		RedrawWindow(&CRect(256+(x<<4),y<<4,272+(x<<4),16+(y<<4)));
	}

	CDialog::OnMouseMove(nFlags, point);
}
