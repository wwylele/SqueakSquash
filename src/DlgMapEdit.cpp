


//Unicode

#include "stdafx.h"
#include "DlgMapEdit.h"
#include "Det.h"
#include "Bgm.h"
#include "Foe.h"


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
	DDX_Control(pDX, IDC_LIST_MAP_OBJ, m_ListObj);
	DDX_Control(pDX, IDC_EDIT_MAP_CUR_GRID_BOSS, m_EditCurGridBoss);
	DDX_Control(pDX, IDC_CHECK_HIDE_FOE, m_CheckHideFoe);
	DDX_Control(pDX, IDC_CHECK_HIDE_SUP, m_CheckHideSup);
	DDX_Control(pDX, IDC_CHECK_HIDE_DOOR, m_CheckHideDoor);
	DDX_Control(pDX, IDC_CHECK_HIDE_MCTRL, m_CheckHideMctrl);
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
	ON_BN_CLICKED(IDC_RADIO_MAP_PT_GRID, &CDlgMapEdit::OnBnClickedRadioMapPtGrid)
	ON_BN_CLICKED(IDC_RADIO_MAP_PT_OBJ, &CDlgMapEdit::OnBnClickedRadioMapPtObj)
	ON_BN_CLICKED(IDC_RADIO_MAP_PT_BOSS, &CDlgMapEdit::OnBnClickedRadioMapPtBoss)
	ON_BN_CLICKED(IDC_CHECK_HIDE_FOE, &CDlgMapEdit::OnBnClickedCheckHideObj)
	ON_BN_CLICKED(IDC_CHECK_HIDE_SUP, &CDlgMapEdit::OnBnClickedCheckHideObj)
	ON_BN_CLICKED(IDC_CHECK_HIDE_DOOR, &CDlgMapEdit::OnBnClickedCheckHideObj)
	ON_BN_CLICKED(IDC_CHECK_HIDE_MCTRL, &CDlgMapEdit::OnBnClickedCheckHideObj)
END_MESSAGE_MAP()


void CDlgMapEdit::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	OnScroll(nSBCode,nPos,pScrollBar);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	if(pScrollBar==&m_HScroll)
	{
		GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
		GetDlgItem(IDC_STATIC_MAP_CX)->RedrawWindow();
		GetDlgItem(IDC_STATIC_MAP_CY)->RedrawWindow();
	}
	else if(pScrollBar==&m_HScrollStock)GetDlgItem(IDC_STATIC_STOCK)->RedrawWindow();
}

void CDlgMapEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	OnScroll(nSBCode,nPos,pScrollBar);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
	if(pScrollBar==&m_VScroll)
	{
		GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
		GetDlgItem(IDC_STATIC_MAP_CX)->RedrawWindow();
		GetDlgItem(IDC_STATIC_MAP_CY)->RedrawWindow();
	}
	else if(pScrollBar==&m_VScrollStock)GetDlgItem(IDC_STATIC_STOCK)->RedrawWindow();
}

