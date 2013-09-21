
//

#include "stdafx.h"
#include "DlgMapEdit.h"
#include "Det.h"
#include "Bgm.h"


IMPLEMENT_DYNAMIC(CDlgMapEdit, CDialog)

CDlgMapEdit::CDlgMapEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMapEdit::IDD, pParent)
{

}

CDlgMapEdit::~CDlgMapEdit()
{
}

void CDlgMapEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAP_HSCROLL, m_HScroll);
	DDX_Control(pDX, IDC_MAP_VSCROLL, m_VScroll);
	DDX_Control(pDX, IDC_CHECK_MAP_PLANE2, m_CheckPlane2);
	DDX_Control(pDX, IDC_CHECK_MAP_PLANE0, m_CheckPlane0);
	DDX_Control(pDX, IDC_CHECK_MAP_DET, m_CheckDet);
	DDX_Control(pDX, IDC_MAP_STOCK_SCROLL, m_VScrollStock);
	DDX_Control(pDX, IDC_MAP_STOCK_HSCROLL, m_HScrollStock);
	DDX_Control(pDX, IDC_STATIC_MAP_CUR_COORD, m_StaticCurCoord);
	DDX_Control(pDX, IDC_EDIT_MAP_CUR_GRID0, m_EditCurGrid0);
	DDX_Control(pDX, IDC_EDIT_MAP_CUR_GRID1, m_EditCurGrid1);
	DDX_Control(pDX, IDC_EDIT_MAP_CUR_GRID2, m_EditCurGrid2);
	DDX_Control(pDX, IDC_COMBO_BGM, m_ComboBgm);
	DDX_Control(pDX, IDC_COMBO_BOSS, m_ComboBoss);
	DDX_Control(pDX, IDC_CHECK_MAP_ANI, m_CheckMapAni);
}


BEGIN_MESSAGE_MAP(CDlgMapEdit, CDialog)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_CHECK_MAP_PLANE2, &CDlgMapEdit::OnBnClickedCheckMapPlane2)
	ON_BN_CLICKED(IDC_CHECK_MAP_PLANE0, &CDlgMapEdit::OnBnClickedCheckMapPlane0)
	ON_BN_CLICKED(IDC_CHECK_MAP_DET, &CDlgMapEdit::OnBnClickedCheckMapDet)

	ON_WM_SETCURSOR()
	ON_CBN_SELCHANGE(IDC_COMBO_BGM, &CDlgMapEdit::OnCbnSelchangeComboBgm)
	ON_CBN_SELCHANGE(IDC_COMBO_BOSS, &CDlgMapEdit::OnCbnSelchangeComboBoss)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_MAP_ANI, &CDlgMapEdit::OnBnClickedCheckMapAni)
END_MESSAGE_MAP()


void CDlgMapEdit::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  int minpos;
   int maxpos;
   pScrollBar->GetScrollRange(&minpos, &maxpos); 
   maxpos = pScrollBar->GetScrollLimit();
   int curpos = pScrollBar->GetScrollPos();
   switch (nSBCode)
   {
   case SB_LEFT:curpos = minpos;break;
   case SB_RIGHT:curpos = maxpos;break;
   case SB_LINELEFT:
      if (curpos > minpos)
         curpos--;
      break;
   case SB_LINERIGHT:
      if (curpos < maxpos)
         curpos++;
      break;
   case SB_PAGELEFT:
   {
      SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);
      if (curpos > minpos)
      curpos = max(minpos, curpos - (int) info.nPage);
   }
      break;
   case SB_PAGERIGHT:
   {
      SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);
      if (curpos < maxpos)
         curpos = min(maxpos, curpos + (int) info.nPage);
   }
      break;
   case SB_THUMBPOSITION: 
      curpos = nPos;     
      break;
   case SB_THUMBTRACK:   
      curpos = nPos;     
      break;
   }
   pScrollBar->SetScrollPos(curpos);


	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	if(pScrollBar==&m_HScroll)GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
	else if(pScrollBar==&m_HScrollStock)GetDlgItem(IDC_STATIC_STOCK)->RedrawWindow();
}

void CDlgMapEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int minpos;
   int maxpos;
   pScrollBar->GetScrollRange(&minpos, &maxpos); 
   maxpos = pScrollBar->GetScrollLimit();
   int curpos = pScrollBar->GetScrollPos();
   switch (nSBCode)
   {
   case SB_TOP:      
      curpos = minpos;
      break;
   case SB_BOTTOM:    
      curpos = maxpos;
      break;
   case SB_LINEUP:     
      if (curpos > minpos)
         curpos--;
      break;
   case SB_LINEDOWN:
      if (curpos < maxpos)
         curpos++;
      break;
   case SB_PAGEUP: 
   {
      SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);
      if (curpos > minpos)
      curpos = max(minpos, curpos - (int) info.nPage);
   }
      break;
   case SB_PAGEDOWN: 
   {
      SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);

      if (curpos < maxpos)
         curpos = min(maxpos, curpos + (int) info.nPage);
   }
      break;
   case SB_THUMBPOSITION: 
      curpos = nPos;
      break;
   case SB_THUMBTRACK:  
      curpos = nPos;
      break;
   }
   pScrollBar->SetScrollPos(curpos);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
	if(pScrollBar==&m_VScroll)GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
	else if(pScrollBar==&m_VScrollStock)GetDlgItem(IDC_STATIC_STOCK)->RedrawWindow();
}

