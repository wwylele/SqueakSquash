

#include "stdafx.h"
#include "Main.h"
#include "MainFrm.h"
#include "DlgMakeRom.h"
#include "DlgRename.h"
#include "DlgChangeLib.h"
#include "WndWait.h"

#include "SqB.h"
#include "SqPl1.h"
#include "SqMa.h"
#include "SqDe.h"
#include "Det.h"

#include <list>

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
	ON_COMMAND(ID_MAKE,&CMainFrame::OnTbMake)
	ON_COMMAND(ID_TESTGAME,&CMainFrame::OnTbTestGame)
	ON_COMMAND(ID_BUTTON_OPTION,&CMainFrame::OnButtonOption)
	ON_WM_SIZE()
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, &CMainFrame::OnTtnNeedText)
	ON_NOTIFY(TVN_SELCHANGED, ID_FILETREE, &CMainFrame::OnFileTreeSelChanged)
	ON_WM_DRAWITEM()
	ON_COMMAND(ID_BOPTM_RENAME,&CMainFrame::OnBoptmRename)
	ON_COMMAND(ID_BOPTM_CHANGELIB,&CMainFrame::OnBoptmChangeLib)
	ON_COMMAND(ID_BOPTM_DELETE,&CMainFrame::OnBoptmDelete)
	ON_COMMAND(ID_BOPTM_COPY,&CMainFrame::OnBoptmCopy)
END_MESSAGE_MAP()