void CDlgMapEdit::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	RECT rc;
	if(nIDCtl==IDC_STATIC_MAP)
	{
		CString str;

		GetDlgItem(IDC_STATIC_MAP)->GetClientRect(&rc);
		m_CanMap.Present(lpDrawItemStruct->hDC,0,0,rc.right,rc.bottom,
			m_HScroll.GetScrollPos(),m_VScroll.GetScrollPos());

		CBrush NullBrush,*pOldBrush;
		CPoint pt;
		CPen* pOldPen;
		
		if(mode==MPT_OBJ)
		{
			static CPen PenFoe(PS_SOLID,2,RGB(192,0,0)),
				PenSup(PS_SOLID,2,RGB(0,0,192)),
				PenDoor(PS_SOLID,2,RGB(0,192,0)),
				PenEp(PS_SOLID,2,RGB(192,0,192)),
				PenCur(PS_SOLID,2,RGB(255,255,0));
			bool hitcur=false;
			
			
			NullBrush.CreateStockObject(NULL_BRUSH);
			pOldBrush=dc.SelectObject(&NullBrush);
			

			dc.SetBkMode(TRANSPARENT);

			//Foe
			pOldPen=dc.SelectObject(&PenFoe);
			if(!m_CheckHideFoe.GetCheck())for(u16 i=0;i<m_De.GetFoeCount();++i)
			{
				pt.x=m_De.Foe(i).x-7-m_HScroll.GetScrollPos();
				pt.y=m_De.Foe(i).y-7-m_VScroll.GetScrollPos();
				if(cur_obj==i && cur_objgroup==FOE)
				{
					dc.SelectObject(&PenCur);
					hitcur=true;
				}
				else if(hitcur)
				{
					dc.SelectObject(&PenFoe);
					hitcur=false;
				}
				dc.Ellipse(pt.x,pt.y,pt.x+14,pt.y+14);
				str.Format(_T("%d %s"),i,FoeDesc[m_De.Foe(i).class_id].name);
				dc.DrawText(str,CRect(pt.x+4,pt.y,0,0),DT_LEFT|DT_TOP|DT_NOCLIP);
			}
			
			//Sup
			dc.SelectObject(&PenSup);
			if(!m_CheckHideSup.GetCheck())for(u16 i=0;i<m_De.GetSupCount();++i)
			{
				pt.x=m_De.Sup(i).x-m_HScroll.GetScrollPos();
				pt.y=m_De.Sup(i).y-9-m_VScroll.GetScrollPos();
				if(cur_obj==i && cur_objgroup==SUP)
				{
					dc.SelectObject(&PenCur);
					hitcur=true;
				}
				else if(hitcur)
				{
					dc.SelectObject(&PenSup);
					hitcur=false;
				}
				dc.MoveTo(pt);
				pt.x-=8;
				pt.y+=13;
				dc.LineTo(pt);
				pt.x+=16;
				dc.LineTo(pt);
				pt.x-=8;
				pt.y-=13;
				dc.LineTo(pt);
				str.Format(_T("%d %s"),i,SupDesc[m_De.Sup(i).class_id].name);
				dc.DrawText(str,CRect(pt.x-2,pt.y+1,0,0),DT_LEFT|DT_TOP|DT_NOCLIP);
			}

			//Door
			dc.SelectObject(&PenDoor);
			if(!m_CheckHideDoor.GetCheck())for(u16 i=0;i<m_Ma.GetDoorCount();++i)
			{
				pt.x=m_Ma.Door(i).x-8-m_HScroll.GetScrollPos();
				pt.y=m_Ma.Door(i).y-8-m_VScroll.GetScrollPos();
				if(cur_obj==i && cur_objgroup==DOOR)
				{
					dc.SelectObject(&PenCur);
					hitcur=true;
				}
				else if(hitcur)
				{
					dc.SelectObject(&PenDoor);
					hitcur=false;
				}
				dc.Rectangle(pt.x,pt.y,pt.x+16,pt.y+16);
				str.Format(_T("%d"),i);
				dc.DrawText(str,CRect(pt.x+2,pt.y,0,0),DT_LEFT|DT_TOP|DT_NOCLIP);
			}
			//Mctrl
			dc.SelectObject(&PenEp);
			if(!m_CheckHideMctrl.GetCheck())for(u8 i=0;i<m_Mctrl.GetMctrlCount();++i)
			{
				pt.x=m_Mctrl[i].x-m_HScroll.GetScrollPos();
				pt.y=m_Mctrl[i].y-9-m_VScroll.GetScrollPos();
				if(cur_obj==i && cur_objgroup==MCTRL)
				{
					dc.SelectObject(&PenCur);
					hitcur=true;
				}
				else if(hitcur)
				{
					dc.SelectObject(&PenEp);
					hitcur=false;
				}
				dc.MoveTo(pt);
				pt.y+=9;
				pt.x-=9;
				dc.LineTo(pt);
				pt.y+=9;
				pt.x+=9;
				dc.LineTo(pt);
				pt.y-=9;
				pt.x+=9;
				dc.LineTo(pt);
				pt.y-=9;
				pt.x-=9;
				dc.LineTo(pt);
				str.Format(_T("%d  %s"),i,EvcDesc[m_Mctrl[i].class_id].Name);
				dc.DrawText(str,CRect(pt.x-3,pt.y+1,0,0),DT_LEFT|DT_TOP|DT_NOCLIP);
			}
			dc.SelectObject(pOldPen);
			dc.SelectObject(pOldBrush);
			NullBrush.DeleteObject();
		}
		if(mode==MPT_BOSS)
		{
			static CPen PenGuide(PS_SOLID,2,RGB(192,128,0));
			NullBrush.CreateStockObject(WHITE_BRUSH);
			pOldBrush=dc.SelectObject(&NullBrush);
			dc.SetBkMode(TRANSPARENT);
			pOldPen=dc.SelectObject(&PenGuide);
			for(u16 i=0;i<m_Ma.GetGuideCount();++i)
			{
				pt.x=m_Ma.Guide(i).x-9-m_HScroll.GetScrollPos();
				pt.y=m_Ma.Guide(i).y-9-m_VScroll.GetScrollPos();
				dc.Ellipse(pt.x,pt.y,pt.x+18,pt.y+18);
				str.Format(_T("%d"),m_Ma.Guide(i).id);
				dc.DrawText(str,CRect(pt.x+3,pt.y+1,0,0),DT_LEFT|DT_TOP|DT_NOCLIP);
			}
			dc.SelectObject(pOldPen);
			dc.SelectObject(pOldBrush);
			NullBrush.DeleteObject();
		}
		

		if((mode==MPT_GRID || mode==MPT_BOSS) && cur_grid_x!=0xFF)
		{
			CPen RedPen(PS_DOT,1,RGB(255,0,0));
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
			CPen RedPen(PS_DOT,1,RGB(255,0,0));
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
	else if(nIDCtl==IDC_STATIC_MAP_CX)
	{
		static CBrush bbck(dc.GetBkColor());
		CString str;
		GetDlgItem(IDC_STATIC_MAP_CX)->GetClientRect(&rc);
		int begin,end;
		begin=m_HScroll.GetScrollPos()>>4;
		end=begin+((rc.right-rc.left)>>4)+1;
		for(int i=begin;i<=end;++i)
		{
			str.Format(_T("%d"),i);
			rc.left=(i<<4)-m_HScroll.GetScrollPos();
			rc.top=0;
			rc.bottom=16;
			rc.right=rc.left+16;
			dc.FillRect(&rc,&bbck);
			dc.DrawText(str,-1,&rc,DT_LEFT|DT_TOP|DT_NOCLIP);
		}
	}
	else if(nIDCtl==IDC_STATIC_MAP_CY)
	{
		static CBrush bbck(dc.GetBkColor());
		CString str;
		GetDlgItem(IDC_STATIC_MAP_CY)->GetClientRect(&rc);
		int begin,end;
		begin=m_VScroll.GetScrollPos()>>4;
		end=begin+((rc.bottom-rc.top)>>4)+1;
		for(int i=begin;i<=end;++i)
		{
			str.Format(_T("%d"),i);
			rc.left=0;
			rc.top=(i<<4)-m_VScroll.GetScrollPos();
			rc.bottom=rc.top+16;
			rc.right=16;
			dc.FillRect(&rc,&bbck);
			dc.DrawText(str,-1,&rc,DT_LEFT|DT_TOP|DT_NOCLIP);
		}
	}


	dc.Detach();
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
		u16 TexMappingCount=CheckPlane0?
			m_Ma.GetTexMappingCount(TEXM_F):m_Ma.GetTexMappingCount(TEXM_B);
		SqB &sqb=CheckPlane0?m_Fb:m_Bb;
		for(u16 i=0;i<TexMappingCount;++i)
		{
			for(u8 b=0;b<4;++b)
			{
				if(CheckPlane0)chardata=m_Ma.TexMapping(TEXM_F,i).mapping[b];
				else chardata=m_Ma.TexMapping(TEXM_B,i).mapping[b];
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

					plti=sqb.Tile(chardata.tile,false,chardata.plt==0xF).Get(chardata.flipx?7-bx:bx,chardata.flipy?7-by:by);
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
inline COLORREF CDlgMapEdit::ColorToLi(COLORREF color,u8 bosszone)
{
	static bool bosszonecolorinit=false;
	static struct{float r,g,b;} bosszonecolor[100];
	if(!bosszonecolorinit)
	{
		int randoldseed=rand();
		srand(123);
		for(int i=0;i<100;++i)
		{
			
			bosszonecolor[i].r=(float)rand()/RAND_MAX*0.7f+0.2f;
			bosszonecolor[i].g=(float)rand()/RAND_MAX*0.7f+0.2f;
			bosszonecolor[i].b=(float)rand()/RAND_MAX*0.7f+0.2f;
			
		}
		srand(randoldseed);
		bosszonecolorinit=true;
	}

	if(mode==MPT_GRID)return color;
	
	else if(mode==MPT_BOSS)
	{
		BYTE gr=(GetRValue(color)+GetGValue(color)+GetBValue(color))/3;
		return RGB(
			(BYTE)(bosszonecolor[bosszone].r*gr),
			(BYTE)(bosszonecolor[bosszone].g*gr),
			(BYTE)(bosszonecolor[bosszone].b*gr)
			);
	}
	return RGB(
		(GetRValue(color)+765)>>2,
		(GetGValue(color)+765)>>2,
		(GetBValue(color)+765)>>2
		);
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
		GetDet0Desc(m_Ma.Cell(x,y).det[0],&detbmp);
		det1bmp=GetDet1Bmp(m_Ma.Cell(x,y).det[1]);
		det2bmp=GetDet1Bmp(m_Ma.Cell(x,y).det[2]);
		for(u8 b=0;b<4;++b)
		{
			chardata[1]=m_Ma.TexMapping(TEXM_B,m_Ma.Cell(x,y).gra[1],true).mapping[b];
			chardata[2]=m_Ma.TexMapping(TEXM_B,m_Ma.Cell(x,y).gra[2],true).mapping[b];
			chardata[0]=m_Ma.TexMapping(TEXM_F,m_Ma.Cell(x,y).gra[0],true).mapping[b];

			for(u8 bx=0;bx<8;++bx)for(u8 by=0;by<8;++by)
			{
				dx=bx|((b&1)<<3)|(x<<4);
				dy=by|((b&2)<<2)|(y<<4);
				plti[0]=m_Fb.Tile(chardata[0].tile,false,chardata[0].plt==0xF).Get(chardata[0].flipx?7-bx:bx,chardata[0].flipy?7-by:by);
				plti[1]=m_Bb.Tile(chardata[1].tile,true,chardata[1].plt==0xF).Get(chardata[1].flipx?7-bx:bx,chardata[1].flipy?7-by:by);
				plti[2]=m_Bb.Tile(chardata[2].tile,true,chardata[2].plt==0xF).Get(chardata[2].flipx?7-bx:bx,chardata[2].flipy?7-by:by);
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
							m_CanMap.SetPixel(dx,dy,ColorToLi(RGB(0,128,0),m_Ma.Cell(x,y).guide_id));
					}else if(plti[0])m_CanMap.SetPixel(dx,dy,ColorToLi(clr[0],m_Ma.Cell(x,y).guide_id));
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
							if(detc!=RGB(255,255,255))m_CanMap.SetPixel(dx,dy,ColorToLi(detc,m_Ma.Cell(x,y).guide_id));
						}
						else
							if(plti[2])m_CanMap.SetPixel(dx,dy,ColorToLi(clr[2],m_Ma.Cell(x,y).guide_id));
					}
					else
					{
						if(CheckDet)
						{
							COLORREF detc;
							m_CanMap.SetPixel(dx,dy,ColorToGrey(clr[1]));
							detc=det1bmp->data[bx|((b&1)<<3)][by|((b&2)<<2)];
							if(detc!=RGB(255,255,255))m_CanMap.SetPixel(dx,dy,ColorToLi(detc,m_Ma.Cell(x,y).guide_id));

						}
						else m_CanMap.SetPixel(dx,dy,ColorToLi(clr[1],m_Ma.Cell(x,y).guide_id));
					}
				}
			}
		}
	}
	
}
BOOL CDlgMapEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Mctrl.FromMa(m_Ma);

	m_CanMap.Create(m_Ma.GetW()*16,m_Ma.GetH()*16);
	m_CanStock.Create(1024,max(max(
		m_Ma.GetTexMappingCount(TEXM_B)>>6,
		m_Ma.GetTexMappingCount(TEXM_F)>>5),12)<<4);
	m_CanCurGrid.Create(32,48);

	cur_grid_x=cur_grid_y=0xFF;
	cur_stock_x=cur_stock_y=0xFF;
	cursel_stock_x=cursel_stock_y=0xFF;
	cursel_stock=0xFFFFFFFF;
	grid_drawing=false;
	cur_obj=cur_sel_obj=0xFFFF;
	
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

	CheckRadioButton(0,30000,IDC_RADIO_MAP_PT_GRID);
	mode=MPT_GRID;

	m_ListObj.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ListObj.InsertColumn(0,_T("ID"),0,32);
	m_ListObj.InsertColumn(1,_T("类别"),0,64);
	m_ListObj.InsertColumn(2,_T("坐标"),0,128);
	m_ListObj.InsertColumn(3,_T("参数/脚本"),0,256);
	m_ListObj.EnableGroupView(TRUE);
	LVGROUP lvg={0};
	lvg.cbSize=sizeof(LVGROUP);
	lvg.mask=LVGF_ALIGN|LVGF_STATE|LVGF_HEADER|LVGF_GROUPID;
	lvg.state=LVGS_NORMAL; 
	lvg.uAlign=LVGA_HEADER_LEFT;
	lvg.iGroupId=0;
	lvg.pszHeader=_T("敌人");
	m_ListObj.InsertGroup(-1,&lvg);
	lvg.iGroupId=1;
	lvg.pszHeader=_T("补给品");
	m_ListObj.InsertGroup(-1,&lvg);
	lvg.iGroupId=2;
	lvg.pszHeader=_T("传送装置");
	m_ListObj.InsertGroup(-1,&lvg);
	lvg.iGroupId=3;
	lvg.pszHeader=_T("地图组件/事件");
	m_ListObj.InsertGroup(-1,&lvg);
	lvg.iGroupId=4;
	lvg.pszHeader=_T("BOSS引导器");
	m_ListObj.InsertGroup(-1,&lvg);

	UpdateObjList();

	PaintMap();
	PaintStock();

	OnStockLButtonDown(0,0);


	memcpy(&m_Bb.TileTimeDelta,&m_Ma.S12HScript.TileTimeDelta,4);
	OnBnClickedCheckMapAni();
	
	return TRUE;
}
#define OUTSIDE_MARK _T("[出界]")
void CDlgMapEdit::UpdateObjList()
{
	m_ListObj.DeleteAllItems();
	CString str,str2;

	LVITEM lvi={0};
	lvi.mask=LVIF_GROUPID|LVIF_TEXT;

	int ibase=0;

	bool in;

	//Foe
	lvi.iGroupId=0;
	for(u16 i=0;i<m_De.GetFoeCount();++i)
	{
		lvi.iItem=i;
		str.Format(_T("%d"),i);
		lvi.pszText=str.GetBuffer();
		m_ListObj.InsertItem(&lvi);
		str.Format(_T("%s"),FoeDesc[m_De.Foe(i).class_id].name);
		m_ListObj.SetItem(i,1,LVIF_TEXT,str,0,0,0,0);
		in=m_De.Foe(i).x<(m_Ma.GetW()<<4) && m_De.Foe(i).y<(m_Ma.GetH()<<4);
		str.Format(_T("%s(%d.%X,  %d.%X)"),in?_T(""):OUTSIDE_MARK,
			m_De.Foe(i).x>>4,m_De.Foe(i).y&0xF,
			m_De.Foe(i).y>>4,m_De.Foe(i).y&0xF);
		m_ListObj.SetItem(i,2,LVIF_TEXT,str,0,0,0,0);
		str.Format(_T("%04X %04X %04X %04X"),m_De.Foe(i).pa,m_De.Foe(i).pb,
			m_De.Foe(i).pc,m_De.Foe(i).pd);
		m_ListObj.SetItem(i,3,LVIF_TEXT,str.GetBuffer(),0,0,0,0);
	}
	ibase+=m_De.GetFoeCount();

	//Sup
	lvi.iGroupId=1;
	for(u16 i=0;i<m_De.GetSupCount();++i)
	{
		lvi.iItem=i+ibase;
		str.Format(_T("%d"),i);
		lvi.pszText=str.GetBuffer();
		m_ListObj.InsertItem(&lvi);
		str.Format(_T("%s"),SupDesc[m_De.Sup(i).class_id].name);
		m_ListObj.SetItem(i+ibase,1,LVIF_TEXT,str,0,0,0,0);
		in=m_De.Sup(i).x<(m_Ma.GetW()<<4) && m_De.Sup(i).y<(m_Ma.GetH()<<4);
		str.Format(_T("%s(%d.%X,  %d.%X)"),in?_T(""):OUTSIDE_MARK,
			m_De.Sup(i).x>>4,m_De.Sup(i).y&0xF,
			m_De.Sup(i).y>>4,m_De.Sup(i).y&0xF);
		m_ListObj.SetItem(i+ibase,2,LVIF_TEXT,str,0,0,0,0);
		str.Format(_T("%04X %04X %04X %04X"),m_De.Sup(i).pa,m_De.Sup(i).pb,
			m_De.Sup(i).pc,m_De.Sup(i).pd);
		m_ListObj.SetItem(i+ibase,3,LVIF_TEXT,str.GetBuffer(),0,0,0,0);
	}
	ibase+=m_De.GetSupCount();

	//Door
	lvi.iGroupId=2;
	for(u16 i=0;i<m_Ma.GetDoorCount();++i)
	{
		lvi.iItem=i+ibase;
		str.Format(_T("%d"),i);
		lvi.pszText=str.GetBuffer();
		m_ListObj.InsertItem(&lvi);
		str.Format(_T("0x%04X"),m_Ma.Door(i).class_id);
		m_ListObj.SetItem(i+ibase,1,LVIF_TEXT,str,0,0,0,0);
		in=m_Ma.Door(i).x<(m_Ma.GetW()<<4) && m_Ma.Door(i).y<(m_Ma.GetH()<<4);
		str.Format(_T("%s(%d,  %d)"),in?_T(""):OUTSIDE_MARK,
			m_Ma.Door(i).x>>4,m_Ma.Door(i).y>>4);
		m_ListObj.SetItem(i+ibase,2,LVIF_TEXT,str,0,0,0,0);
		if(m_Ma.Door(i).dst_step!=0xFF)
			str.Format(_T("To Step%02d,Door%d"),m_Ma.Door(i).dst_step,m_Ma.Door(i).dst_door);
		else if(m_Ma.Door(i).dst_door)
			str.Format(_T("%d"),m_Ma.Door(i).dst_door);
		else
			str=_T("void");
		m_ListObj.SetItem(i+ibase,3,LVIF_TEXT,str.GetBuffer(),0,0,0,0);
	}
	ibase+=m_Ma.GetDoorCount();

	//Mctrl
	lvi.iGroupId=3;
	for(u8 i=0;i<m_Mctrl.GetMctrlCount();++i)
	{
		lvi.iItem=i+ibase;
		str.Format(_T("%d"),i);
		lvi.pszText=str.GetBuffer();
		m_ListObj.InsertItem(&lvi);
		str.Format(_T("%s"),EvcDesc[m_Mctrl[i].class_id].Name);
		m_ListObj.SetItem(i+ibase,1,LVIF_TEXT,str,0,0,0,0);
		in=m_Mctrl[i].x<(m_Ma.GetW()<<4) && m_Mctrl[i].y<(m_Ma.GetH()<<4);
		str.Format(_T("%s(%d.%X,  %d.%X)"),in?_T(""):OUTSIDE_MARK,
			m_Mctrl[i].x>>4,m_Mctrl[i].y&0xF,
			m_Mctrl[i].y>>4,m_Mctrl[i].y&0xF);
		m_ListObj.SetItem(i+ibase,2,LVIF_TEXT,str,0,0,0,0);
		str=_T("ExtData:");
		for(u16 j=0;j<EvcDesc[m_Mctrl[i].class_id].DataLen;++j)
		{
			str2.Format(_T("%02X "),m_Mctrl[i].pExtData[j]);
			str+=str2;
		}
		m_ListObj.SetItem(i+ibase,3,LVIF_TEXT,str.GetBuffer(),0,0,0,0);
	}

}
void CDlgMapEdit::UpdateGuideList()
{
	m_ListObj.DeleteAllItems();
	CString str,str2;

	LVITEM lvi={0};
	lvi.mask=LVIF_GROUPID|LVIF_TEXT;

	bool in;

	//Guide
	lvi.iGroupId=4;
	for(u16 i=0;i<m_Ma.GetGuideCount();++i)
	{
		lvi.iItem=i;
		str.Format(_T("%d"),m_Ma.Guide(i).id);
		lvi.pszText=str.GetBuffer();
		m_ListObj.InsertItem(&lvi);
		str=_T("-");
		m_ListObj.SetItem(i,1,LVIF_TEXT,str,0,0,0,0);
		in=m_Ma.Guide(i).x<(m_Ma.GetW()<<4) && m_Ma.Guide(i).y<(m_Ma.GetH()<<4);
		str.Format(_T("%s(%d.%X,  %d.%X)"),in?_T(""):OUTSIDE_MARK,
			m_Ma.Guide(i).x>>4,m_Ma.Guide(i).y&0xF,
			m_Ma.Guide(i).y>>4,m_Ma.Guide(i).y&0xF);
		m_ListObj.SetItem(i,2,LVIF_TEXT,str,0,0,0,0);
		str.Format(_T("%04X"),m_Ma.Guide(i).U);
		m_ListObj.SetItem(i,3,LVIF_TEXT,str.GetBuffer(),0,0,0,0);
	}


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
		H=(m_Ma.GetTexMappingCount(TEXM_F)>>5)<<4;
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
			H=(m_Ma.GetTexMappingCount(TEXM_B)>>6)<<4;
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
	if(mode==MPT_BOSS)return;
	grid_drawing=true;
	OnMapMouseMove(x,y);
}
void CDlgMapEdit::OnMapLButtonUp(u8 x,u8 y)
{
	if(mode==MPT_BOSS)return;
	grid_drawing=false;

}
void CDlgMapEdit::OnMapDraw(u8 x,u8 y)
{
	if(mode==MPT_BOSS)return;
	if(m_CheckDet.GetCheck())
		if(m_CheckPlane0.GetCheck())m_Ma.Cell(x,y).det[0]=cursel_stock;
		else
			if(m_CheckPlane2.GetCheck())m_Ma.Cell(x,y).det[2]=cursel_stock;
			else m_Ma.Cell(x,y).det[1]=cursel_stock;
	else
		if(m_CheckPlane0.GetCheck())m_Ma.Cell(x,y).gra[0]=(u16)cursel_stock;
		else
			if(m_CheckPlane2.GetCheck())m_Ma.Cell(x,y).gra[2]=(u16)cursel_stock;
			else m_Ma.Cell(x,y).gra[1]=(u16)cursel_stock;
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
		m_EditCurGridBoss.SetWindowText(_T(""));
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

	str.Format(_T("%04X|%08X"),m_Ma.Cell(x,y).gra[0],m_Ma.Cell(x,y).det[0]);
	m_EditCurGrid0.SetWindowText(str);
	str.Format(_T("%04X|%08X"),m_Ma.Cell(x,y).gra[1],m_Ma.Cell(x,y).det[1]);
	m_EditCurGrid1.SetWindowText(str);
	str.Format(_T("%04X|%08X"),m_Ma.Cell(x,y).gra[2],m_Ma.Cell(x,y).det[2]);
	m_EditCurGrid2.SetWindowText(str);
	str.Format(_T("%d"),m_Ma.Cell(x,y).guide_id);
	m_EditCurGridBoss.SetWindowText(str);

	//Paint the grid data
	Nitro::CharData chardata[3];
	COLORREF clr[3];
	u8 plti[3];
	int dx,dy;
	BMP1b16x16* detbmp;
	BMP3216x16* det1bmp,*det2bmp;
	GetDet0Desc(m_Ma.Cell(x,y).det[0],&detbmp);
	det1bmp=GetDet1Bmp(m_Ma.Cell(x,y).det[1]);
	det2bmp=GetDet1Bmp(m_Ma.Cell(x,y).det[2]);
	for(u8 b=0;b<4;++b)
	{
		chardata[1]=m_Ma.TexMapping(TEXM_B,m_Ma.Cell(x,y).gra[1]).mapping[b];
		chardata[2]=m_Ma.TexMapping(TEXM_B,m_Ma.Cell(x,y).gra[2]).mapping[b];
		chardata[0]=m_Ma.TexMapping(TEXM_F,m_Ma.Cell(x,y).gra[0]).mapping[b];
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
	if(mode!=MPT_GRID)return;
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
			if(x>=32 || y>m_Ma.GetTexMappingCount(TEXM_F)>>5)
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
			if(x>=64 || y>m_Ma.GetTexMappingCount(TEXM_B)>>6)
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
	if(mode!=MPT_GRID)return;
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
			if(x>=32|| y>m_Ma.GetTexMappingCount(TEXM_F)>>5) return;
			else
			{
				cursel_stock_x=x;
				cursel_stock_y=y;
				cursel_stock=x+y*32;
			}
		}
		else
		{
			if(x>=64|| y>m_Ma.GetTexMappingCount(TEXM_B)>>6) return;
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
void CDlgMapEdit::OnMapMouseMove_Obj(u16 x,u16 y)
{
	PICKOBJ_GROUP n_cur_objgroup;
	u16 n_cur_obj;
	n_cur_obj=PickObj(x,y,&n_cur_objgroup);
	if(n_cur_obj!=cur_obj || n_cur_objgroup!=cur_objgroup)
	{
		cur_obj=n_cur_obj;
		cur_objgroup=n_cur_objgroup;
		GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
	}
}
void CDlgMapEdit::OnMapLButtonDown_Obj(u16 x,u16 y)
{
	cur_sel_obj=PickObj(x,y,&cur_sel_objgroup);
	if(cur_sel_obj!=0xFFFF)
	{
		int nItem=cur_sel_obj;
		switch(cur_sel_objgroup)
		{
		case MCTRL:nItem+=m_Ma.GetDoorCount();
		case DOOR:nItem+=m_De.GetSupCount();
		case SUP:nItem+=m_De.GetFoeCount();
		case FOE:break;
		default:ASSERT(FALSE);
		}
		//IsListFoeChangeFromSoftWare=true;
		m_ListObj.SetItemState(nItem,
			LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
		m_ListObj.EnsureVisible(nItem,TRUE);
		m_ListObj.SetFocus();
		//IsListFoeChangeFromSoftWare=false;
	}
}
BOOL CDlgMapEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{

	LONG Id=GetWindowLong(pWnd->GetSafeHwnd(),GWL_ID);
	CPoint pt;
	CRect rc;
	GetCursorPos(&pt);
	pWnd->GetWindowRect(&rc);
	pt-=rc.TopLeft();
	if(mode==MPT_OBJ)
	{
		if(Id==IDC_STATIC_MAP)
		{
			pt.x+=m_HScroll.GetScrollPos();
			pt.y+=m_VScroll.GetScrollPos();
			switch(message)
			{
			case WM_MOUSEMOVE:OnMapMouseMove_Obj((u16)pt.x,(u16)pt.y);break;
			case WM_LBUTTONDOWN:OnMapLButtonDown_Obj((u16)pt.x,(u16)pt.y);break;
			//case WM_LBUTTONUP:OnMapLButtonUp((u8)(pt.x>>4),(u8)(pt.y>>4));break;
			}
		}
	}
	else if(mode==MPT_GRID || mode==MPT_BOSS)
	{
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

void CDlgMapEdit::OnBnClickedRadioMapPtGrid()
{
	mode=MPT_GRID;
	m_ListObj.ShowWindow(SW_HIDE);
	m_CheckHideFoe.ShowWindow(SW_HIDE);
	m_CheckHideSup.ShowWindow(SW_HIDE);
	m_CheckHideDoor.ShowWindow(SW_HIDE);
	m_CheckHideMctrl.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_STOCK)->ShowWindow(SW_SHOW);
	m_HScrollStock.ShowWindow(SW_SHOW);
	m_VScrollStock.ShowWindow(SW_SHOW);
	PaintMap();
	GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
}

void CDlgMapEdit::OnBnClickedRadioMapPtObj()
{
	mode=MPT_OBJ;
	UpdateObjList();
	m_ListObj.ShowWindow(SW_SHOW);
	m_CheckHideFoe.ShowWindow(SW_SHOW);
	m_CheckHideSup.ShowWindow(SW_SHOW);
	m_CheckHideDoor.ShowWindow(SW_SHOW);
	m_CheckHideMctrl.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_STOCK)->ShowWindow(SW_HIDE);
	m_HScrollStock.ShowWindow(SW_HIDE);
	m_VScrollStock.ShowWindow(SW_HIDE);
	cur_obj=cur_sel_obj=0xFFFF;
	PaintMap();
	GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
}

void CDlgMapEdit::OnBnClickedRadioMapPtBoss()
{
	mode=MPT_BOSS;
	UpdateGuideList();
	m_ListObj.ShowWindow(SW_SHOW);
	m_CheckHideFoe.ShowWindow(SW_HIDE);
	m_CheckHideSup.ShowWindow(SW_HIDE);
	m_CheckHideDoor.ShowWindow(SW_HIDE);
	m_CheckHideMctrl.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_STOCK)->ShowWindow(SW_HIDE);
	m_HScrollStock.ShowWindow(SW_HIDE);
	m_VScrollStock.ShowWindow(SW_HIDE);
	PaintMap();
	GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
}

void CDlgMapEdit::OnBnClickedCheckHideObj()
{
	GetDlgItem(IDC_STATIC_MAP)->RedrawWindow();
}

u16 CDlgMapEdit::PickObj(u16 x,u16 y,PICKOBJ_GROUP *retPickGroup)
{
#define DISTANCE_F1(OBJ) (abs(OBJ.x-x)+abs(OBJ.y-y))
	if(!m_CheckHideFoe.GetCheck())for(u16 i=0;i<m_De.GetFoeCount();++i)
	{
		if(DISTANCE_F1(m_De.Foe(i))<16)
		{
			if(retPickGroup)*retPickGroup=FOE;
			return i;
		}
	}
	if(!m_CheckHideSup.GetCheck())for(u16 i=0;i<m_De.GetSupCount();++i)
	{
		if(DISTANCE_F1(m_De.Sup(i))<16)
		{
			if(retPickGroup)*retPickGroup=SUP;
			return i;
		}
	}
	if(!m_CheckHideDoor.GetCheck())for(u16 i=0;i<m_Ma.GetDoorCount();++i)
	{
		if(DISTANCE_F1(m_Ma.Door(i))<16)
		{
			if(retPickGroup)*retPickGroup=DOOR;
			return i;
		}
	}
	if(!m_CheckHideMctrl.GetCheck())for(u8 i=0;i<m_Mctrl.GetMctrlCount();++i)
	{
		if(DISTANCE_F1(m_Mctrl[i])<16)
		{
			if(retPickGroup)*retPickGroup=MCTRL;
			return i;
		}
	}
	return 0xFFFF;
#undef DISTANCE_F1
}