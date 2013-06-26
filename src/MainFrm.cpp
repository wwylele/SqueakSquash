

#include "stdafx.h"
#include "Main.h"

#include "MainFrm.h"
#include "SqB.h"
#include "SqPl1.h"
#include "SqMa.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_OPEN,&CMainFrame::OnTbOpen)
	ON_COMMAND(ID_SAVE,&CMainFrame::OnTbSave)
	ON_COMMAND(ID_SAVEAS,&CMainFrame::OnTbSaveas)
	ON_WM_SIZE()
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, &CMainFrame::OnTtnNeedText)
	ON_NOTIFY(TVN_SELCHANGED, ID_FILETREE, &CMainFrame::OnNMClickFileTreeFile)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()





CMainFrame::CMainFrame()
{

}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		 | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED) ||
		!m_ToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Fail to create the toolbar.\n");
		return -1;      
	}

	RECT ZeroRect={0,0,0,0};

	m_FileTree.Create(WS_CHILD | WS_VISIBLE |WS_BORDER|
		TVS_LINESATROOT|TVS_HASBUTTONS|TVS_FULLROWSELECT|
		TVS_HASLINES|TVS_SHOWSELALWAYS ,
		ZeroRect,this,ID_FILETREE);

	m_StaticDesc.Create(_T("..."),WS_CHILD|WS_VISIBLE,ZeroRect,this);
	m_StaticPrvw.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_OWNERDRAW,ZeroRect,this,ID_STATIC_PRVW);

	BMP_PRVW_W=GetSystemMetrics(SM_CXFULLSCREEN)-250;
	BMP_PRVW_H=GetSystemMetrics(SM_CYFULLSCREEN)-100;

	CDC *pDC=GetDC();
	m_DCPrvw.CreateCompatibleDC(pDC);
	m_BmpPrvw.CreateCompatibleBitmap(pDC,BMP_PRVW_W,BMP_PRVW_H);
	m_DCPrvw.SelectObject(&m_BmpPrvw);
	m_DCPrvw.FillRect((LPRECT)&CRect(0,0,BMP_PRVW_W,BMP_PRVW_H),&CBrush((COLORREF)0));

	ReleaseDC(pDC);
	
	return 0;
}
void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	m_FileTree.MoveWindow(0,30,200,cy-35);
	m_StaticDesc.MoveWindow(202,30,600,40);
	m_StaticPrvw.MoveWindow(202,70,cx-220,cy-80);
}
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0,0,
		GetSysColorBrush(COLOR_3DFACE),
		LoadIcon(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME))
		);
	return TRUE;
}