void CMainFrame::OnTbTestGame()
{
	PrintLog("OnTbTestGame==>\n");
	if(!m_SqMapSet.IsLoaded())return;
	SqMa sqma;
	u32 mxpl;
	u8* mxpb;
	//bool used[256];
	//for(int i=0;i<256;++i)used[i]=false;
	CList<u32> list;
	u32 i;
	u16 j;
	u8 l,s;

	for(i=0;i<m_SqMapSet.GetStageCount();++i)
	{
		m_SqMapSet.GetStageInfo(i,&l,&s);
		for(j=0;j<m_SqMapSet.GetStepCount(i);++j)
		{
			mxpb=m_SqMapSet.GetMxpBuffer(i,j,&mxpl);
			if(mxpl!=16)
			{
#define DWORD_SR(a) ((u32)((a)>>16)|((a)<<16))
				sqma.Load(mxpb);

				//PrintLog("%d\n",sqma.s9exl);
				/*if(sqma.Section10()[2]==0)
				{
					for(int k=2;k<10;++k)
					if(sqma.Section10()[k])printf("[Lv%d-St%d]-%d\n",l,s,j);
				}*/
				//else if(sqma.Section10()[2]!=8)printf("[Lv%d-St%d]-%d\n",l,s,j);
				if(sqma.Section10()[2]==8)printf("%d-%d-%02d:%02X %02X %02X %02X %02X %02X %02X %02X\n",l,s,j,
					sqma.Section10()[2],sqma.Section10()[3],sqma.Section10()[4],sqma.Section10()[5],
					sqma.Section10()[6],sqma.Section10()[7],sqma.Section10()[8],sqma.Section10()[9]);

				

				/*for(u8 x=0;x<sqma.GetW();++x)for(u8 y=0;y<sqma.GetH();++y)
				{
					//if(!list.Find(DWORD_SR(sqma.Grid(x,y).det[0])))
					//	list.AddHead(DWORD_SR(sqma.Grid(x,y).det[0]));
					//ASSERT(GetDet0Name(sqma.Grid(x,y).det[0]));
				}*/
				
				
				
				/*for(u16 x=0;x<sqma.GetDoorCount();++x)
				{
					u32 d1,d2;
					if(sqma.Door(x).dt[0]!=x){
						printf("[Lv%d-St%d]-%d\n",l,s,j);
						printf("[%02d]%02X,%02X,%02X,%02X,%04X,%04X,%02X,%02X\n",x,
						sqma.Door(x).dt[0],sqma.Door(x).dt[1],sqma.Door(x).dt[2],sqma.Door(x).dt[3],
						sqma.Door(x).x,sqma.Door(x).y,sqma.Door(x).dst_step,sqma.Door(x).unk);
					}
				}*/
			}
		}
	}


outside:
	/*CStringA ofn;
	u8 l,s;
	m_SqMapSet.GetStageInfo(i,&l,&s);
	ofn.Format("a%ds%ds%d.det.txt",l,s,j);
	PrintLog(ofn);
	FILE *pf=fopen(ofn,"wt");
	for(u8 y=0;y<sqma.GetH();++y){for(u8 x=0;x<sqma.GetW();++x)
	{
		fprintf(pf,"%08X|",sqma.Grid(x,y).det[1]);
	}fprintf(pf,"\n");}
	fclose(pf);*/

	/*POSITION pos=list.GetHeadPosition();
	std::list<u32> alist;
	while(pos)
	{
		alist.push_front(list.GetNext(pos));
	}
	alist.sort();
	std::list<u32>::iterator iter=alist.begin();
	FILE *pf=fopen("det0.txt","wt");
	for(;iter!=alist.end();iter++)
	{
		fprintf(pf,"%04X %04X\n",*iter>>16,*iter & 0xFFFF);
	}
	fclose(pf);*/
	PrintLog("<==OnTbTestGame\n");
}


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
	m_ButtonOption.Create(_T("Option >>>"),WS_CHILD|WS_VISIBLE,ZeroRect,this,ID_BUTTON_OPTION);

	BMP_PRVW_W=GetSystemMetrics(SM_CXFULLSCREEN)-250;
	BMP_PRVW_H=GetSystemMetrics(SM_CYFULLSCREEN)-100;

	CDC *pDC=GetDC();
	m_DCPrvw.CreateCompatibleDC(pDC);
	m_BmpPrvw.CreateCompatibleBitmap(pDC,BMP_PRVW_W,BMP_PRVW_H);
	m_DCPrvw.SelectObject(&m_BmpPrvw);
	m_DCPrvw.FillRect((LPRECT)&CRect(0,0,BMP_PRVW_W,BMP_PRVW_H),&CBrush((COLORREF)0));

	ReleaseDC(pDC);

	m_BmpDet0.LoadBitmap(IDB_DET0);
	//m_BrushDet0.CreatePatternBrush(&m_BmpDet0);

	m_TreeSelCls=FTS_0;
	m_TreeSelIdx=0;

	CWndWait::InitWndWait();
	
	return 0;
}
void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	m_FileTree.MoveWindow(0,30,200,cy-35);
	m_StaticDesc.MoveWindow(202,60,600,40);
	m_StaticPrvw.MoveWindow(202,100,cx-220,cy-110);
	m_ButtonOption.MoveWindow(202,30,80,30);
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
	
	CFileDialog filedlg(TRUE,0,0,0,
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
		CWndWait::ShowWndWait();
		if(!m_SqMapSet.LoadFromRom(file))
		{
			CWndWait::HideWndWait();
			MessageBox(_T("Fail to load the file"),_T("Error"),MB_ICONERROR);
			file.Close();
			return;
		}
		else
		{
			CWndWait::HideWndWait();
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

	PrintLog("SubFileCount=%u\n",m_SqMapSet.GetSubFileCount());

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

	m_TreeSelCls=FTS_0;
	m_TreeSelIdx=0;
}

void CMainFrame::PaintStepPrvw(u32 Stage,u16 Step)
{
	m_DCPrvw.FillRect((LPRECT)&CRect(0,0,BMP_PRVW_W,BMP_PRVW_H),&CBrush((COLORREF)0));

	SqMa sqma;
	SqDe sqde;
	SqB bb,fb;
	u8 bbi,fbi;
	u32 malen;
	u8* maend;
	m_SqMapSet.GetStepInfo(Stage,Step,0,&bbi,&fbi,0);
	sqma.Load(m_SqMapSet.GetMxpBuffer(Stage,Step,&malen));
	maend=m_SqMapSet.GetMxpBuffer(Stage,Step)+malen;
	sqde.Load(m_SqMapSet.GetDoeBuffer(Stage,Step));
	bb.Load(m_SqMapSet.GetGlBuffer(bbi,0));
	fb.Load(m_SqMapSet.GetGlBuffer(fbi,0));

	u16 dw=sqma.GetW()<<4,dh=sqma.GetH()<<4;

	CDC TempDC,*pDC=GetDC(),det0DC;
	CBitmap TempBmp;
	TempDC.CreateCompatibleDC(pDC);
	TempBmp.CreateCompatibleBitmap(pDC,dw,dh);
	TempDC.SelectObject(&TempBmp);
	det0DC.CreateCompatibleDC(pDC);
	det0DC.SelectObject(&m_BmpDet0);
	ReleaseDC(pDC);

	

	//S9
	/*PrintLog("S9");
	for(u32 i=0;i<sqma.s9exl;++i)
	{
		if(i%16==0)PrintLog("\n");
		PrintLog("%02X ",sqma.s9exp[1]);
		
	}
	PrintLog("\n");
	PrintLog("Len=%d\n",sqma.s9exl);

	*/

	//Section10
	u8 *ps10=sqma.Section10();
	u16 s10len=*(u16*)ps10;
	PrintLog("S10:\n");
	for(u16 i=2;i<s10len;++i)
	{
		PrintLog("%02X ",ps10[i]);
		if(!((i-1)&15))PrintLog("\n");
	}
	PrintLog("\n");

	
	//Section11(Door)
	/*PrintLog("S11:\n");
	for(u16 i=0;i<sqma.GetDoorCount();++i)
	{
		for(u16 j=0;j<10;++j)
		{
			PrintLog("%02X ",sqma.Door(i).dt[j]);
		}
		PrintLog("\n");
	}
	PrintLog("\n");*/

	/*//Section12
	PrintLog("S12:\nHeader:");
	PrintLog("%02X ",sqma.S12Header.x[0]);
	PrintLog("%02X ",sqma.S12Header.x[1]);
	PrintLog("%02X ",sqma.S12Header.x[2]);
	PrintLog("%02X ",sqma.S12Header.x[3]);
	PrintLog("%02X ",sqma.S12Header.x[4]);
	PrintLog("%02X ",sqma.S12Header.x[5]);
	PrintLog("%02X ",sqma.S12Header.t[0]);
	PrintLog("%02X ",sqma.S12Header.t[1]);
	PrintLog("%02X ",sqma.S12Header.t[2]);
	PrintLog("%02X ",sqma.S12Header.t[3]);
	PrintLog("\nData:\n");
	for(u16 i=0;i<sqma.S12Header.GraScriptCount;++i)
	{
		for(int j=0;j<12;j++){
			PrintLog("%02X ",sqma.pGraScript[i].x[j]);
		}
		PrintLog("\n");
	}
	PrintLog("\n");

	//CStringA ofn;
	//u8 l,s;
	//m_SqMapSet.GetStageInfo(Stage,&l,&s);
	//ofn.Format("a%ds%ds%d.det.txt",l,s,Step);
	//FILE *pf=fopen(ofn,"wt");*/

	PrintLog("Size=%d * %d\n",sqma.GetW(),sqma.GetH());

	const DetDesc* ddsc;
	for(u8 y=0;y<sqma.GetH();++y){for(u8 x=0;x<sqma.GetW();++x)if(sqma.Grid(x,y).det[0])
	{
		ddsc=GetDet0Desc(sqma.Grid(x,y).det[0]);
		//TempDC.SetBrushOrg((ddsc->tex_x<<4),(ddsc->tex_y<<4));
		//TempDC.FillRect((LPRECT)&CRect((x<<4),(y<<4),(x<<4)+16,(y<<4)+16),&m_BrushDet0);
		TempDC.BitBlt((x<<4),(y<<4),16,16,&det0DC,(ddsc->tex_x<<4),(ddsc->tex_y<<4),SRCCOPY);
	}
	else
	{
		//fprintf(pf,"%08X|",sqma.Grid(x,y).det[1]);
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
		if(sqma.Grid(x,y).t)
		{
			CString str;
			str.Format(_T("%02X"),sqma.Grid(x,y).t);
			TempDC.TextOut(x<<4,y<<4,str,2);
		}
	}/*fprintf(pf,"\n");*/}

	//fclose(pf);

	for(u16 i=0;i<sqde.GetFoeCount();++i)
	{
		TempDC.Rectangle(sqde.Foe(i).x-8,sqde.Foe(i).y-8,
			sqde.Foe(i).x+8,sqde.Foe(i).y+8);
	}
	for(u16 i=0;i<sqde.GetSupCount();++i)
	{
		TempDC.Ellipse(sqde.Sup(i).x-8,sqde.Sup(i).y-8,
			sqde.Sup(i).x+8,sqde.Sup(i).y+8);
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
	det0DC.DeleteDC();
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
void CMainFrame::OnFileTreeSelChanged(NMHDR *pNMHDR, LRESULT *pResult)
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
			m_TreeSelCls=FTS_ROM;
			m_TreeSelIdx=0;
		}
		else if(tic==m_htiMap)
		{
			str.Format(_T("Stage Count:%u"),m_SqMapSet.GetStageCount());
			m_StaticDesc.SetWindowText(str);
			m_TreeSelCls=FTS_MAP;
			m_TreeSelIdx=0;
		}
		else if(tic==m_htiBg)
		{
			str.Format(_T("Background Count:%u"),m_SqMapSet.GetBgCount());
			m_StaticDesc.SetWindowText(str);
			m_TreeSelCls=FTS_BGF;
			m_TreeSelIdx=0;
		}
		else if(tic==m_htiGl)
		{
			str.Format(_T("Texture Count:%u"),m_SqMapSet.GetGlCount());
			m_StaticDesc.SetWindowText(str);
			m_TreeSelCls=FTS_GLF;
			m_TreeSelIdx=0;
		}
		else if(tic==m_htiPl)
		{
			str.Format(_T("Palatte Count:%u"),m_SqMapSet.GetPlCount());
			m_StaticDesc.SetWindowText(str);
			m_TreeSelCls=FTS_PLF;
			m_TreeSelIdx=0;
		}
		else if(m_FileTree.GetParentItem(tic)==m_htiMap)
		{
			u32 i=0;
			u8 l,s;
			for(;i<(u32)m_htiMapi.GetCount();++i)if(m_htiMapi[i]==tic)break;
			m_SqMapSet.GetStageInfo(i,&l,&s);
			str.Format(_T("Level%u:Stage%u\nStep Count:%u"),l,s,m_SqMapSet.GetStepCount(i));
			m_StaticDesc.SetWindowText(str);
			m_TreeSelCls=FTS_STAGE;
			m_TreeSelIdx=i;
		}
		else if(m_FileTree.GetParentItem(tic)==m_htiBg)
		{
			m_SqMapSet.GetBgName(m_FileTree.GetItemData(tic),strn);
			str.Format(_T("Background:")FORMAT_A2T,strn);
			m_StaticDesc.SetWindowText(str);
			PaintBgPrvw(m_FileTree.GetItemData(tic));
			m_TreeSelCls=FTS_BG;
			m_TreeSelIdx=m_FileTree.GetItemData(tic);
		}
		else if(m_FileTree.GetParentItem(tic)==m_htiGl)
		{
			m_SqMapSet.GetGlName(m_FileTree.GetItemData(tic),strn);
			str.Format(_T("Texture:")FORMAT_A2T,strn);
			m_StaticDesc.SetWindowText(str);
			PaintGlPrvw(m_FileTree.GetItemData(tic));
			m_TreeSelCls=FTS_GL;
			m_TreeSelIdx=m_FileTree.GetItemData(tic);
		}
		else if(m_FileTree.GetParentItem(tic)==m_htiPl)
		{
			m_SqMapSet.GetPlName(m_FileTree.GetItemData(tic),strn);
			str.Format(_T("Palatte:")FORMAT_A2T
				_T("\nType=%d"),strn,*m_SqMapSet.GetPlBuffer(m_FileTree.GetItemData(tic),0));
			m_StaticDesc.SetWindowText(str);
			PaintPlPrvw(m_FileTree.GetItemData(tic));
			m_TreeSelCls=FTS_PL;
			m_TreeSelIdx=m_FileTree.GetItemData(tic);
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
				str.Format(_T("Level%u:Stage%u:Step%02u,[%u*%u]"),l,s,m_FileTree.GetItemData(tic));
				m_StaticDesc.SetWindowText(str);
				PaintStepPrvw(i,(u16)m_FileTree.GetItemData(tic));
				m_TreeSelCls=FTS_STEP;
				m_TreeSelIdx=i;
				m_TreeSelIdx2=m_FileTree.GetItemData(tic);
			}
		}
	}

	if(pResult)*pResult=0;
}