void CDlgMapEdit::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	
	RECT rc;
	if(nIDCtl==IDC_STATIC_MAP)
	{
		GetDlgItem(IDC_STATIC_MAP)->GetClientRect(&rc);
		m_CanMap.Present(lpDrawItemStruct->hDC,0,0,rc.right,rc.bottom,
			m_HScroll.GetScrollPos(),m_VScroll.GetScrollPos());

		if(cur_grid_x!=0xFF)
		{
			CPen RedPen(PS_SOLID,1,RGB(255,0,0));
			HGDIOBJ hOldPen=SelectObject(lpDrawItemStruct->hDC,RedPen.GetSafeHandle());
			SelectObject(lpDrawItemStruct->hDC,GetStockObject(NULL_BRUSH));
			Rectangle(lpDrawItemStruct->hDC,
				(cur_grid_x<<4)-m_HScroll.GetScrollPos(),
				(cur_grid_y<<4)-m_VScroll.GetScrollPos(),
				(cur_grid_x<<4)+16-m_HScroll.GetScrollPos(),
				(cur_grid_y<<4)+16-m_VScroll.GetScrollPos());
			SelectObject(lpDrawItemStruct->hDC,hOldPen);
		}

	}
	else if(nIDCtl==IDC_STATIC_STOCK)
	{
		GetDlgItem(IDC_STATIC_STOCK)->GetClientRect(&rc);
		m_CanStock.Present(lpDrawItemStruct->hDC,0,0,rc.right,rc.bottom,
			m_HScrollStock.GetScrollPos(),m_VScrollStock.GetScrollPos());

		if(cur_stock_x!=0xFF)
		{
			CPen RedPen(PS_SOLID,1,RGB(255,0,0));
			HGDIOBJ hOldPen=SelectObject(lpDrawItemStruct->hDC,RedPen.GetSafeHandle());
			SelectObject(lpDrawItemStruct->hDC,GetStockObject(NULL_BRUSH));
			Rectangle(lpDrawItemStruct->hDC,
				(cur_stock_x<<4)-m_HScrollStock.GetScrollPos(),
				(cur_stock_y<<4)-m_VScrollStock.GetScrollPos(),
				(cur_stock_x<<4)+16-m_HScrollStock.GetScrollPos(),
				(cur_stock_y<<4)+16-m_VScrollStock.GetScrollPos());
			SelectObject(lpDrawItemStruct->hDC,hOldPen);
		}
		if(cursel_stock_x!=0xFF)
		{
			CPen RedPen(PS_SOLID,2,RGB(255,0,0));
			HGDIOBJ hOldPen=SelectObject(lpDrawItemStruct->hDC,RedPen.GetSafeHandle());
			SelectObject(lpDrawItemStruct->hDC,GetStockObject(NULL_BRUSH));
			Rectangle(lpDrawItemStruct->hDC,
				(cursel_stock_x<<4)-m_HScrollStock.GetScrollPos(),
				(cursel_stock_y<<4)-m_VScrollStock.GetScrollPos(),
				(cursel_stock_x<<4)+16-m_HScrollStock.GetScrollPos(),
				(cursel_stock_y<<4)+16-m_VScrollStock.GetScrollPos());
			SelectObject(lpDrawItemStruct->hDC,hOldPen);
		}
	}
	else if(nIDCtl==IDC_STATIC_MAP_CUR_GRID)
	{
		m_CanCurGrid.Present(lpDrawItemStruct->hDC,0,0,32,48,0,0);
	}

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
void CDlgMapEdit::PaintStock()
{
	m_CanStock.Clear(0);
	int CheckPlane0=m_CheckPlane0.GetCheck();
	if(m_CheckDet.GetCheck())
	{
		
		int x=0,y=0;
		if(CheckPlane0) for(int i=0;DetDesc0[i].code!=0xFFFFFFFF;++i)
		{
			if(DetDesc0[i].code==0xFFFFFFFE)
			{
				++y;x=0;
			}
			else
			{
				for(u8 bx=0;bx<16;++bx)for(u8 by=0;by<16;++by)
				{
					m_CanStock.SetPixel((x<<4)|bx,(y<<4)|by,
						Det0Bmp[i].Get(bx,by)?RGB(0,128,0):RGB(255,255,255));
				}
				++x;
			}
		}
		else for(int i=0;Det1List[i]!=0xFFFFFFFF;++i)
		{
			if(Det1List[i]==0xFFFFFFFE)
			{
				++y;x=0;
			}
			else
			{
				for(u8 bx=0;bx<16;++bx)for(u8 by=0;by<16;++by)
				{
					m_CanStock.SetPixel((x<<4)|bx,(y<<4)|by,
						Det1Bmp[i].data[bx][by]);
				}
				++x;
				//if(x==32)x=0,++y;
			}
		}
	}
	else
	{
		Nitro::CharData chardata;
		COLORREF clr;
		u8 plti;
		int dx,dy;
		u16 BlockMappingCount=CheckPlane0?
			m_Ma.GetBlockMappingACount():m_Ma.GetBlockMappingBCount();
		SqB &sqb=CheckPlane0?m_Fb:m_Bb;
		for(u16 i=0;i<BlockMappingCount;++i)
		{
			for(u8 b=0;b<4;++b)
			{
				if(CheckPlane0)chardata=m_Ma.BlockMappingA(i).mapping[b];
				else chardata=m_Ma.BlockMappingB(i).mapping[b];
				for(u8 bx=0;bx<8;++bx)for(u8 by=0;by<8;++by)
				{
					if(CheckPlane0)
					{
						dx=((i&31)<<4)|bx|((b&1)<<3);
						dy=((i>>5)<<4)|by|((b&2)<<2);
					}
					else
					{
						dx=((i&63)<<4)|bx|((b&1)<<3);
						dy=((i>>6)<<4)|by|((b&2)<<2);
					}

					plti=sqb.Tile(chardata.tile).Get(chardata.flipx?7-bx:bx,chardata.flipy?7-by:by);
					if(plti)
						clr=Nitro::Color15to24(sqb.Pal[plti|(sqb.PalLine(chardata.tile)<<4)]);
					else clr=RGB(255,255,255);
					m_CanStock.SetPixel(dx,dy,clr);
				}
				
			}
		}
	}
}
inline COLORREF ColorToGrey(COLORREF color)
{
	BYTE gr=(GetRValue(color)+GetGValue(color)+GetBValue(color))/3;
	gr=(gr+765)>>2;
	return RGB(gr,gr,gr);
}
void CDlgMapEdit::PaintMap(u8 onlyx,u8 onlyy)
{

	Nitro::CharData chardata[3];
	COLORREF clr[3];
	u8 plti[3];
	int dx,dy;

	int CheckPlane2=m_CheckPlane2.GetCheck();
	int CheckPlane0=m_CheckPlane0.GetCheck();
	int CheckDet=m_CheckDet.GetCheck();
	BMP1b16x16* detbmp;
	BMP3216x16* det1bmp,*det2bmp;
	COLORREF detc;
	u8 x0,x1,y0,y1;
	if(onlyx==0xFF)x0=0,x1=m_Ma.GetW();else x0=onlyx,x1=onlyx+1;
	if(onlyy==0xFF)y0=0,y1=m_Ma.GetH();else y0=onlyy,y1=onlyy+1;
	for(u8 y=y0;y<y1;++y)for(u8 x=x0;x<x1;++x)
	{
		GetDet0Desc(m_Ma.Grid(x,y).det[0],&detbmp);
		det1bmp=GetDet1Bmp(m_Ma.Grid(x,y).det[1]);
		det2bmp=GetDet1Bmp(m_Ma.Grid(x,y).det[2]);
		for(u8 b=0;b<4;++b)
		{
			chardata[1]=m_Ma.BlockMappingB(m_Ma.Grid(x,y).gra[1],true).mapping[b];
			chardata[2]=m_Ma.BlockMappingB(m_Ma.Grid(x,y).gra[2],true).mapping[b];
			chardata[0]=m_Ma.BlockMappingA(m_Ma.Grid(x,y).gra[0],true).mapping[b];

			for(u8 bx=0;bx<8;++bx)for(u8 by=0;by<8;++by)
			{
				dx=bx|((b&1)<<3)|(x<<4);
				dy=by|((b&2)<<2)|(y<<4);
				plti[0]=m_Fb.Tile(chardata[0].tile     ).Get(chardata[0].flipx?7-bx:bx,chardata[0].flipy?7-by:by);
				plti[1]=m_Bb.Tile(chardata[1].tile,true).Get(chardata[1].flipx?7-bx:bx,chardata[1].flipy?7-by:by);
				plti[2]=m_Bb.Tile(chardata[2].tile,true).Get(chardata[2].flipx?7-bx:bx,chardata[2].flipy?7-by:by);
				clr[0]=Nitro::Color15to24(m_Fb.Pal[plti[0]|(m_Fb.PalLine(chardata[0].tile)<<4)]);
				if(plti[1])
					clr[1]=Nitro::Color15to24(m_Bb.Pal_Ani(m_Bb.PalLine(chardata[1].tile,true),plti[1]));
				else clr[1]=RGB(255,255,255);
				clr[2]=Nitro::Color15to24(m_Bb.Pal_Ani(m_Bb.PalLine(chardata[2].tile,true),plti[2]));
				if(CheckPlane0)
				{
					if(CheckPlane2)
					{
						m_CanMap.SetPixel(dx,dy,ColorToGrey(clr[1]));
						if(plti[2])m_CanMap.SetPixel(dx,dy,ColorToGrey(clr[2]));
						detc=det2bmp->data[bx|((b&1)<<3)][by|((b&2)<<2)];
					}
					else
					{
						m_CanMap.SetPixel(dx,dy,ColorToGrey(clr[1]));
						detc=det1bmp->data[bx|((b&1)<<3)][by|((b&2)<<2)];
					}
					if(CheckDet)
					{
						if(plti[0])m_CanMap.SetPixel(dx,dy,ColorToGrey(clr[0]));
						if(detc!=RGB(255,255,255))m_CanMap.SetPixel(dx,dy,ColorToGrey(detc));
						if(detbmp->Get(bx|((b&1)<<3),by|((b&2)<<2)))
							m_CanMap.SetPixel(dx,dy,RGB(0,128,0));
					}else if(plti[0])m_CanMap.SetPixel(dx,dy,clr[0]);
				}
				else
				{
					if(CheckPlane2)
					{
						m_CanMap.SetPixel(dx,dy,ColorToGrey(clr[1]));
						if(CheckDet)
						{
							if(plti[2])m_CanMap.SetPixel(dx,dy,ColorToGrey(clr[2]));
							detc=det2bmp->data[bx|((b&1)<<3)][by|((b&2)<<2)];
							if(detc!=RGB(255,255,255))m_CanMap.SetPixel(dx,dy,detc);
						}
						else
							if(plti[2])m_CanMap.SetPixel(dx,dy,clr[2]);
					}
					else
					{
						if(CheckDet)
						{
							COLORREF detc;
							m_CanMap.SetPixel(dx,dy,ColorToGrey(clr[1]));
							detc=det1bmp->data[bx|((b&1)<<3)][by|((b&2)<<2)];
							if(detc!=RGB(255,255,255))m_CanMap.SetPixel(dx,dy,detc);

						}
						else m_CanMap.SetPixel(dx,dy,clr[1]);
					}
				}
			}
		}
	}
	
}
BOOL CDlgMapEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CanMap.Create(m_Ma.GetW()*16,m_Ma.GetH()*16);
	m_CanStock.Create(1024,max(max(
		m_Ma.GetBlockMappingBCount()>>6,
		m_Ma.GetBlockMappingACount()>>5),12)<<4);
	m_CanCurGrid.Create(32,48);

	cur_grid_x=cur_grid_y=0xFF;
	cur_stock_x=cur_stock_y=0xFF;
	cursel_stock_x=cursel_stock_y=0xFF;
	cursel_stock=0xFFFFFFFF;
	grid_drawing=false;
	
	m_CheckPlane0.SetCheck(GetPrivateProfileInt(_T("MapEditor"),_T("Plane0"),0,ProfilePath)?TRUE:FALSE);
	m_CheckPlane2.SetCheck(GetPrivateProfileInt(_T("MapEditor"),_T("Plane2"),0,ProfilePath)?TRUE:FALSE);
	m_CheckDet.SetCheck(GetPrivateProfileInt(_T("MapEditor"),_T("Det"),0,ProfilePath)?TRUE:FALSE);
	m_CheckMapAni.SetCheck(GetPrivateProfileInt(_T("MapEditor"),_T("MapAni"),0,ProfilePath)?TRUE:FALSE);

	CString str;
	for(int i=0;i<256;++i)
	{
		str.Format(FORMAT_BGM,i,GetBgmName((u8)i));
		m_ComboBgm.AddString(str);
		if(m_Ma.MapAttribute.Bgm==(u8)i)m_ComboBgm.SelectString(-1,str);
	}
	for(u8 i=0;i<10;++i)
	{
		str.Format(FORMAT_BOSS,i);
		m_ComboBoss.AddString(str);
		if(m_Ma.MapAttribute.Boss==i)m_ComboBoss.SelectString(-1,str);
	}

	SetScroll();
	SetStockScroll();

	PaintMap();
	PaintStock();

	OnStockLButtonDown(0,0);


	memcpy(&m_Bb.TileTimeDelta,&m_Ma.S12HScript.TileTimeDelta,4);
	OnBnClickedCheckMapAni();
	
	return TRUE;
}
void CDlgMapEdit::SetScroll()
{
	RECT rc;
	int hsrg,vsrg;
	GetDlgItem(IDC_STATIC_MAP)->GetClientRect(&rc);
	hsrg=m_Ma.GetW()*16-rc.right;
	if(hsrg<1)hsrg=1;
	vsrg=m_Ma.GetH()*16-rc.bottom;
	if(vsrg<1)vsrg=1;
	SCROLLINFO scrinfo={0};
	scrinfo.cbSize=sizeof(SCROLLINFO);
	scrinfo.fMask=SIF_PAGE|SIF_RANGE|SIF_POS;
	scrinfo.nPage=rc.right;
	scrinfo.nMin=0;scrinfo.nMax=hsrg-1+scrinfo.nPage-1;
	scrinfo.nPos=0;
	m_HScroll.SetScrollInfo(&scrinfo);
	scrinfo.nPage=rc.bottom;
	scrinfo.nMin=0;scrinfo.nMax=vsrg-1+scrinfo.nPage-1;
	scrinfo.nPos=0;
	m_VScroll.SetScrollInfo(&scrinfo);
}
void CDlgMapEdit::SetStockScroll()
{
	u32 W,H;
	if(m_CheckPlane0.GetCheck())
	{
		W=512;
		H=(m_Ma.GetBlockMappingACount()>>5)<<4;
	}
	else
	{
		if(m_CheckDet.GetCheck())
		{
			W=512;
			H=12*16;
			
		}
		else
		{
			W=1024;
			H=(m_Ma.GetBlockMappingBCount()>>6)<<4;
		}
	}
	RECT rc;
	int hsrg,vsrg;
	GetDlgItem(IDC_STATIC_STOCK)->GetClientRect(&rc);
	hsrg=W-rc.right;
	if(hsrg<1)hsrg=1;
	vsrg=H-rc.bottom;
	if(vsrg<1)vsrg=1;
	SCROLLINFO scrinfo={0};
	scrinfo.cbSize=sizeof(SCROLLINFO);
	scrinfo.fMask=SIF_PAGE|SIF_RANGE|SIF_POS;
	scrinfo.nPage=rc.right;
	scrinfo.nMin=0;scrinfo.nMax=hsrg-1+scrinfo.nPage-1;
	scrinfo.nPos=0;
	m_HScrollStock.SetScrollInfo(&scrinfo);
	scrinfo.nPage=rc.bottom;
	scrinfo.nMin=0;scrinfo.nMax=vsrg-1+scrinfo.nPage-1;
	scrinfo.nPos=0;
	m_VScrollStock.SetScrollInfo(&scrinfo);
}
void CDlgMapEdit::OnBnClickedCheckMapPlane2()
{
	WritePrivateProfileString(_T("MapEditor"),_T("Plane2"),m_CheckPlane2.GetCheck()?
		_T("1"):_T("0"),ProfilePath);
	PaintMap();
	GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
}

