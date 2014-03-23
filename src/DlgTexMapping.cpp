
#include "stdafx.h"
#include "DlgTexMapping.h"
#include "DlgTmani.h"
#include "DlgTmResize.h"

struct MENU_TMCOPY_ITEM_MAP
{
	u32 StageIndex;
	u16 StepIndex;
};
CList<MENU_TMCOPY_ITEM_MAP> MenuTmcopyItemList;

IMPLEMENT_DYNAMIC(CDlgTexMapping, CDialog)

CDlgTexMapping::CDlgTexMapping(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTexMapping::IDD, pParent),cur_tmindex(0xFFFF)
{

}

CDlgTexMapping::~CDlgTexMapping()
{
}

void CDlgTexMapping::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXMAPPING_VSCROLL, m_VScroll);
	DDX_Control(pDX, IDC_TEXMAPPING_HSCROLL, m_HScroll);
	DDX_Control(pDX, IDC_STATIC_TEXMAPPING_DETAIL, m_StaticTmDetail);
	DDX_Control(pDX, IDC_CHECK_TEXMAPPING_CLR0, m_CheckClr0);
	DDX_Control(pDX, IDC_EDIT_TILE_ANI0, m_EditTileAni0);
	DDX_Control(pDX, IDC_EDIT_TILE_ANI1, m_EditTileAni1);
	DDX_Control(pDX, IDC_EDIT_TILE_ANI2, m_EditTileAni2);
	DDX_Control(pDX, IDC_EDIT_TILE_ANI3, m_EditTileAni3);
	DDX_Control(pDX, IDC_LIST_TEXMAPPING_ANI, m_ListAni);
}


BEGIN_MESSAGE_MAP(CDlgTexMapping, CDialog)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_RADIO_TEXMAPPING_A, &CDlgTexMapping::OnBnClickedRadioTexmappingA)
	ON_BN_CLICKED(IDC_RADIO_TEXMAPPING_B, &CDlgTexMapping::OnBnClickedRadioTexmappingB)
	ON_WM_DRAWITEM()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_TEXMAPPING_COPY, &CDlgTexMapping::OnBnClickedButtonTexmappingCopy)
	ON_COMMAND_RANGE(ID_MENU_TMCOPY_BASE,ID_MENU_TMCOPY_BASE+10000,&CDlgTexMapping::OnMenuTmcopy)
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_CHECK_TEXMAPPING_CLR0, &CDlgTexMapping::OnBnClickedCheckTexmappingClr0)
	ON_EN_CHANGE(IDC_EDIT_TILE_ANI0, &CDlgTexMapping::OnEnChangeEditTileAni0)
	ON_EN_CHANGE(IDC_EDIT_TILE_ANI1, &CDlgTexMapping::OnEnChangeEditTileAni1)
	ON_EN_CHANGE(IDC_EDIT_TILE_ANI2, &CDlgTexMapping::OnEnChangeEditTileAni2)
	ON_EN_CHANGE(IDC_EDIT_TILE_ANI3, &CDlgTexMapping::OnEnChangeEditTileAni3)
	ON_BN_CLICKED(IDC_BUTTON_TEXMAPPING_FX, &CDlgTexMapping::OnBnClickedButtonTexmappingFx)
	ON_BN_CLICKED(IDC_BUTTON_TEXMAPPING_FY, &CDlgTexMapping::OnBnClickedButtonTexmappingFy)
	ON_BN_CLICKED(IDC_BUTTON_TEXMAPPING_ANI_REMOVE, &CDlgTexMapping::OnBnClickedButtonTexmappingAniRemove)
	ON_BN_CLICKED(IDC_BUTTON_TEXMAPPING_ANI_PROPERTY, &CDlgTexMapping::OnBnClickedButtonTexmappingAniProperty)
	ON_LBN_DBLCLK(IDC_LIST_TEXMAPPING_ANI, &CDlgTexMapping::OnLbnDblclkListTexmappingAni)
	ON_BN_CLICKED(IDC_BUTTON_TEXMAPPING_ANI_NEW, &CDlgTexMapping::OnBnClickedButtonTexmappingAniNew)
	ON_BN_CLICKED(IDC_BUTTON_TEXMAPPING_RESIZE, &CDlgTexMapping::OnBnClickedButtonTexmappingResize)