void CMainFrame::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(nIDCtl==ID_STATIC_PRVW)
	{
		BitBlt(lpDrawItemStruct->hDC,0,0,BMP_PRVW_W,BMP_PRVW_H,m_DCPrvw.GetSafeHdc(),0,0,SRCCOPY);
	}
	//CFrameWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


void CMainFrame::OnTbMake()
{
	if(!m_SqMapSet.IsLoaded())return;
	CDlgMakeRom dlg;
	dlg.m_pMapSet=&m_SqMapSet;
	dlg.DoModal();
}

void CMainFrame::OnButtonOption()
{
	if(!m_SqMapSet.IsLoaded())return;
	CMenu menu;
	menu.CreatePopupMenu();
	CString str;
	u8 l,s;
	switch(m_TreeSelCls)
	{
	case FTS_ROM:
		menu.AppendMenu(MF_GRAYED,0,_T(">>>ROM"));
		menu.AppendMenu(0,ID_BOPTM_ROM,_T("Edit Game Title"));
		break;
	case FTS_MAP:
		menu.AppendMenu(MF_GRAYED,0,_T(">>>Map"));
		break;
	case FTS_BGF:
		menu.AppendMenu(MF_GRAYED,0,_T(">>>Background Library"));
		break;
	case FTS_GLF:
		menu.AppendMenu(MF_GRAYED,0,_T(">>>Texture Library"));
		break;
	case FTS_PLF:
		menu.AppendMenu(MF_GRAYED,0,_T(">>>Palette Library"));
		break;
	case FTS_STAGE:
		m_SqMapSet.GetStageInfo(m_TreeSelIdx,&l,&s);
		str.Format(_T(">>>Stage %d-%d"),l,s);
		menu.AppendMenu(MF_GRAYED,0,str);
		menu.AppendMenu(0,ID_BOPTM_ADDSTEP,_T("Add Step"));
		break;
	case FTS_STEP:
		m_SqMapSet.GetStageInfo(m_TreeSelIdx,&l,&s);
		str.Format(_T(">>>Step %d-%d-%02d"),l,s,m_TreeSelIdx2);
		menu.AppendMenu(MF_GRAYED,0,str);
		menu.AppendMenu(0,ID_BOPTM_EDIT,_T("Edit map"));
		menu.AppendMenu(0,ID_BOPTM_CHANGELIB,_T("Change library"));
		menu.AppendMenu(0,ID_BOPTM_TEXMAP,_T("Edit texture mapping"));
		menu.AppendMenu(0,ID_BOPTM_MOVE,_T("Move step"));
		menu.AppendMenu(0,ID_BOPTM_COPY,_T("Copy step"));
		menu.AppendMenu(0,ID_BOPTM_DELETE,_T("Delete step"));
		break;
	case FTS_BG:case FTS_GL:case FTS_PL:
		str.Format(_T(">>>%s %d"),
			m_TreeSelCls==FTS_BG?_T("Background"):
			(m_TreeSelCls==FTS_GL?_T("Texture"):_T("Palette")),m_TreeSelIdx);
		menu.AppendMenu(MF_GRAYED,0,str);
		menu.AppendMenu(0,ID_BOPTM_EDIT,_T("Edit"));
		menu.AppendMenu(0,ID_BOPTM_RENAME,_T("Rename"));
		menu.AppendMenu(0,ID_BOPTM_COPY,_T("Copy"));
		menu.AppendMenu(0,ID_BOPTM_DELETE,_T("Delete"));
		break;
	default:case FTS_0:
		menu.AppendMenu(MF_GRAYED,0,_T("Not select any item in the file tree"));
		break;
	}

	RECT brect;
	m_ButtonOption.GetWindowRect(&brect);
	menu.TrackPopupMenu(0,brect.right,brect.top,this);
	menu.DestroyMenu();
}