void CDlgMapEdit::OnBnClickedCheckMapPlane0()
{
	WritePrivateProfileString(_T("MapEditor"),_T("Plane0"),m_CheckPlane0.GetCheck()?
		_T("1"):_T("0"),ProfilePath);
	SetStockScroll();
	OnStockLButtonDown(0,0);
	PaintMap();
	PaintStock();
	GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
	GetDlgItem(IDC_STATIC_STOCK)->RedrawWindow();
}

void CDlgMapEdit::OnBnClickedCheckMapDet()
{
	WritePrivateProfileString(_T("MapEditor"),_T("Det"),m_CheckDet.GetCheck()?
		_T("1"):_T("0"),ProfilePath);
	SetStockScroll();
	OnStockLButtonDown(0,0);
	PaintMap();
	PaintStock();
	GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
	GetDlgItem(IDC_STATIC_STOCK)->RedrawWindow();
}

void CDlgMapEdit::OnMapLButtonDown(u8 x,u8 y)
{
	grid_drawing=true;
	OnMapMouseMove(x,y);
}
void CDlgMapEdit::OnMapLButtonUp(u8 x,u8 y)
{
	grid_drawing=false;

}
void CDlgMapEdit::OnMapDraw(u8 x,u8 y)
{
	if(m_CheckDet.GetCheck())
		if(m_CheckPlane0.GetCheck())m_Ma.Grid(x,y).det[0]=cursel_stock;
		else
			if(m_CheckPlane2.GetCheck())m_Ma.Grid(x,y).det[2]=cursel_stock;
			else m_Ma.Grid(x,y).det[1]=cursel_stock;
	else
		if(m_CheckPlane0.GetCheck())m_Ma.Grid(x,y).gra[0]=(u16)cursel_stock;
		else
			if(m_CheckPlane2.GetCheck())m_Ma.Grid(x,y).gra[2]=(u16)cursel_stock;
			else m_Ma.Grid(x,y).gra[1]=(u16)cursel_stock;
	PaintMap(x,y);
}
void CDlgMapEdit::OnMapMouseMove(u8 x,u8 y)
{
	if(x>=m_Ma.GetW() || y>=m_Ma.GetH())
	{
		grid_drawing=false;
		m_StaticCurCoord.SetWindowText(_T(""));
		m_EditCurGrid0.SetWindowText(_T(""));
		m_EditCurGrid1.SetWindowText(_T(""));
		m_EditCurGrid2.SetWindowText(_T(""));
		m_CanCurGrid.Clear(RGB(255,255,255));
		GetDlgItem(IDC_STATIC_MAP_CUR_GRID)->RedrawWindow();
		cur_grid_x=0xFF;
		cur_grid_y=0xFF;
		GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
		return;
	}
	if(grid_drawing)
	{
		OnMapDraw(x,y);
	}

	CString str;
	str.Format(_T("(%d,%d)"),x,y);
	m_StaticCurCoord.SetWindowText(str);

	str.Format(_T("%04X|%08X"),m_Ma.Grid(x,y).gra[0],m_Ma.Grid(x,y).det[0]);
	m_EditCurGrid0.SetWindowText(str);
	str.Format(_T("%04X|%08X"),m_Ma.Grid(x,y).gra[1],m_Ma.Grid(x,y).det[1]);
	m_EditCurGrid1.SetWindowText(str);
	str.Format(_T("%04X|%08X"),m_Ma.Grid(x,y).gra[2],m_Ma.Grid(x,y).det[2]);
	m_EditCurGrid2.SetWindowText(str);

	//Paint the grid data
	Nitro::CharData chardata[3];
	COLORREF clr[3];
	u8 plti[3];
	int dx,dy;
	BMP1b16x16* detbmp;
	BMP3216x16* det1bmp,*det2bmp;
	GetDet0Desc(m_Ma.Grid(x,y).det[0],&detbmp);
	det1bmp=GetDet1Bmp(m_Ma.Grid(x,y).det[1]);
	det2bmp=GetDet1Bmp(m_Ma.Grid(x,y).det[2]);
	for(u8 b=0;b<4;++b)
	{
		chardata[1]=m_Ma.BlockMappingB(m_Ma.Grid(x,y).gra[1]).mapping[b];
		chardata[2]=m_Ma.BlockMappingB(m_Ma.Grid(x,y).gra[2]).mapping[b];
		chardata[0]=m_Ma.BlockMappingA(m_Ma.Grid(x,y).gra[0]).mapping[b];
		for(u8 bx=0;bx<8;++bx)for(u8 by=0;by<8;++by)
		{
			dx=bx|((b&1)<<3);
			dy=by|((b&2)<<2);
			plti[0]=m_Fb.Tile(chardata[0].tile).Get(chardata[0].flipx?7-bx:bx,chardata[0].flipy?7-by:by);
			plti[1]=m_Bb.Tile(chardata[1].tile).Get(chardata[1].flipx?7-bx:bx,chardata[1].flipy?7-by:by);
			plti[2]=m_Bb.Tile(chardata[2].tile).Get(chardata[2].flipx?7-bx:bx,chardata[2].flipy?7-by:by);
			if(plti[0])
				clr[0]=Nitro::Color15to24(m_Fb.Pal[plti[0]|(m_Fb.PalLine(chardata[0].tile)<<4)]);
			else clr[0]=RGB(255,255,255);
			if(plti[1])
				clr[1]=Nitro::Color15to24(m_Bb.Pal[plti[1]|(m_Bb.PalLine(chardata[1].tile)<<4)]);
			else clr[1]=RGB(255,255,255);
			if(plti[2]) 
				clr[2]=Nitro::Color15to24(m_Bb.Pal[plti[2]|(m_Bb.PalLine(chardata[2].tile)<<4)]);
			else clr[2]=RGB(255,255,255);
			
			m_CanCurGrid.SetPixel(dx,dy,clr[0]);
			m_CanCurGrid.SetPixel(dx,dy+16,clr[1]);
			m_CanCurGrid.SetPixel(dx,dy+32,clr[2]);
			m_CanCurGrid.SetPixel(dx+16,dy,detbmp->Get(dx,dy)?RGB(0,128,0):RGB(255,255,255));
			m_CanCurGrid.SetPixel(dx+16,dy+16,det1bmp->data[dx][dy]);
			m_CanCurGrid.SetPixel(dx+16,dy+32,det2bmp->data[dx][dy]);
		}
	}
	GetDlgItem(IDC_STATIC_MAP_CUR_GRID)->RedrawWindow();

	cur_grid_x=x;
	cur_grid_y=y;
	GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
}
void CDlgMapEdit::OnStockMouseMove(u8 x,u8 y)
{

	CString str;
	if(m_CheckDet.GetCheck())
	{
		if(m_CheckPlane0.GetCheck())
		{
			const DetDesc* ddesc=GetDet0Desc(x,y);
			if(ddesc)
			{
				cur_stock_x=x;
				cur_stock_y=y;
				str=ddesc->name;
			}
			else
			{
				cur_stock_x=cur_stock_y=0xFF;
				str=_T("");
			}
		}
		else
		{
			u32 det1code=GetDet1CodeByStockCoord(x,y);
			if(det1code!=0xFFFFFFFF)
			{
				cur_stock_x=x;
				cur_stock_y=y;
				GetDet1Desc(det1code,str);
			}
			else
			{
				cur_stock_x=cur_stock_y=0xFF;
				str=_T("");
			}
		}
	}
	else
	{
		if(m_CheckPlane0.GetCheck())
		{
			if(x>=32 || y>m_Ma.GetBlockMappingACount()>>5)
			{
				cur_stock_x=cur_stock_y=0xFF;
				str=_T("");
			}
			else
			{
				cur_stock_x=x;
				cur_stock_y=y;
				str.Format(_T("%02X"),x+y*32);
			}
		}
		else
		{
			if(x>=64 || y>m_Ma.GetBlockMappingBCount()>>6)
			{
				cur_stock_x=cur_stock_y=0xFF;
				str=_T("");
			}
			else
			{
				cur_stock_x=x;
				cur_stock_y=y;
				str.Format(_T("%02X"),x+y*64);
			}
		}
	}
	m_StaticCurCoord.SetWindowText(str);
	GetDlgItem(IDC_STATIC_STOCK)->RedrawWindow();
}
void CDlgMapEdit::OnStockLButtonDown(u8 x,u8 y)
{
	if(m_CheckDet.GetCheck())
	{
		if(m_CheckPlane0.GetCheck())
		{
			const DetDesc* ddesc=GetDet0Desc(x,y);
			if(ddesc)
			{
				cursel_stock_x=x;
				cursel_stock_y=y;
				cursel_stock=ddesc->code;
			}
			else return;
		}
		else
		{
			u32 det1code=GetDet1CodeByStockCoord(x,y);
			if(det1code!=0xFFFFFFFF)
			{
				cursel_stock_x=x;
				cursel_stock_y=y;
				cursel_stock=det1code;
			}
			else return;
		}
	}
	else
	{
		if(m_CheckPlane0.GetCheck())
		{
			if(x>=32|| y>m_Ma.GetBlockMappingACount()>>5) return;
			else
			{
				cursel_stock_x=x;
				cursel_stock_y=y;
				cursel_stock=x+y*32;
			}
		}
		else
		{
			if(x>=64|| y>m_Ma.GetBlockMappingBCount()>>6) return;
			else
			{
				cursel_stock_x=x;
				cursel_stock_y=y;
				cursel_stock=x+y*64;
			}
		}
	}
	GetDlgItem(IDC_STATIC_STOCK)->RedrawWindow();
}
BOOL CDlgMapEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{

	LONG Id=GetWindowLong(pWnd->GetSafeHwnd(),GWL_ID);
	CPoint pt;
	CRect rc;
	GetCursorPos(&pt);
	pWnd->GetWindowRect(&rc);
	pt-=rc.TopLeft();

	if(message==WM_LBUTTONUP)
	{
		OnMapLButtonUp(0xFF,0xFF);
	}
	else if(Id==IDC_STATIC_MAP)
	{
		pt.x+=m_HScroll.GetScrollPos();
		pt.y+=m_VScroll.GetScrollPos();
		switch(message)
		{
		case WM_MOUSEMOVE:OnMapMouseMove((u8)(pt.x>>4),(u8)(pt.y>>4));break;
		case WM_LBUTTONDOWN:OnMapLButtonDown((u8)(pt.x>>4),(u8)(pt.y>>4));break;
		//case WM_LBUTTONUP:OnMapLButtonUp((u8)(pt.x>>4),(u8)(pt.y>>4));break;
		}

	}
	else if(Id==IDC_STATIC_STOCK)
	{
		pt.x+=m_HScrollStock.GetScrollPos();
		pt.y+=m_VScrollStock.GetScrollPos();
		switch(message)
		{
		case WM_MOUSEMOVE:OnStockMouseMove((u8)(pt.x>>4),(u8)(pt.y>>4));break;
		case WM_LBUTTONDOWN:OnStockLButtonDown((u8)(pt.x>>4),(u8)(pt.y>>4));break;
		//case WM_LBUTTONUP:OnStockLButtonUp((u8)(pt.x>>4),(u8)(pt.y>>4));break;
		}
	}
	else
	{
		OnMapMouseMove(0xFF,0xFF);
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CDlgMapEdit::OnCbnSelchangeComboBgm()
{
	CString str;
	m_ComboBgm.GetWindowText(str);
	int bgm;
	TCHAR dummybuf[100];
	_stscanf(str,FORMAT_BGM,&bgm,dummybuf);
	m_Ma.MapAttribute.Bgm=(u8)bgm;
}

void CDlgMapEdit::OnCbnSelchangeComboBoss()
{
	CString str;
	m_ComboBoss.GetWindowText(str);
	int bgm;
	TCHAR dummybuf[100];
	_stscanf(str,FORMAT_BOSS,&bgm,dummybuf);
	m_Ma.MapAttribute.Boss=(u8)bgm;
}

void CDlgMapEdit::OnTimer(UINT_PTR nIDEvent)
{
	bool needredraw=false;
	if(nIDEvent==ID_TIMER_REDRAW_MAP)
	{
		needredraw=m_Ma.TicketIn()||needredraw;
		needredraw=m_Bb.TicketIn()||needredraw;
		if(needredraw)
		{
			PaintMap();
			GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgMapEdit::OnBnClickedCheckMapAni()
{
	WritePrivateProfileString(_T("MapEditor"),_T("MapAni"),m_CheckMapAni.GetCheck()?
		_T("1"):_T("0"),ProfilePath);
	if(m_CheckMapAni.GetCheck())
	{
		SetTimer(ID_TIMER_REDRAW_MAP,20,0);
	}
	else
	{
		KillTimer(ID_TIMER_REDRAW_MAP);
		m_Ma.TicketClear();
		m_Bb.TicketClear();
		if(m_Bb.pSqPl1)m_Bb.pSqPl1->TickedClear();
		PaintMap();
		GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
	}
}