END_MESSAGE_MAP()

void CDlgTexMapping::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	OnScroll(nSBCode,nPos,pScrollBar);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
	GetDlgItem(IDC_STATIC_TEXMAPPING)->RedrawWindow();
}

void CDlgTexMapping::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	OnScroll(nSBCode,nPos,pScrollBar);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	GetDlgItem(IDC_STATIC_TEXMAPPING)->RedrawWindow();
}
void CDlgTexMapping::SetScroll()
{
	cursel_tmindex=0xFFFF;
	u32 W,H;
	if(m_TmF)
	{
		W=512;
		H=(m_Ma.GetTexMappingCount(TEXM_F)>>5)<<4;
	}
	else
	{
		W=1024;
		H=(m_Ma.GetTexMappingCount(TEXM_B)>>6)<<4;
	}
	RECT rc;
	int hsrg,vsrg;
	GetDlgItem(IDC_STATIC_TEXMAPPING)->GetClientRect(&rc);
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
	m_HScroll.SetScrollInfo(&scrinfo);
	scrinfo.nPage=rc.bottom;
	scrinfo.nMin=0;scrinfo.nMax=vsrg-1+scrinfo.nPage-1;
	scrinfo.nPos=0;
	m_VScroll.SetScrollInfo(&scrinfo);

	m_Can.Create(W,H);
	HDC hDC=::GetDC(GetDlgItem(IDC_STATIC_TEXMAPPING)->m_hWnd);
	FillRect(hDC,&rc,(HBRUSH)GetStockObject(BLACK_BRUSH));
	::ReleaseDC(GetDlgItem(IDC_STATIC_TEXMAPPING)->m_hWnd,hDC);
	PaintTexMapping();
	GetDlgItem(IDC_STATIC_TEXMAPPING)->RedrawWindow();
}


BOOL CDlgTexMapping::OnInitDialog()
{
	CDialog::OnInitDialog();
	cur_tmindex=0xFFFF;
	cursel_tmindex=0xFFFF;
	m_AniDisp=true;
	CheckRadioButton(0,30000,IDC_RADIO_TEXMAPPING_A);
	m_TmF=true;
	SetScroll();
	m_CanTile.Create(256,256);
	PaintTile();

	CString str;
	str.Format(_T("%d"),m_Ma.S12HScript.TileTimeDelta[0]);
	m_EditTileAni0.SetLimitText(3);
	m_EditTileAni0.SetWindowText(str);
	str.Format(_T("%d"),m_Ma.S12HScript.TileTimeDelta[1]);
	m_EditTileAni1.SetLimitText(3);
	m_EditTileAni1.SetWindowText(str);
	str.Format(_T("%d"),m_Ma.S12HScript.TileTimeDelta[2]);
	m_EditTileAni2.SetLimitText(3);
	m_EditTileAni2.SetWindowText(str);
	str.Format(_T("%d"),m_Ma.S12HScript.TileTimeDelta[3]);
	m_EditTileAni3.SetLimitText(3);
	m_EditTileAni3.SetWindowText(str);

	UpdateAniList();

	SetTimer(ID_TIMER_ANI_DISP,200,0);
	return TRUE;
}

void CDlgTexMapping::OnBnClickedRadioTexmappingA()
{
	m_TmF=true;
	cursel_tmindex=0xFFFF;
	SetScroll();
	PaintTile();
	GetDlgItem(IDC_STATIC_TILE)->RedrawWindow();
}