void CMainFrame::OnBoptmRename()
{
	ASSERT(m_SqMapSet.IsLoaded());
	CDlgRename dlg;

	//Get the old name
	char strn[16];
	switch(m_TreeSelCls)
	{
	case FTS_BG:
		m_SqMapSet.GetBgName(m_TreeSelIdx,strn);
		break;
	case FTS_GL:
		m_SqMapSet.GetGlName(m_TreeSelIdx,strn);
		break;
	case FTS_PL:
		m_SqMapSet.GetPlName(m_TreeSelIdx,strn);
		break;
	default:
		ASSERT(FALSE);
		return;
	}
	dlg.m_NameIO.Format(FORMAT_A2T,strn);

	bool suc;
	while(1)
	{
		if(dlg.DoModal()!=IDOK)return;
		ZeroMemory(strn,16);
		suc=true;
		//CString to char[16]
		for(int i=0;dlg.m_NameIO[i];++i)
		{
			//because the textbox has the length limit so this "if{}" is just an ensurence
			if(i==15)
			{
				ASSERT(FALSE);
				MessageBox(_T("Too many characters."));
				suc=false;
				break;
			}
			if(dlg.m_NameIO[i]<0x21 || dlg.m_NameIO[i]>0x7E)
			{
				MessageBox(_T("Invalid character."));
				suc=false;
				break;
			}
			strn[i]=(char)dlg.m_NameIO[i];
		}
		//ensure there is no name conflict
		if(m_SqMapSet.FindSecitem(0,strn)!=0xFFFFFFFF)
		{
			MessageBox(_T("Name conflict."));
			suc=false;
		}
		//if no problem then break the loop and copy the name to SqMapSet
		if(suc)break;
	}
	
	HTREEITEM htiEntry;
	switch(m_TreeSelCls)
	{
	case FTS_BG:
		m_SqMapSet.SetBgName(m_TreeSelIdx,strn);
		htiEntry=m_htiBg;
		break;
	case FTS_GL:
		m_SqMapSet.SetGlName(m_TreeSelIdx,strn);
		htiEntry=m_htiGl;
		break;
	case FTS_PL:
		m_SqMapSet.SetPlName(m_TreeSelIdx,strn);
		htiEntry=m_htiPl;
		break;
	}
	//copy the name to the file tree
	htiEntry=m_FileTree.GetChildItem(htiEntry);
	while(m_FileTree.GetItemData(htiEntry)!=m_TreeSelIdx)
		htiEntry=m_FileTree.GetNextItem(htiEntry,TVGN_NEXT);
	m_FileTree.SetItemText(htiEntry,dlg.m_NameIO);
	OnFileTreeSelChanged(0,0);

}
void CMainFrame::OnBoptmChangeLib()
{
	ASSERT(m_SqMapSet.IsLoaded());
	ASSERT(m_TreeSelCls==FTS_STEP);
	CDlgChangeLib dlg;
	dlg.pMapSet=&m_SqMapSet;
	m_SqMapSet.GetStepInfo(m_TreeSelIdx,(u16)m_TreeSelIdx2,
		&dlg.Bg,&dlg.BGl,&dlg.FGl,&dlg.Pl);
	if(dlg.DoModal()==IDOK)
	{
		m_SqMapSet.SetStepInfo(m_TreeSelIdx,(u16)m_TreeSelIdx2,
			dlg.Bg,dlg.BGl,dlg.FGl,dlg.Pl);
		OnFileTreeSelChanged(0,0);
	}
}