void CMainFrame::OnTbSaveas()
{
	if(!m_SqMapSet.IsLoaded())return;
	CFile file;

	TCHAR strPath[1000]={0};
	//GetPrivateProfileString(_T("FILE"),_T("Path"),0,strPath,999,ProfilePath);
	CFileDialog filedlg(FALSE,_T("sqms"),_T("NewMapSet.sqms"),OFN_OVERWRITEPROMPT,
		_T("SqueakSquashMapSet|*.sqms||"),this);
	filedlg.GetOFN().lpstrInitialDir=strPath;
	filedlg.GetOFN().lpstrTitle=_T("Set the file path...");
	if(filedlg.DoModal()==IDCANCEL)return;
	CString strFileName=filedlg.GetPathName();
	if(!file.Open(strFileName,CFile::modeCreate|CFile::modeWrite))
	{
		MessageBox(_T("Fail to create the file."),_T("Error"),MB_ICONERROR);
		return;
	}
	m_StrFileName=strFileName;

	if(!m_SqMapSet.Save(file))
	{
		MessageBox(_T("SqMapSet::Save:Fail to save the file."),_T("Error"),MB_ICONERROR);
	}
	file.Close();
}
void CMainFrame::OnTbSave()
{
	if(!m_SqMapSet.IsLoaded())return;
	CFile file;
	if(m_StrFileName==_T(""))
	{
		TCHAR strPath[1000]={0};
		//GetPrivateProfileString(_T("FILE"),_T("Path"),0,strPath,999,ProfilePath);
		CFileDialog filedlg(FALSE,_T("sqms"),_T("NewMapSet.sqms"),OFN_OVERWRITEPROMPT,
			_T("SqueakSquashMapSet|*.sqms||"),this);
		filedlg.GetOFN().lpstrInitialDir=strPath;
		filedlg.GetOFN().lpstrTitle=_T("Set the file path...");
		if(filedlg.DoModal()==IDCANCEL)return;
		CString strFileName=filedlg.GetPathName();
		if(!file.Open(strFileName,CFile::modeCreate|CFile::modeWrite))
		{
			MessageBox(_T("Fail to create the file."),_T("Error"),MB_ICONERROR);
			return;
		}
		m_StrFileName=strFileName;
	}
	else
	{
		if(!file.Open(m_StrFileName,CFile::modeCreate|CFile::modeWrite))
		{
			MessageBox(_T("Fail to save the file."),_T("Error"),MB_ICONERROR);
			return;
		}
	}
	if(!m_SqMapSet.Save(file))
	{
		MessageBox(_T("SqMapSet::Save:Fail to save the file."),_T("Error"),MB_ICONERROR);
	}
	file.Close();
	
}
void CMainFrame::OnTbOpen()
{
	TCHAR strPath[1000]={0};
	//GetPrivateProfileString(_T("FILE"),_T("Path"),0,strPath,999,ProfilePath);
	
	CFileDialog filedlg(TRUE,0,0,OFN_HIDEREADONLY,
		_T("All supported files|*.sqms;*.nds;*.bin|")
		_T("SqueakSquashMapSet(*.sqms)|*.sqms|")
		_T("Nitro ROM(*.nds;*.bin)|*.nds;*.bin|")
		_T("All files(*.*)|*.*|")
		_T("|"),this);
	filedlg.GetOFN().lpstrInitialDir=strPath;
	if(filedlg.DoModal()==IDCANCEL)return;
	CString strFileName=filedlg.GetPathName();
	CFile file;
	if(!file.Open(strFileName,CFile::modeRead))
	{
		MessageBox(_T("Fail to open the file"),_T("Error"),MB_ICONERROR);
		return;
	}
	if(!m_SqMapSet.Load(file))
	{
		//Fail to load it as a map set,try to load it as a ROM
		MessageBox(_T("The program is going to load the file as a ROM.\n")
			_T("It may take about a minute and the window will not response to your request when loading."),
			_T("SqueakSquash"));
		if(!m_SqMapSet.LoadFromRom(file))
		{
			MessageBox(_T("Fail to load the file"),_T("Error"),MB_ICONERROR);
			file.Close();
			return;
		}
		else
		{
			MessageBox(_T("Success to load from ROM"),_T("SqueakSquash"));
			m_StrFileName=_T("");
		}
	}
	else
	{
		m_StrFileName=strFileName;
	}
	file.Close();

	/*FILE* of=fopen("nsbtx.txt","wt");
	char strn[16];
	u32 len;
	for(u32 i=0;i<m_SqMapSet.GetBgCount();++i)
	{
		m_SqMapSet.GetBgName(i,strn);
		fprintf(of,"\n%s:\t",strn);
		for(u32 j=0;j<0xB0;++j)
		{
			fprintf(of,"%02X ",m_SqMapSet.GetBgBuffer(i,&len)[j]);
		}
		fprintf(of,"Len=%08X",len);
	}

	fclose(of);*/

	FlushFileTree();
}


void CMainFrame::FlushFileTree()
{
	ASSERT(m_SqMapSet.IsLoaded());

	CString str;
	char strn[16];

	m_FileTree.DeleteAllItems();
	m_htiMapi.RemoveAll();
	m_htiRom=m_FileTree.InsertItem(_T("Rom Information"));
	m_htiMap=m_FileTree.InsertItem(_T("Map"));
	m_htiBg=m_FileTree.InsertItem(_T("Background"));
	m_htiGl=m_FileTree.InsertItem(_T("Texture"));
	m_htiPl=m_FileTree.InsertItem(_T("Palette"));

	for(u32 i=0;i<m_SqMapSet.GetStageCount();++i)
	{
		u8 l,s;
		m_SqMapSet.GetStageInfo(i,&l,&s);
		str.Format(_T("Level%d:Stage%d"),l,s);
		m_htiMapi.Add(m_FileTree.InsertItem(str,m_htiMap));
		for(u16 j=0;j<m_SqMapSet.GetStepCount(i);++j)
		{
			str.Format(_T("Step%02d"),j);
			m_FileTree.InsertItem(TVIF_TEXT|TVIF_PARAM,str,0,0,0,0,j,m_htiMapi[i],TVI_LAST);
		}
	}

	for(u32 i=0;i<m_SqMapSet.GetBgCount();++i)
	{
		m_SqMapSet.GetBgName(i,strn);
		str.Format(FORMAT_A2T,strn);
		m_FileTree.InsertItem(TVIF_TEXT|TVIF_PARAM,str,0,0,0,0,i,m_htiBg,TVI_LAST);
	}
	for(u32 i=0;i<m_SqMapSet.GetGlCount();++i)
	{
		m_SqMapSet.GetGlName(i,strn);
		str.Format(FORMAT_A2T,strn);
		m_FileTree.InsertItem(TVIF_TEXT|TVIF_PARAM,str,0,0,0,0,i,m_htiGl,TVI_LAST);
	}
	for(u32 i=0;i<m_SqMapSet.GetPlCount();++i)
	{
		m_SqMapSet.GetPlName(i,strn);
		str.Format(FORMAT_A2T,strn);
		m_FileTree.InsertItem(TVIF_TEXT|TVIF_PARAM,str,0,0,0,0,i,m_htiPl,TVI_LAST);
	}
}