void CDlgTexMapping::OnBnClickedRadioTexmappingB()
{
	m_TmF=false;
	cursel_tmindex=0xFFFF;
	SetScroll();
	PaintTile();
	GetDlgItem(IDC_STATIC_TILE)->RedrawWindow();
}
void CDlgTexMapping::PaintTile()
{
	SqB& b=m_TmF?m_Fb:m_Bb;
	u8 palline;
	Nitro::Tile* ptile;
	u8 plt;
	int c0=m_CheckClr0.GetCheck();
	for(u32 i=0;i<0x400;++i)
	{
		palline=b.PalLine(i);
		ptile=&b.Tile(i);
		for(u8 y=0;y<8;++y)for(u8 x=0;x<8;++x)
		{
			plt=ptile->Get(x,y);
			if(c0 && plt==0)
				m_CanTile.SetPixel(
					x|((i&31)<<3),
					y|((i>>5)<<3),
					RGB(255,255,255));
			else
				m_CanTile.SetPixel(
					x|((i&31)<<3),
					y|((i>>5)<<3),
					Nitro::Color15to24(b.Pal[palline<<4|plt]));
		}
	}
}
void CDlgTexMapping::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	RECT rc;
	CPen* pOldPen;
	if(nIDCtl==IDC_STATIC_TEXMAPPING)
	{
		GetDlgItem(IDC_STATIC_TEXMAPPING)->GetClientRect(&rc);
		m_Can.Present(lpDrawItemStruct->hDC,0,0,rc.right,rc.bottom,
			m_HScroll.GetScrollPos(),m_VScroll.GetScrollPos());

		u8 w=m_TmF?32:64;

		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);
		CBrush NullBrush,*pOldBrush;
		static CPen PenAniM(PS_DOT,1,RGB(255,128,0)),PenAniS(PS_DOT,1,RGB(0,0,255)),
			PenCur(PS_SOLID,1,RGB(255,0,0)),PenCurL(PS_DOT,1,RGB(255,0,0)),
			PenCurSel(PS_SOLID,2,RGB(255,0,0));
		NullBrush.CreateStockObject(NULL_BRUSH);
		pOldBrush=dc.SelectObject(&NullBrush);
		pOldPen=dc.SelectObject(&PenAniM);
		if(m_AniDisp)
		{
			
			int x,y;
			u16 idx;
			for(u16 i=0;i<m_Ma.GetGraScriptCount();++i)
			{
				if((m_Ma.GraScript(i).TexMappingPlane==0 && m_TmF)||
					(m_Ma.GraScript(i).TexMappingPlane==1 &&!m_TmF))
				{
					idx=m_Ma.GraScript(i).TexMappingIndex;
					dc.SelectObject(&PenAniM);
					x=(idx%w)*16-m_HScroll.GetScrollPos();
					y=(idx/w)*16-m_VScroll.GetScrollPos();
					dc.Rectangle(x,y,x+16,y+16);
					dc.SelectObject(&PenAniS);
					for(u8 j=0;j<m_Ma.GraScript(i).FrameCount;++j)
					{
						x=((idx+j+1)%w)*16-m_HScroll.GetScrollPos();
						y=((idx+j+1)/w)*16-m_VScroll.GetScrollPos();
						dc.Rectangle(x,y,x+16,y+16);
					}
				}
			}
			
		}

		if(cur_tmindex!=0xFFFF)
		{
			
			int x,y;
			x=(cur_tmindex%w)*16-m_HScroll.GetScrollPos();
			y=(cur_tmindex/w)*16-m_VScroll.GetScrollPos();
			dc.SelectObject(&PenCurL);
			dc.Rectangle(x,y,x+16,y+16);
			dc.SelectObject(&PenCur);
			dc.Rectangle(x+(cur_stm&1)*8,y+(cur_stm>>1)*8,
				x+(cur_stm&1)*8+8,y+(cur_stm>>1)*8+8);
			
		}
		if(cursel_tmindex!=0xFFFF)
		{
			int x,y;
			x=(cursel_tmindex%w)*16-m_HScroll.GetScrollPos();
			y=(cursel_tmindex/w)*16-m_VScroll.GetScrollPos();
			dc.SelectObject(&PenCurSel);
			dc.Rectangle(x+(cursel_stm&1)*8-1,y+(cursel_stm>>1)*8-1,
				x+(cursel_stm&1)*8+8+1,y+(cursel_stm>>1)*8+8+1);
		}
		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);
		NullBrush.DeleteObject();
		dc.Detach();
	}
	else if(nIDCtl==IDC_STATIC_TILE)
	{
		m_CanTile.Present(lpDrawItemStruct->hDC,0,0,256,256,0,0);
		if(cursel_tmindex!=0xFFFF)
		{
			CDC dc;
			dc.Attach(lpDrawItemStruct->hDC);
			int x,y;
			static CPen PenCur(PS_SOLID,1,RGB(255,0,0));
			CBrush NullBrush,*pOldBrush;
			NullBrush.CreateStockObject(NULL_BRUSH);
			pOldPen=dc.SelectObject(&PenCur);
			pOldBrush=dc.SelectObject(&NullBrush);
			u16 ti=m_Ma.TexMapping(m_TmF?TEXM_F:TEXM_B,cursel_tmindex).mapping[cursel_stm].tile;
			x=(ti%32)*8;
			y=(ti/32)*8;
			
			dc.Rectangle(x,y,x+8,y+8);

			dc.SelectObject(pOldPen);
			dc.SelectObject(pOldBrush);
			NullBrush.DeleteObject();
			dc.Detach();
		}
	}
}
void CDlgTexMapping::PaintTexMapping()
{
	SqB *pB;
	u8 w;
	u16 Count;
	u8 plane;
	if(m_TmF)
	{
		w=32;
		pB=&m_Fb;
		plane=TEXM_F;
		Count=m_Ma.GetTexMappingCount(TEXM_F);
	}
	else
	{
		w=64;
		pB=&m_Bb;
		plane=TEXM_B;
		Count=m_Ma.GetTexMappingCount(TEXM_B);
	}
	int c0=m_CheckClr0.GetCheck();
	Nitro::CharData chardata;
	COLORREF clr;
	u8 plti;
	int dx,dy;
	for(u16 i=0;i<Count;++i)
	{
		for(u8 b=0;b<4;++b)
		{
			chardata=m_Ma.TexMapping(plane,i,false).mapping[b];
			for(u8 bx=0;bx<8;++bx)for(u8 by=0;by<8;++by)
			{
				dx=((i%w)<<4)|bx|((b&1)<<3);
				dy=((i/w)<<4)|by|((b&2)<<2);

				plti=pB->Tile(chardata.tile).Get(chardata.flipx?7-bx:bx,chardata.flipy?7-by:by);
				if(c0&&plti==0)
				{
					m_Can.SetPixel(dx,dy,RGB(255,255,255));
				}
				else
				{
					clr=Nitro::Color15to24(pB->Pal[plti|(pB->PalLine(chardata.tile)<<4)]);
					m_Can.SetPixel(dx,dy,clr);
				}	
			}
			
		}
	}
}
void CDlgTexMapping::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==ID_TIMER_ANI_DISP)
	{
		m_AniDisp=!m_AniDisp;
		GetDlgItem(IDC_STATIC_TEXMAPPING)->RedrawWindow();
	}
	CDialog::OnTimer(nIDEvent);
}