void CMainFrame::OnBoptmDelete()
{
	ASSERT(m_SqMapSet.IsLoaded());
	CString str;

	//delete step:
	if(m_TreeSelCls==FTS_STEP)
	{
		if(m_SqMapSet.GetStepCount(m_TreeSelIdx)==1)
		{
			MessageBox(_T("There must be at least one step in a stage"),0,MB_ICONERROR);
			return;
		}
		str.Format(_T("Are you sure to delete the step %d ?"),m_TreeSelIdx2);
		if(MessageBox(str,_T("Warning"),MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON2)!=IDYES)return;
		//delete in SqMapSet
		m_SqMapSet.DeleteStep(m_TreeSelIdx,(u16)m_TreeSelIdx2);
		//delete in file tree
		HTREEITEM htiEntry=m_FileTree.GetChildItem(m_htiMapi[m_TreeSelIdx]);
		while(m_FileTree.GetItemData(htiEntry)!=m_SqMapSet.GetStepCount(m_TreeSelIdx))
			htiEntry=m_FileTree.GetNextItem(htiEntry,TVGN_NEXT);
		m_FileTree.DeleteItem(htiEntry);
		OnFileTreeSelChanged(0,0);
		return;
	}

	//delete secitem:
	u8 SiSwitch;
	HTREEITEM htiEntry,htiDel;
	switch(m_TreeSelCls)
	{
	case FTS_BG:SiSwitch=0;htiEntry=m_htiBg;
		break;
	case FTS_GL:SiSwitch=1;htiEntry=m_htiGl;
		break;
	case FTS_PL:SiSwitch=2;htiEntry=m_htiPl;
		break;
	default:ASSERT(FALSE);
		return;
	}
	if(m_SqMapSet.GetSecitemCount(SiSwitch)==1)
	{
		MessageBox(_T("There must be at least one file"),0,MB_ICONERROR);
		return;
	}
	
	char strn[16];
	m_SqMapSet.GetSecitemName(SiSwitch,m_TreeSelIdx,strn);
	str.Format(_T("Are you sure to delete \"")FORMAT_A2T _T("\"?"),strn);
	if(MessageBox(str,_T("Warning"),MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON2)!=IDYES)return;
	//delete in SqMapSet
	m_SqMapSet.DeleteSecitem(SiSwitch,m_TreeSelIdx);
	//delete in file tree
	htiEntry=m_FileTree.GetChildItem(htiEntry);
	u32 itemdt;
	do{
		itemdt=m_FileTree.GetItemData(htiEntry);
		if(itemdt>m_TreeSelIdx)
			m_FileTree.SetItemData(htiEntry,itemdt-1);
		else if(itemdt==m_TreeSelIdx)htiDel=htiEntry;
		
	}while(htiEntry=m_FileTree.GetNextItem(htiEntry,TVGN_NEXT));
	m_FileTree.DeleteItem(htiDel);


}
void CMainFrame::OnBoptmCopy()
{
	ASSERT(m_SqMapSet.IsLoaded());
	if(m_TreeSelCls==FTS_STEP)
	{
		MessageBox(_T("Unfinished code."));
		//
		return;
	}
	u8 SiSwitch;
	HTREEITEM htiEntry;
	switch(m_TreeSelCls)
	{
	case FTS_BG:SiSwitch=0;htiEntry=m_htiBg;
		break;
	case FTS_GL:SiSwitch=1;htiEntry=m_htiGl;
		break;
	case FTS_PL:SiSwitch=2;htiEntry=m_htiPl;
		break;
	default:ASSERT(FALSE);
		return;
	}

	//generate a new name
	char strn[16];int ni=1;
	do{
		sprintf(strn,"new%d",ni++);
	}while(m_SqMapSet.FindSecitem(0,strn)!=0xFFFFFFFF);

	//new secitem in SqMapSet and copy the data
	u32 datalen;
	u8* pdata=m_SqMapSet.GetSecitemBuffer(SiSwitch,m_TreeSelIdx,&datalen);
	memcpy(m_SqMapSet.NewSecitem(SiSwitch,datalen,strn),pdata,datalen);

	//new secitem in file tree
	CString str;
	str.Format(FORMAT_A2T,strn);
	m_FileTree.SelectItem(m_FileTree.InsertItem(TVIF_TEXT|TVIF_PARAM,str,0,0,0,0,
		m_SqMapSet.GetSecitemCount(SiSwitch)-1,htiEntry,TVI_LAST));
}