void CMainFrame::PaintStepPrvw(u32 Stage,u16 Step)
{
	m_DCPrvw.FillRect((LPRECT)&CRect(0,0,BMP_PRVW_W,BMP_PRVW_H),&CBrush((COLORREF)0));

	SqMa sqma;
	SqB bb,fb;
	u8 bbi,fbi;
	m_SqMapSet.GetStepInfo(Stage,Step,0,&bbi,&fbi,0);
	sqma.Load(m_SqMapSet.GetMxpBuffer(Stage,Step));
	bb.Load(m_SqMapSet.GetGlBuffer(bbi,0));
	fb.Load(m_SqMapSet.GetGlBuffer(fbi,0));

	u16 dw=sqma.GetW()<<4,dh=sqma.GetH()<<4;

	CDC TempDC,*pDC=GetDC();
	CBitmap TempBmp;
	TempDC.CreateCompatibleDC(pDC);
	TempBmp.CreateCompatibleBitmap(pDC,dw,dh);
	TempDC.SelectObject(&TempBmp);
	ReleaseDC(pDC);

	for(u8 y=0;y<sqma.GetH();++y)for(u8 x=0;x<sqma.GetW();++x)
	{

		bb.DrawTile(&TempDC,
			sqma.BlockMappingB(sqma.Grid(x,y).gra[1]).mapping[0],
			(x<<4)+0,(y<<4)+0,true,true);
		bb.DrawTile(&TempDC,
			sqma.BlockMappingB(sqma.Grid(x,y).gra[1]).mapping[1],
			(x<<4)+8,(y<<4)+0,true,true);
		bb.DrawTile(&TempDC,
			sqma.BlockMappingB(sqma.Grid(x,y).gra[1]).mapping[2],
			(x<<4)+0,(y<<4)+8,true,true);
		bb.DrawTile(&TempDC,
			sqma.BlockMappingB(sqma.Grid(x,y).gra[1]).mapping[3],
			(x<<4)+8,(y<<4)+8,true,true);
		fb.DrawTile(&TempDC,
			sqma.BlockMappingA(sqma.Grid(x,y).gra[0]).mapping[0],
			(x<<4)+0,(y<<4)+0,true,true);
		fb.DrawTile(&TempDC,
			sqma.BlockMappingA(sqma.Grid(x,y).gra[0]).mapping[1],
			(x<<4)+8,(y<<4)+0,true,true);
		fb.DrawTile(&TempDC,
			sqma.BlockMappingA(sqma.Grid(x,y).gra[0]).mapping[2],
			(x<<4)+0,(y<<4)+8,true,true);
		fb.DrawTile(&TempDC,
			sqma.BlockMappingA(sqma.Grid(x,y).gra[0]).mapping[3],
			(x<<4)+8,(y<<4)+8,true,true);
	}
	int strtw,strth;
	float q;
	if(dw<=BMP_PRVW_W && dh<=BMP_PRVW_H){
		strtw=dw;
		strth=dh;
	}else if((q=(float)dw/BMP_PRVW_W)>(float)dh/BMP_PRVW_H)
	{
		strtw=BMP_PRVW_W;
		strth=(int)(dh/q);
	}else{
		q=(float)dh/BMP_PRVW_H;
		strtw=(int)(dw/q);
		strth=BMP_PRVW_H;
	}
	m_DCPrvw.StretchBlt(0,0,strtw,strth,&TempDC,0,0,dw,dh,SRCCOPY);
	TempDC.DeleteDC();
	TempBmp.DeleteObject();
	m_StaticPrvw.RedrawWindow();
}
void CMainFrame::PaintRomPrvw()
{
	m_DCPrvw.FillRect((LPRECT)&CRect(0,0,BMP_PRVW_W,BMP_PRVW_H),&CBrush((COLORREF)0));
	u8 pixl;
	for(int by=0;by<4;by++)for(int bx=0;bx<4;bx++)
	for(int ty=0;ty<8;ty++)for(int tx=0;tx<8;tx++)
	{
		if(tx&1)
		{
			pixl=m_SqMapSet.m_RomInfo.Title_icon_pixel[by][bx][ty][tx/2]>>4;
		}
		else
		{
			pixl=m_SqMapSet.m_RomInfo.Title_icon_pixel[by][bx][ty][tx/2]&0xF;
		}
		m_DCPrvw.SetPixel((bx<<3)+tx,(by<<3)+ty,Nitro::Color15to24(m_SqMapSet.m_RomInfo.Title_icon_palette[pixl]));
		
	}
	m_StaticPrvw.RedrawWindow();
}