void CDlgTexMapping::OnBnClickedButtonTexmappingCopy()
{
	CMenu menu;
	menu.CreatePopupMenu();
	CString str;
	u8 fb,bb;
	u8 el,es;
	int menui=0;
	MENU_TMCOPY_ITEM_MAP mtim;
	MenuTmcopyItemList.RemoveAll();
	for(u32 i=0;i<m_pMapSet->GetStageCount();++i)
		for(u16 j=0;j<m_pMapSet->GetStepCount(i);++j)
			if(i!=m_StageIdx && j!=m_StepIdx)
			{
				m_pMapSet->GetStepInfo(i,j,0,&bb,&fb,0);
				if((m_TmF && fb==m_FbIndex) ||
					(!m_TmF && bb==m_BbIndex))
				{
					m_pMapSet->GetStageEntry(i,&el,&es);
					str.Format(_T("Level%d:Stage%d:Step%02d"),el,es,j);
					menu.AppendMenu(0,ID_MENU_TMCOPY_BASE+menui,str);
					++menui;
					mtim.StageIndex=i;
					mtim.StepIndex=j;
					MenuTmcopyItemList.AddTail(mtim);
				}
			}
	RECT rc;
	GetDlgItem(IDC_BUTTON_TEXMAPPING_COPY)->GetWindowRect(&rc);
	menu.TrackPopupMenu(
		GetSystemMetrics(SM_MENUDROPALIGNMENT)?
		TPM_RIGHTALIGN:TPM_LEFTALIGN,
		rc.left,rc.bottom,this);
	menu.DestroyMenu();
}
void CDlgTexMapping::OnMenuTmcopy(UINT nID)
{
	MENU_TMCOPY_ITEM_MAP& stage=MenuTmcopyItemList.GetAt(
		MenuTmcopyItemList.FindIndex(nID-ID_MENU_TMCOPY_BASE));
	SqMa copysrc;
	copysrc.Load(m_pMapSet->GetMxpBuffer(stage.StageIndex,stage.StepIndex));
	m_Ma.CopyTexMapping(copysrc,m_TmF?TEXM_F:TEXM_B);
	UpdateAniList();
	SetScroll();
	

}
BOOL CDlgTexMapping::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{

	LONG Id=GetWindowLong(pWnd->GetSafeHwnd(),GWL_ID);
	CPoint pt;
	CRect rc;
	GetCursorPos(&pt);
	pWnd->GetWindowRect(&rc);
	pt-=rc.TopLeft();
	if(Id==IDC_STATIC_TEXMAPPING && pt.x+m_HScroll.GetScrollPos()<(m_TmF?512:1024))
	{
		pt.x+=m_HScroll.GetScrollPos();
		pt.y+=m_VScroll.GetScrollPos();
		u16 tmindex=(u16)((pt.x>>4)|((pt.y>>4)<<(m_TmF?5:6)));
		u8 stm=((pt.x&8)>>3)|((pt.y&8)>>2);
		if(tmindex<m_Ma.GetTexMappingCount(m_TmF?TEXM_F:TEXM_B))switch(message)
		{
		case WM_MOUSEMOVE:OnTexMappingMouseMove(tmindex,stm);break;
		case WM_LBUTTONDOWN:OnTexMappingLButtonDown(tmindex,stm);break;
		}
		else OnTexMappingMouseMove(0xFFFF,0xFF);
	}
	else if(Id==IDC_STATIC_TILE && message==WM_LBUTTONDOWN && cursel_tmindex!=0xFFFF)
	{
		if(pt.x<256 && pt.y<256)
		{
			u16 ti=(u16)(pt.x/8+pt.y/8*32);
			m_Ma.TexMapping(m_TmF?TEXM_F:TEXM_B,cursel_tmindex).mapping[cursel_stm].tile=ti;
			m_Ma.TexMapping(m_TmF?TEXM_F:TEXM_B,cursel_tmindex).mapping[cursel_stm].flipx=0;
			m_Ma.TexMapping(m_TmF?TEXM_F:TEXM_B,cursel_tmindex).mapping[cursel_stm].flipy=0;
			PaintTexMapping();
			GetDlgItem(IDC_STATIC_TEXMAPPING)->RedrawWindow();
			GetDlgItem(IDC_STATIC_TILE)->RedrawWindow();

		}
	}
	else
	{
		OnTexMappingMouseMove(0xFFFF,0xFF);
	}
	

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
void CDlgTexMapping::OnTexMappingMouseMove(u16 tmindex,u8 stm)
{
	cur_tmindex=tmindex;
	cur_stm=stm;
	GetDlgItem(IDC_STATIC_TEXMAPPING)->RedrawWindow();
	if(tmindex==0xFFFF)
	{
		m_StaticTmDetail.SetWindowText(_T(""));
	}
	else
	{
		Nitro::CharData &tm=m_Ma.TexMapping(m_TmF?TEXM_F:TEXM_B,cur_tmindex).mapping[cur_stm];
		CString str;
		str.Format(_T("Mapping ID:%d (%d)\nTile ID:%d\nFlip: %s %s"),
			cur_tmindex,cur_stm,tm.tile,tm.flipx?_T("x"):_T("-"),tm.flipy?_T("y"):_T("-"));
		m_StaticTmDetail.SetWindowText(str);
	}
}
void CDlgTexMapping::OnTexMappingLButtonDown(u16 tmindex,u8 stm)
{
	cursel_tmindex=tmindex;
	cursel_stm=stm;
	GetDlgItem(IDC_STATIC_TEXMAPPING)->RedrawWindow();
	GetDlgItem(IDC_STATIC_TILE)->RedrawWindow();
}

void CDlgTexMapping::OnBnClickedCheckTexmappingClr0()
{
	PaintTexMapping();
	GetDlgItem(IDC_STATIC_TEXMAPPING)->RedrawWindow();
	PaintTile();
	GetDlgItem(IDC_STATIC_TILE)->RedrawWindow();
}

void CDlgTexMapping::OnEnChangeEditTileAni0()
{
	CString str;
	m_EditTileAni0.GetWindowText(str);
	int n=_ttoi(str);
	if(n>255)n=255;
	m_Ma.S12HScript.TileTimeDelta[0]=(u8)n;
}

void CDlgTexMapping::OnEnChangeEditTileAni1()
{
	CString str;
	m_EditTileAni1.GetWindowText(str);
	int n=_ttoi(str);
	if(n>255)n=255;
	m_Ma.S12HScript.TileTimeDelta[1]=(u8)n;
}

void CDlgTexMapping::OnEnChangeEditTileAni2()
{
	CString str;
	m_EditTileAni2.GetWindowText(str);
	int n=_ttoi(str);
	if(n>255)n=255;
	m_Ma.S12HScript.TileTimeDelta[2]=(u8)n;
}

void CDlgTexMapping::OnEnChangeEditTileAni3()
{
	CString str;
	m_EditTileAni3.GetWindowText(str);
	int n=_ttoi(str);
	if(n>255)n=255;
	m_Ma.S12HScript.TileTimeDelta[3]=(u8)n;
}

void CDlgTexMapping::OnBnClickedButtonTexmappingFx()
{
	if(cursel_tmindex!=0xFFFF)
	{
		Nitro::CharData &c=m_Ma.TexMapping(m_TmF?TEXM_F:TEXM_B,cursel_tmindex).mapping[cursel_stm];
		c.flipx=1-c.flipx;
		PaintTexMapping();
		GetDlgItem(IDC_STATIC_TEXMAPPING)->RedrawWindow();
	}
}

void CDlgTexMapping::OnBnClickedButtonTexmappingFy()
{
	if(cursel_tmindex!=0xFFFF)
	{
		Nitro::CharData &c=m_Ma.TexMapping(m_TmF?TEXM_F:TEXM_B,cursel_tmindex).mapping[cursel_stm];
		c.flipy=1-c.flipy;
		PaintTexMapping();
		GetDlgItem(IDC_STATIC_TEXMAPPING)->RedrawWindow();
	}
}

void CDlgTexMapping::UpdateAniList()
{
	m_ListAni.ResetContent();
	CString str,str2;
	for(u16 i=0;i<m_Ma.GetGraScriptCount();++i)
	{
		str.Format(_T("%s-%d,%d Frames:"),m_Ma.GraScript(i).TexMappingPlane?
			_T("Plane1"):_T("Plane0"),m_Ma.GraScript(i).TexMappingIndex,
			m_Ma.GraScript(i).FrameCount);
		for(u8 j=0;j<m_Ma.GraScript(i).FrameCount;++j)
		{
			str2.Format(_T("%d "),m_Ma.GraScript(i).TimeDelta[j]);
			str+=str2;
		}
		m_ListAni.AddString(str);
	}
}
void CDlgTexMapping::OnBnClickedButtonTexmappingAniRemove()
{
	int i=m_ListAni.GetCurSel();
	if(i!=LB_ERR)
	{
		m_Ma.RemoveGraScript((u16)i);
		UpdateAniList();
	}
}

void CDlgTexMapping::OnBnClickedButtonTexmappingAniProperty()
{
	int i=m_ListAni.GetCurSel();
	if(i!=LB_ERR)
	{
		CDlgTmani dlg;
		dlg.m_GraScript=m_Ma.GraScript((u16)i);
		if(dlg.DoModal()==IDOK)
		{
			m_Ma.GraScript((u16)i)=dlg.m_GraScript;
			UpdateAniList();
		}
	}
}

void CDlgTexMapping::OnLbnDblclkListTexmappingAni()
{
	OnBnClickedButtonTexmappingAniProperty();

}

void CDlgTexMapping::OnBnClickedButtonTexmappingAniNew()
{
	CDlgTmani dlg;
	ZeroMemory(&dlg.m_GraScript,sizeof(dlg.m_GraScript));
	if(dlg.DoModal()==IDOK)
	{
		m_Ma.GraScript(m_Ma.NewGraScript())=dlg.m_GraScript;
		UpdateAniList();
	}
	
}

void CDlgTexMapping::OnBnClickedButtonTexmappingResize()
{
	u8 plane=m_TmF?TEXM_F:TEXM_B;
	CDlgTmResize dlg;
	dlg.m_TexMappingCount=m_Ma.GetTexMappingCount(plane);
	if(dlg.DoModal()==IDOK)
	{
		m_Ma.ResizeTexMapping(plane,dlg.m_TexMappingCount);
		SetScroll();
	}
}
