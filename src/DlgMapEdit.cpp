
//

#include "stdafx.h"
#include "DlgMapEdit.h"
#include "Det.h"


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
}


BEGIN_MESSAGE_MAP(CDlgMapEdit, CDialog)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_CHECK_MAP_PLANE2, &CDlgMapEdit::OnBnClickedCheckMapPlane2)
	ON_BN_CLICKED(IDC_CHECK_MAP_PLANE0, &CDlgMapEdit::OnBnClickedCheckMapPlane0)
	ON_BN_CLICKED(IDC_CHECK_MAP_DET, &CDlgMapEdit::OnBnClickedCheckMapDet)
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
	GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
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
	GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
}

void CDlgMapEdit::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(nIDCtl==IDC_STATIC_MAP)
	{
		RECT rc;
		GetDlgItem(IDC_STATIC_MAP)->GetClientRect(&rc);
		m_CanMap.Present(lpDrawItemStruct->hDC,0,0,rc.right,rc.bottom,
			m_HScroll.GetScrollPos(),m_VScroll.GetScrollPos());
	}

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
inline COLORREF ColorToGrey(COLORREF color)
{
	BYTE gr=(GetRValue(color)+GetGValue(color)+GetBValue(color))/3;
	gr=(gr+765)>>2;
	return RGB(gr,gr,gr);
}
void CDlgMapEdit::PaintMap()
{

	u16 tilei[3];
	bool flipx[3],flipy[3];
	COLORREF clr[3];
	u8 plti[3];
	int dx,dy;

	int CheckPlane2=m_CheckPlane2.GetCheck();
	int CheckPlane0=m_CheckPlane0.GetCheck();
	int CheckDet=m_CheckDet.GetCheck();
	BMP1b16x16* detbmp;
	BMP3216x16* det1bmp,*det2bmp;
	for(u8 y=0;y<m_Ma.GetH();++y)for(u8 x=0;x<m_Ma.GetW();++x)
	{
		GetDet0Desc(m_Ma.Grid(x,y).det[0],&detbmp);
		det1bmp=GetDet1Bmp(m_Ma.Grid(x,y).det[1]);
		det2bmp=GetDet1Bmp(m_Ma.Grid(x,y).det[2]);
		for(u8 b=0;b<4;++b)
		{
			tilei[1]=m_Ma.BlockMappingB(m_Ma.Grid(x,y).gra[1]).mapping[b];
			tilei[2]=m_Ma.BlockMappingB(m_Ma.Grid(x,y).gra[2]).mapping[b];
			tilei[0]=m_Ma.BlockMappingA(m_Ma.Grid(x,y).gra[0]).mapping[b];
			flipx[0]=tilei[0]&1024 ? true:false;
			flipy[0]=tilei[0]&2048 ? true:false;
			tilei[0]=tilei[0]&1023;
			flipx[1]=tilei[1]&1024 ? true:false;
			flipy[1]=tilei[1]&2048 ? true:false;
			tilei[1]=tilei[1]&1023;
			flipx[2]=tilei[2]&1024 ? true:false;
			flipy[2]=tilei[2]&2048 ? true:false;
			tilei[2]=tilei[2]&1023;
			for(u8 bx=0;bx<8;++bx)for(u8 by=0;by<8;++by)
			{
				dx=bx|((b&1)<<3)|(x<<4);
				dy=by|((b&2)<<2)|(y<<4);
				plti[0]=m_Fb.Tile(tilei[0]).Get(flipx[0]?7-bx:bx,flipy[0]?7-by:by);
				plti[1]=m_Bb.Tile(tilei[1]).Get(flipx[1]?7-bx:bx,flipy[1]?7-by:by);
				plti[2]=m_Bb.Tile(tilei[2]).Get(flipx[2]?7-bx:bx,flipy[2]?7-by:by);
				clr[0]=Nitro::Color15to24(m_Fb.Pal[plti[0]|(m_Fb.PalLine(tilei[0])<<4)]);
				if(plti[1])
					clr[1]=Nitro::Color15to24(m_Bb.Pal[plti[1]|(m_Bb.PalLine(tilei[1])<<4)]);
				else clr[1]=RGB(255,255,255);
				clr[2]=Nitro::Color15to24(m_Bb.Pal[plti[2]|(m_Bb.PalLine(tilei[2])<<4)]);
				if(CheckPlane0)
				{
					if(CheckPlane2)
					{
						m_CanMap.SetPixel(dx,dy,ColorToGrey(clr[1]));
						if(plti[2])m_CanMap.SetPixel(dx,dy,ColorToGrey(clr[2]));
					}
					else
					{
						m_CanMap.SetPixel(dx,dy,ColorToGrey(clr[1]));
					}
					if(CheckDet)
					{
						if(plti[0])m_CanMap.SetPixel(dx,dy,ColorToGrey(clr[0]));
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
							COLORREF detc;
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

	SetScroll();

	PaintMap();

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
void CDlgMapEdit::OnBnClickedCheckMapPlane2()
{
	PaintMap();
	GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
}

void CDlgMapEdit::OnBnClickedCheckMapPlane0()
{
	PaintMap();
	GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
}

void CDlgMapEdit::OnBnClickedCheckMapDet()
{
	PaintMap();
	GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
}