void CMainFrame::PaintBgPrvw(u32 index)
{
	m_DCPrvw.FillRect((LPRECT)&CRect(0,0,BMP_PRVW_W,BMP_PRVW_H),&CBrush((COLORREF)0));
	u32 Len;
	u8 *p;
	p=m_SqMapSet.GetBgBuffer(index,&Len);
	
	//if(Len==0x202A4)
	Len&=0xF0000;

	u8* pD=p+0xA4;
	Nitro::Color15 *pP=(Nitro::Color15*)(p+Len+0xA4);
	
	for(u32 i=0;i<Len;++i)
	{
		m_DCPrvw.SetPixel(i%(Len>>8),i/(Len>>8),Nitro::Color15to24(pP[pD[i]]));
	}
	
	for(u32 i=0;i<0x100;++i)
	{
		m_DCPrvw.FillRect((LPRECT)&CRect(
			256+((i&15)<<4),256+((i>>4)<<4),256+((i&15)<<4)+16,256+((i>>4)<<4)+16),
			&CBrush(Nitro::Color15to24(pP[i])));
	}

	m_StaticPrvw.RedrawWindow();
}

void CMainFrame::PaintGlPrvw(u32 index)
{
	SqB sqb;
	sqb.Load(m_SqMapSet.GetGlBuffer(index,0));
	m_DCPrvw.FillRect((LPRECT)&CRect(0,0,BMP_PRVW_W,BMP_PRVW_H),&CBrush((COLORREF)0));
	for(u16 i=0;i<sqb.GetTileCount();++i)
	{
		sqb.DrawTile(&m_DCPrvw,i,256+((i&31)<<3),(i>>5)<<3);
	}
	for(u32 i=0;i<0x100;++i)
	{
		m_DCPrvw.FillRect((LPRECT)&CRect(
			(i&15)<<4,(i>>4)<<4,((i&15)<<4)+16,((i>>4)<<4)+16),
			&CBrush(Nitro::Color15to24(sqb.Pal[i])));
	}
	m_StaticPrvw.RedrawWindow();
}
void CMainFrame::PaintPlPrvw(u32 index)
{
	
	m_DCPrvw.FillRect((LPRECT)&CRect(0,0,BMP_PRVW_W,BMP_PRVW_H),&CBrush((COLORREF)0));
	u8* pPl=m_SqMapSet.GetPlBuffer(index,0);
	if(*(u16*)pPl==2)
	{
		for(u32 i=0;i<0x100;++i)
		{
			m_DCPrvw.FillRect((LPRECT)&CRect(
				(i&15)<<4,(i>>4)<<4,((i&15)<<4)+16,((i>>4)<<4)+16),
				&CBrush(Nitro::Color15to24(*((Nitro::Color15*)pPl+i+1))));
		}
	}
	else
	{
		SqPl1 sqpl1;
		sqpl1.Load(pPl);
		for(u8 line=0;line<16;++line)for(u8 frame=0;frame<sqpl1.GetFrameCount(line);++frame)for(u8 i=0;i<16;++i)
		{
			m_DCPrvw.FillRect((LPRECT)&CRect(
				i<<4,(line<<4)+frame,(i<<4)+16,(line<<4)+frame+1
				),&CBrush(Nitro::Color15to24(sqpl1.GetFrame(line,frame).Pl[i])));
				
		}
	}
	m_StaticPrvw.RedrawWindow();
}
BOOL CMainFrame::OnTtnNeedText(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	UINT_PTR nID = pNMHDR->idFrom;

	CString strTipText;
   //nID = ::GetDlgCtrlID((HWND)nID);
	if(nID!=0)
	{
		strTipText.LoadString(nID);
		lstrcpyn(pTTT->szText, strTipText, sizeof(pTTT->szText));
	}

	*pResult=0;
	return TRUE;
}
void CMainFrame::OnNMClickFileTreeFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	char strn[16];
	if(m_SqMapSet.IsLoaded())
	{
		HTREEITEM tic=m_FileTree.GetSelectedItem();

		CString str;
		if(tic==m_htiRom)
		{
			str.Format(_T("ROM:")FORMAT_A2T _T("|") FORMAT_A2T _T("\n")
				_T("Title:")FORMAT_W2T _T("\n"),
				m_SqMapSet.m_RomInfo.Rom_name,
				m_SqMapSet.m_RomInfo.Rom_id,
				m_SqMapSet.m_RomInfo.Title_text[0]);
			m_StaticDesc.SetWindowText(str);
			PaintRomPrvw();
		}
		else if(tic==m_htiMap)
		{
			str.Format(_T("Stage Count:%u"),m_SqMapSet.GetStageCount());
			m_StaticDesc.SetWindowText(str);
		}
		else if(tic==m_htiBg)
		{
			str.Format(_T("Background Count:%u"),m_SqMapSet.GetBgCount());
			m_StaticDesc.SetWindowText(str);
		}
		else if(tic==m_htiGl)
		{
			str.Format(_T("Texture Count:%u"),m_SqMapSet.GetGlCount());
			m_StaticDesc.SetWindowText(str);
		}
		else if(tic==m_htiPl)
		{
			str.Format(_T("Palatte Count:%u"),m_SqMapSet.GetPlCount());
			m_StaticDesc.SetWindowText(str);
		}
		else if(m_FileTree.GetParentItem(tic)==m_htiMap)
		{
			u32 i=0;
			u8 l,s;
			for(;i<(u32)m_htiMapi.GetCount();++i)if(m_htiMapi[i]==tic)break;
			m_SqMapSet.GetStageInfo(i,&l,&s);
			str.Format(_T("Level%u:Stage%u\nStep Count:%u"),l,s,m_SqMapSet.GetStepCount(i));
			m_StaticDesc.SetWindowText(str);
		}
		else if(m_FileTree.GetParentItem(tic)==m_htiBg)
		{
			m_SqMapSet.GetBgName(m_FileTree.GetItemData(tic),strn);
			str.Format(_T("Background:")FORMAT_A2T,strn);
			m_StaticDesc.SetWindowText(str);
			PaintBgPrvw(m_FileTree.GetItemData(tic));
		}
		else if(m_FileTree.GetParentItem(tic)==m_htiGl)
		{
			m_SqMapSet.GetGlName(m_FileTree.GetItemData(tic),strn);
			str.Format(_T("Texture:")FORMAT_A2T,strn);
			m_StaticDesc.SetWindowText(str);
			PaintGlPrvw(m_FileTree.GetItemData(tic));
		}
		else if(m_FileTree.GetParentItem(tic)==m_htiPl)
		{
			m_SqMapSet.GetPlName(m_FileTree.GetItemData(tic),strn);
			str.Format(_T("Palatte:")FORMAT_A2T
				_T("\nType=%d"),strn,*m_SqMapSet.GetPlBuffer(m_FileTree.GetItemData(tic),0));
			m_StaticDesc.SetWindowText(str);
			PaintPlPrvw(m_FileTree.GetItemData(tic));
		}
		else
		{
			HTREEITEM htiStage=m_FileTree.GetParentItem(tic);
			if(m_FileTree.GetParentItem(htiStage)==m_htiMap)
			{
				u32 i=0;
				u8 l,s;
				for(;i<(u32)m_htiMapi.GetCount();++i)if(m_htiMapi[i]==htiStage)break;
				m_SqMapSet.GetStageInfo(i,&l,&s);
				str.Format(_T("Level%u:Stage%u:Step%02u"),l,s,m_FileTree.GetItemData(tic));
				m_StaticDesc.SetWindowText(str);
				PaintStepPrvw(i,(u16)m_FileTree.GetItemData(tic));
			}
		}
	}

	*pResult=0;
}


void CMainFrame::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(nIDCtl==ID_STATIC_PRVW)
	{
		BitBlt(lpDrawItemStruct->hDC,0,0,BMP_PRVW_W,BMP_PRVW_H,m_DCPrvw.GetSafeHdc(),0,0,SRCCOPY);
	}
	//CFrameWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
