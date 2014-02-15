

//UTF16


#include "stdafx.h"
#include "Main.h"
#include "MainFrm.h"
#include "DlgMakeRom.h"
#include "DlgChangeLib.h"
#include "DlgMapEdit.h"
#include "DlgTexMapping.h"
#include "DlgImportBg.h"
#include "DlgRomInfo.h"
#include "WndWait.h"

#include "SqB.h"
#include "SqPl1.h"
#include "SqMa.h"
#include "Evc.h"
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
	ON_COMMAND(ID_ABOUT,&CMainFrame::OnAbout)
	ON_COMMAND(ID_BUTTON_OPERATION,&CMainFrame::OnButtonOperation)
	ON_WM_SIZE()
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, &CMainFrame::OnTtnNeedText)
	ON_NOTIFY(TVN_SELCHANGED, ID_FILETREE, &CMainFrame::OnFileTreeSelChanged)
	ON_NOTIFY(NM_DBLCLK, ID_FILETREE,&CMainFrame::OnFileTreeDblclk)
	ON_NOTIFY(NM_RCLICK, ID_FILETREE,&CMainFrame::OnFileTreeRclk)
	ON_NOTIFY(TVN_BEGINLABELEDIT, ID_FILETREE,&CMainFrame::OnFileTreeBeginLabelEdit)
	ON_NOTIFY(TVN_ENDLABELEDIT,ID_FILETREE,&CMainFrame::OnFileTreeEndLabelEdit)
	ON_WM_DRAWITEM()
	ON_COMMAND(ID_BOPTM_RENAME,&CMainFrame::OnBoptmRename)
	ON_COMMAND(ID_BOPTM_CHANGELIB,&CMainFrame::OnBoptmChangeLib)
	ON_COMMAND(ID_BOPTM_DELETE,&CMainFrame::OnBoptmDelete)
	ON_COMMAND(ID_BOPTM_COPY,&CMainFrame::OnBoptmCopy)
	ON_COMMAND(ID_BOPTM_EDIT,&CMainFrame::OnBoptmEdit)
	ON_COMMAND(ID_BOPTM_EXPORT,&CMainFrame::OnBoptmExport)
	ON_COMMAND(ID_BOPTM_IMPORT,&CMainFrame::OnBoptmImport)
	ON_COMMAND(ID_BOPTM_NEW,&CMainFrame::OnBoptmNew)
	ON_COMMAND(ID_BOPTM_ROM,&CMainFrame::OnBoptmRom)
	ON_COMMAND(ID_BOPTM_TEXMAP,&CMainFrame::OnBoptmTexMap)
END_MESSAGE_MAP()
void CMainFrame::OnAbout()
{
	CDlgAbout dlg;
	dlg.DoModal();
}
void CMainFrame::OnTbTestGame()
{
	//ASSERT(FALSE);
#ifdef _DEBUG
	PrintLog("OnTbTestGame==>\n");
	if(!m_SqMapSet.IsLoaded())return;
	SqMa sqma;
	SqDe sqde;
	SqMctrlPack sqep;
	u32 mxpl,mxpld;
	u8* mxpb,*mxpbd;
	u16 *doeb;
	//bool used[256];
	//for(int i=0;i<256;++i)used[i]=false;
	CList<u16> list;
	u32 i;
	u16 j;
	u8 l,s;

	/*struct BGH
	{
		u8 data[0xA4];
	};
	BGH BGHT;
	std::list<BGH> BGHL;
	bool b;
	for(i=0;i<m_SqMapSet.GetBgCount();++i)
	{
		BGHT=*(BGH*)m_SqMapSet.GetBgBuffer(i,0);
		ZeroMemory(BGHT.data+0x6C,0x10);
		ZeroMemory(BGHT.data+0x94,0x10);
		b=true;
		for(std::list<BGH>::iterator iter=BGHL.begin();iter!=BGHL.end();iter++)
		{
			if(memcmp(&BGHT,&*iter,0xA4)==0)
			{
				b=false;
				break;
			}
		}
		if(b)
		{
			BGHL.push_front(BGHT);
			char bname[17]={0};
			m_SqMapSet.GetBgName(i,bname);
			puts(bname);
		}
		
	}*/
	
	/*i=0;
	for(std::list<BGH>::iterator iter=BGHL.begin();iter!=BGHL.end();iter++)
	{
		char filename[100];
		sprintf(filename,"nsbtx_temp_%d",i);
		FILE* pf=fopen(filename,"wb");
		fwrite(&*iter,0xA4,1,pf);
		fprintf(pf,"\n");
		fclose(pf);
		++i;
	}*/
	
	/*m_SqMapSet.ResizeDoeBuffer(0,0,16+16*10);
	doeb=(u16*)m_SqMapSet.GetDoeBuffer(0,0);
	*(doeb++)=0x6544;
	*(doeb++)=0xC;
	*(doeb++)=0xC;
	*(doeb++)=0xE;
	*(doeb++)=0;
	*(doeb++)=1;
	*(doeb++)=0;
	*(doeb++)=10;
	for(int i=0;i<10;i++)
	{
		*(doeb++)=i;
		*(doeb++)=i+8;
		*(doeb++)=i*3*16+96;
		*(doeb++)=64;
		*(doeb++)=0;
		*(doeb++)=0;
		*(doeb++)=0;
		*(doeb++)=0;
	}*/
	

	u32 bossc[256];
	for(i=0;i<256;++i)bossc[i]=0xFFFFFFFF;


	int evce[100]={0};
	//FILE* pf=fopen("bossclist_dc.txt","wt");
	for(i=0;i<m_SqMapSet.GetStageCount();++i)
	{
		m_SqMapSet.GetStageEntry(i,&l,&s);
		for(j=0;j<m_SqMapSet.GetStepCount(i);++j)
		{
			mxpb=m_SqMapSet.GetMxpBuffer(i,j,&mxpl);
			doeb=(u16*)m_SqMapSet.GetDoeBuffer(i,j);
			if(mxpl!=16)
			{
				sqma.Load(mxpb);
				sqde.Load((u8*)doeb);
				//sqep.FromMa(sqma);
				//sqep.ToMa(sqma);

				/*fprintf(pf,"\n\n[%2d-%2d-%02d]\n",l,s,j);
				if(sqma.GetGuideCount())
				{
					for(u16 i=0;i<sqma.GetGuideCount();++i)
					{
						fprintf(pf,"[%04X] %04X %04X %04X %04X %c\n",
							i,sqma.Guide(i).id,sqma.Guide(i).x,
							sqma.Guide(i).y,sqma.Guide(i).U,
							i==sqma.Guide(i).id?' ':'!');
						
					}
					for(u16 i=0;
						i<sqma.GetGuideCount()*sqma.GetGuideCount();
						++i)
					{
						if(i%sqma.GetGuideCount()==0)fprintf(pf,"\n");
						fprintf(pf,"%02X ",sqma.GuideMatrix(i));
					}
				}*/
				

				//if(sqma.s9exl>1)
				//{
					//u8 maxboss;
					//maxboss=0;
					//for(u8 x=0;x<sqma.GetW();++x)for(u8 y=0;y<sqma.GetH();++y)
					//{
					//	if(maxboss<sqma.Cell(x,y).t)maxboss=sqma.Cell(x,y).t;
					//}
				/*int pad;
				pad=sqma.GetW()*sqma.GetH()%2;
					ASSERT(sqma.s9exl%2==pad);
					if(bossc[sqma.s9exp[pad]]==0xFFFFFFFF)
					{
						bossc[sqma.s9exp[pad]]=sqma.s9exl-pad;
					}
					else ASSERT(bossc[sqma.s9exp[pad]]==sqma.s9exl-pad);
				ASSERT(sqma.s9exp[pad]!=253);*/
						
					
				//}
				for(u8 fi=0;fi<sqde.GetSupCount();++fi)
				{
					if(sqde.Sup(fi).class_id==7)PrintLog("%d-%d-%d\n",l,s,j);
				}
				/*for(u16 i=0;i<sqde.GetSupCount();i++)
				{
					if(!list.Find(sqde.Sup(i).class_id))
						list.AddHead(sqde.Sup(i).class_id);
				}*/

				sqma.MapAttribute.x0A=0;
				mxpld=sqma.MakeLen();
				mxpbd=new u8[mxpld];
				sqma.Make(mxpbd);
				if(mxpld!=mxpl||memcmp(mxpb,mxpbd,mxpl)!=0)
				{
					PrintLog("%d-%d-%d!!!\n",l,s,j);
				}
				delete[] mxpbd;
				m_SqMapSet.ResizeMxpBuffer(i,j,sqma.MakeLen());
				sqma.Make(m_SqMapSet.GetMxpBuffer(i,j));

				/*if(*(u16*)(sqma.Section10()+2))
				{
					u8 count2,sc;
					u8* org2,*o;
					ASSERT(*(u16*)(sqma.Section10()+2)==8);
					ASSERT(*(u16*)(sqma.Section10()+6)==0);
					ASSERT(*(u16*)(sqma.Section10()+4)==
						*(sqma.Section10()+8)*8+8);
					ASSERT(*(sqma.Section10()+8));
					ASSERT(count2=*(sqma.Section10()+9));
					
					o=org2=sqma.Section10()+2+*(u16*)(sqma.Section10()+4);
					o+=count2*2;
					for(u8 i=0;i<count2;++i)
					{
						ASSERT(o==sqma.Section10()+2+((u16*)org2)[i]);
						o+=1;
						sc=*(o++);
						ASSERT(sc);
						ASSERT(*(u16*)o);
						o+=2+sc**(u16*)o;
					}
					ASSERT(o-sqma.Section10()==*(u16*)sqma.Section10());
				}
				//if(sqma.GetTexMappingACount()%32 ||
				//	sqma.GetTexMappingBCount()%64)ASSERT(FALSE);
			/*fprintf(pf,"\n[Lv%d-St%d]-%02d",l,s,j);
				for(u16 j=0;j<10;++j)
				{
					fprintf(pf," %02X",sqma.S12Header.x[j]);
				}*/

				//PrintLog("%d\n",sqma.s9exl);
				/*if(sqma.Section10()[2]==0)
				{
					for(int k=2;k<10;++k)
					if(sqma.Section10()[k])printf("[Lv%d-St%d]-%d\n",l,s,j);
				}*/
				//else if(sqma.Section10()[2]!=8)printf("[Lv%d-St%d]-%d\n",l,s,j);
				/*if(sqma.Section10()[2]==8)printf("%d-%d-%02d:%02X %02X %02X %02X %02X %02X %02X %02X\n",l,s,j,
					sqma.Section10()[2],sqma.Section10()[3],sqma.Section10()[4],sqma.Section10()[5],
					sqma.Section10()[6],sqma.Section10()[7],sqma.Section10()[8],sqma.Section10()[9]);
				*/
				

				/*for(u8 x=0;x<sqma.GetW();++x)for(u8 y=0;y<sqma.GetH();++y)
				{
					if(sqma.Cell(x,y).det[1]==0x80000000)ASSERT(FALSE);
				}*/
				
				
				
				//for(u8 x=0;x<sqma.GetMctrlGroupCount();++x)
				//{
				//	fprintf(pf,"%d\t%d\n",sqma.GetMctrlGroupId(x),sqma.GetEvcDataLen(x));
				//}
			}
		}
	}

	/*for(int i=0;i<256;++i)
	{
		if(bossc[i]!=0xFFFFFFFF)
			fprintf(pf,"%d\t%d\n",i,bossc[i]);
	}*/
	//fclose(pf);

	/*FILE* pf=fopen("supclass.txt","wt");
	POSITION pos=list.GetHeadPosition();
	while(pos)
	{
		fprintf(pf,"%d\n",list.GetNext(pos));
	}
	fclose(pf);*/


	/*CStringA ofn;
	u8 l,s;
	m_SqMapSet.GetStageEntry(i,&l,&s);
	ofn.Format("a%ds%ds%d.det.txt",l,s,j);
	PrintLog(ofn);
	FILE *pf=fopen(ofn,"wt");
	for(u8 y=0;y<sqma.GetH();++y){for(u8 x=0;x<sqma.GetW();++x)
	{
		fprintf(pf,"%08X|",sqma.Cell(x,y).det[1]);
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
#else
	MessageBox(_T("这里是wwylele的私用空间，禁止进入"));
#endif
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
		TVS_HASLINES|TVS_SHOWSELALWAYS|TVS_EDITLABELS ,
		ZeroRect,this,ID_FILETREE);
	

	m_StaticDesc.Create(_T("..."),WS_CHILD|WS_VISIBLE,ZeroRect,this);
	m_StaticPrvw.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_OWNERDRAW,ZeroRect,this,ID_STATIC_PRVW);
	m_ButtonOption.Create(_T("操作..."),WS_CHILD|WS_VISIBLE,ZeroRect,this,ID_BUTTON_OPERATION);

	BMP_PRVW_W=GetSystemMetrics(SM_CXFULLSCREEN)-250;
	BMP_PRVW_H=GetSystemMetrics(SM_CYFULLSCREEN)-100;

	CDC *pDC=GetDC();
	m_DCPrvw.CreateCompatibleDC(pDC);
	m_BmpPrvw.CreateCompatibleBitmap(pDC,BMP_PRVW_W,BMP_PRVW_H);
	m_DCPrvw.SelectObject(&m_BmpPrvw);
	m_DCPrvw.FillRect((LPRECT)&CRect(0,0,BMP_PRVW_W,BMP_PRVW_H),&CBrush((COLORREF)0));

	InitDet0Bmp(pDC);
	InitDet1Bmp(pDC);
	ReleaseDC(pDC);

	//m_BmpDet0.LoadBitmap(IDB_DET0);


	m_TreeSelCls=FTS_0;
	m_TreeSelIdx=0;

	CWndWait::InitWndWait();

	//尝试打开命令行中的文件
	CCommandLineInfo cmdli;
	AfxGetApp()->ParseCommandLine(cmdli);
	CString strFileName=cmdli.m_strFileName;
	strFileName.Replace(_T("\""),_T(""));
	CFile file;
	if(file.Open(strFileName,CFile::modeReadWrite))
	{
		if(m_SqMapSet.Load(file))
		{
			m_StrFileName=strFileName;
			UpdateFileTree();
		}
		file.Close();
	}

	
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
	GetPrivateProfileString(_T("FILE"),_T("Path"),0,strPath,999,ProfilePath);
	CFileDialog filedlg(FALSE,_T("sqms"),_T("NewMapSet.sqms"),OFN_OVERWRITEPROMPT,
		_T("SqueakSquashMapSet|*.sqms||"),this);
	filedlg.GetOFN().lpstrInitialDir=strPath;
	filedlg.GetOFN().lpstrTitle=_T("选择保存的位置");
	if(filedlg.DoModal()==IDCANCEL)return;
	CString strFileName=filedlg.GetPathName(),strFOName=filedlg.GetFileName();
	WritePrivateProfileString(_T("FILE"),_T("Path"),
		strFileName.Left(strFileName.GetLength()-strFOName.GetLength()),ProfilePath);
	if(!file.Open(strFileName,CFile::modeCreate|CFile::modeWrite))
	{
		MessageBox(_T("创建文件失败"),_T("Error"),MB_ICONERROR);
		return;
	}
	m_StrFileName=strFileName;

	if(!m_SqMapSet.Save(file))
	{
		MessageBox(_T("SqMapSet::Save:写入文件失败"),_T("Error"),MB_ICONERROR);
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
		GetPrivateProfileString(_T("FILE"),_T("Path"),0,strPath,999,ProfilePath);
		CFileDialog filedlg(FALSE,_T("sqms"),_T("NewMapSet.sqms"),OFN_OVERWRITEPROMPT,
			_T("SqueakSquashMapSet|*.sqms||"),this);
		filedlg.GetOFN().lpstrInitialDir=strPath;
		filedlg.GetOFN().lpstrTitle=_T("选择保存的位置");
		if(filedlg.DoModal()==IDCANCEL)return;
		CString strFileName=filedlg.GetPathName(),strFOName=filedlg.GetFileName();
		WritePrivateProfileString(_T("FILE"),_T("Path"),
			strFileName.Left(strFileName.GetLength()-strFOName.GetLength()),ProfilePath);
		if(!file.Open(strFileName,CFile::modeCreate|CFile::modeWrite))
		{
			MessageBox(_T("创建文件失败"),_T("Error"),MB_ICONERROR);
			return;
		}
		m_StrFileName=strFileName;
	}
	else
	{
		if(!file.Open(m_StrFileName,CFile::modeCreate|CFile::modeWrite))
		{
			MessageBox(_T("写入文件失败"),_T("Error"),MB_ICONERROR);
			return;
		}
	}
	if(!m_SqMapSet.Save(file))
	{
		MessageBox(_T("SqMapSet::Save:写入文件失败"),_T("Error"),MB_ICONERROR);
	}
	file.Close();
	
}
void CMainFrame::OnTbOpen()
{
	TCHAR strPath[1000]={0};
	GetPrivateProfileString(_T("FILE"),_T("Path"),0,strPath,999,ProfilePath);
	
	CFileDialog filedlg(TRUE,0,0,0,
		_T("All supported files|*.sqms;*.nds;*.bin|")
		_T("SqueakSquashMapSet(*.sqms)|*.sqms|")
		_T("Nitro ROM(*.nds;*.bin)|*.nds;*.bin|")
		_T("All files(*.*)|*.*|")
		_T("|"),this);
	filedlg.GetOFN().lpstrInitialDir=strPath;
	if(filedlg.DoModal()==IDCANCEL)return;
	CString strFileName=filedlg.GetPathName(),strFOName=filedlg.GetFileName();
	WritePrivateProfileString(_T("FILE"),_T("Path"),
		strFileName.Left(strFileName.GetLength()-strFOName.GetLength()),ProfilePath);
	CFile file;
	if(!file.Open(strFileName,CFile::modeRead))
	{
		MessageBox(_T("打开文件失败，文件不存在或被其他程序占用"),_T("Error"),MB_ICONERROR);
		return;
	}
	if(!m_SqMapSet.Load(file))
	{
		//Fail to load it as a map set,try to load it as a ROM
		if(MessageBox(_T("准备将文件作为游戏ROM读入\n")
			_T("这个过程可能要花费几分钟\n")
			_T("是否继续?"),
			_T("SqueakSquash"),MB_YESNO)!=IDYES)
		{
			file.Close();
			return;
		}
		CWndWait::ShowWndWait();
		if(!m_SqMapSet.LoadFromRom(file))
		{
			CWndWait::HideWndWait();
			MessageBox(_T("读取文件失败"),_T("Error"),MB_ICONERROR);
			file.Close();
			return;
		}
		else
		{
			CWndWait::HideWndWait();
			MessageBox(_T("成功从ROM读入"),_T("SqueakSquash"));
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

	UpdateFileTree();
}


void CMainFrame::UpdateFileTree()
{
	ASSERT(m_SqMapSet.IsLoaded());

	CString str;
	char strn[16];

	m_FileTree.DeleteAllItems();
	m_htiMapi.RemoveAll();
	m_htiRom=m_FileTree.InsertItem(_T("Rom信息"));
	m_htiMap=m_FileTree.InsertItem(_T("地图"));
	m_htiBg=m_FileTree.InsertItem(_T("背景"));
	m_htiGl=m_FileTree.InsertItem(_T("贴图"));
	m_htiPl=m_FileTree.InsertItem(_T("调色板"));

	for(u32 i=0;i<m_SqMapSet.GetStageCount();++i)
	{
		u8 l,s;
		m_SqMapSet.GetStageEntry(i,&l,&s);
		str.Format(_T("Level%d:Stage%d"),l,s);
		m_htiMapi.Add(m_FileTree.InsertItem(str,m_htiMap));
		for(u16 j=0;j<m_SqMapSet.GetStepCount(i);++j)
		{
			str.Format(_T("Step%02d"),j);
			if(l==1&&s==1&&j==4)
			{
				str+=_T("(初始存档第一个进入的房间)");
			}
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

	//Section12
	/*PrintLog("S12:\nHeader:");
	PrintLog("%02X ",sqma.S12Header.x[0]);
	PrintLog("%02X ",sqma.S12Header.x[1]);
	PrintLog("%02X ",sqma.S12Header.x[2]);
	PrintLog("%02X ",sqma.S12Header.x[3]);
	PrintLog("%02X ",sqma.S12Header.x[4]);
	PrintLog("%02X ",sqma.S12Header.x[5]);
	PrintLog("%02X ",sqma.S12Header.x[6]);
	PrintLog("%02X ",sqma.S12Header.x[7]);
	PrintLog("%02X ",sqma.S12Header.x[8]);
	PrintLog("%02X ",sqma.S12Header.x[9]);
	PrintLog("\nData:\n");
	for(u16 i=0;i<sqma.S12Header.GraScriptCount;++i)
	{
		for(int j=0;j<12;j++){
			PrintLog("%02X ",sqma.pGraScript[i].x[j]);
		}
		PrintLog("\n");
	}
	PrintLog("\n");*/

	//CStringA ofn;
	//u8 l,s;
	//m_SqMapSet.GetStageEntry(Stage,&l,&s);
	//ofn.Format("a%ds%ds%d.det.txt",l,s,Step);
	//FILE *pf=fopen(ofn,"wt");

	PrintLog("Size=%d * %d\n",sqma.GetW(),sqma.GetH());


	for(u8 y=0;y<sqma.GetH();++y){for(u8 x=0;x<sqma.GetW();++x)
	{
		//fprintf(pf,"%08X|",sqma.Cell(x,y).det[1]);
		bb.DrawTile(&TempDC,
			sqma.TexMapping(TEXM_B,sqma.Cell(x,y).gra[1]).mapping[0],
			(x<<4)+0,(y<<4)+0,true,true);
		bb.DrawTile(&TempDC,
			sqma.TexMapping(TEXM_B,sqma.Cell(x,y).gra[1]).mapping[1],
			(x<<4)+8,(y<<4)+0,true,true);
		bb.DrawTile(&TempDC,
			sqma.TexMapping(TEXM_B,sqma.Cell(x,y).gra[1]).mapping[2],
			(x<<4)+0,(y<<4)+8,true,true);
		bb.DrawTile(&TempDC,
			sqma.TexMapping(TEXM_B,sqma.Cell(x,y).gra[1]).mapping[3],
			(x<<4)+8,(y<<4)+8,true,true);
		fb.DrawTile(&TempDC,
			sqma.TexMapping(TEXM_F,sqma.Cell(x,y).gra[0]).mapping[0],
			(x<<4)+0,(y<<4)+0,true,true);
		fb.DrawTile(&TempDC,
			sqma.TexMapping(TEXM_F,sqma.Cell(x,y).gra[0]).mapping[1],
			(x<<4)+8,(y<<4)+0,true,true);
		fb.DrawTile(&TempDC,
			sqma.TexMapping(TEXM_F,sqma.Cell(x,y).gra[0]).mapping[2],
			(x<<4)+0,(y<<4)+8,true,true);
		fb.DrawTile(&TempDC,
			sqma.TexMapping(TEXM_F,sqma.Cell(x,y).gra[0]).mapping[3],
			(x<<4)+8,(y<<4)+8,true,true);
		/*if(sqma.Cell(x,y).t)
		{
			CString str;
			str.Format(_T("%02X"),sqma.Cell(x,y).t);
			TempDC.TextOut(x<<4,y<<4,str,2);
		}*/
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

	u32 Width;
	switch(*(u16*)(p+0x66))
	{
	case 0x12E0:Width=512;break;
	default:Width=256;
	}


	u8* pD=p+0xA4;
	Nitro::Color15 *pP=(Nitro::Color15*)(p+Len+0xA4);
	
	for(u32 i=0;i<Len;++i)
	{
		m_DCPrvw.SetPixel(i%(Width),i/(Width),Nitro::Color15to24(pP[pD[i]]));
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
	PrintLog("GL\n");
	u32 i;
	u8 l,s,j,gli;
	for(i=0;i<m_SqMapSet.GetStageCount();++i)
	{
		m_SqMapSet.GetStageEntry(i,&l,&s);
		for(j=0;j<m_SqMapSet.GetStepCount(i);++j)
		{
			m_SqMapSet.GetStepInfo(i,j,0,&gli,0,0);
			if(gli==index)
			{
				PrintLog("%d-%d-%02d\n",l,s,j);
			}
		}
	}

	SqB sqb;
	sqb.Load(m_SqMapSet.GetGlBuffer(index,0));
	m_DCPrvw.FillRect((LPRECT)&CRect(0,0,BMP_PRVW_W,BMP_PRVW_H),&CBrush((COLORREF)0));
	for(u16 i=0;i<sqb.GetTileCount();++i)
	{
		sqb.DrawTile(&m_DCPrvw,*(Nitro::CharData*)&i,256+((i&31)<<3),(i>>5)<<3);
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
		//HTREEITEM tic=((NMTREEVIEW*)pNMHDR)->itemNew.hItem;
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
			
			str.Format(_T("关卡数量:%u"),m_SqMapSet.GetStageCount());
			m_StaticDesc.SetWindowText(str);
			m_TreeSelCls=FTS_MAP;
			m_TreeSelIdx=0;
		}
		else if(tic==m_htiBg)
		{
			
			str.Format(_T("背景数量:%u"),m_SqMapSet.GetBgCount());
			m_StaticDesc.SetWindowText(str);
			m_TreeSelCls=FTS_BGF;
			m_TreeSelIdx=0;
		}
		else if(tic==m_htiGl)
		{
			
			str.Format(_T("贴图数量:%u"),m_SqMapSet.GetGlCount());
			m_StaticDesc.SetWindowText(str);
			m_TreeSelCls=FTS_GLF;
			m_TreeSelIdx=0;
		}
		else if(tic==m_htiPl)
		{
			str.Format(_T("调色板数量:%u"),m_SqMapSet.GetPlCount());
			m_StaticDesc.SetWindowText(str);
			m_TreeSelCls=FTS_PLF;
			m_TreeSelIdx=0;
		}
		else if(m_FileTree.GetParentItem(tic)==m_htiMap)
		{
			u32 i=0;
			u8 l,s;
			for(;i<(u32)m_htiMapi.GetCount();++i)if(m_htiMapi[i]==tic)break;
			m_SqMapSet.GetStageEntry(i,&l,&s);
			str.Format(_T("Level%u:Stage%u\nStep数量:%u"),l,s,m_SqMapSet.GetStepCount(i));
			m_StaticDesc.SetWindowText(str);
			m_TreeSelCls=FTS_STAGE;
			m_TreeSelIdx=i;
		}
		else if(m_FileTree.GetParentItem(tic)==m_htiBg)
		{
			m_SqMapSet.GetBgName(m_FileTree.GetItemData(tic),strn);
			str.Format(_T("背景:")FORMAT_A2T,strn);
			m_StaticDesc.SetWindowText(str);
			PaintBgPrvw(m_FileTree.GetItemData(tic));
			m_TreeSelCls=FTS_BG;
			m_TreeSelIdx=m_FileTree.GetItemData(tic);
		}
		else if(m_FileTree.GetParentItem(tic)==m_htiGl)
		{
			m_SqMapSet.GetGlName(m_FileTree.GetItemData(tic),strn);
			str.Format(_T("贴图:")FORMAT_A2T,strn);
			m_StaticDesc.SetWindowText(str);
			PaintGlPrvw(m_FileTree.GetItemData(tic));
			m_TreeSelCls=FTS_GL;
			m_TreeSelIdx=m_FileTree.GetItemData(tic);
		}
		else if(m_FileTree.GetParentItem(tic)==m_htiPl)
		{
			m_SqMapSet.GetPlName(m_FileTree.GetItemData(tic),strn);
			str.Format(_T("调色板:")FORMAT_A2T
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
				m_SqMapSet.GetStageEntry(i,&l,&s);
				str.Format(_T("Level%u:Stage%u:Step%02u"),l,s,m_FileTree.GetItemData(tic));
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
void CMainFrame::OnFileTreeBeginLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	HTREEITEM tic=((NMTVDISPINFO*)pNMHDR)->item.hItem;
	HTREEITEM ticp=m_FileTree.GetParentItem(tic);
	if(ticp==m_htiBg ||ticp==m_htiGl||ticp==m_htiPl)
	{
		m_FileTree.GetEditControl()->SetLimitText(15);
		*pResult=FALSE;
	}
	else
	{
		*pResult=TRUE;
	}
	
}

void CMainFrame::OnFileTreeEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	char strn[16]={0};
	TCHAR* stri=((NMTVDISPINFO*)pNMHDR)->item.pszText;
	if(!stri)
	{
		*pResult=FALSE;
		return;
	}
	bool suc=true;
	for(int i=0;stri[i];++i)
	{
		//because the textbox has the length limit so this "if{}" is just an ensurence
		if(i==15)
		{
			ASSERT(FALSE);
			MessageBox(_T("字符数太多."));
			suc=false;
			break;
		}
		if(stri[i]<0x21 || stri[i]>0x7E)
		{
			MessageBox(_T("无效字符"));
			suc=false;
			break;
		}
		strn[i]=(char)stri[i];
	}
	/*if(m_SqMapSet.FindSecitem(0,strn)!=0xFFFFFFFF)
	{
		MessageBox(_T("名称冲突"));
		suc=false;
	}*/
	if(suc)
	{
		HTREEITEM tic=((NMTVDISPINFO*)pNMHDR)->item.hItem;
		HTREEITEM ticp=m_FileTree.GetParentItem(tic);
		if(ticp==m_htiBg)
		{
			m_SqMapSet.SetBgName(m_FileTree.GetItemData(tic),strn);
		}
		else if(ticp==m_htiGl)
		{
			m_SqMapSet.SetGlName(m_FileTree.GetItemData(tic),strn);
		}
		else if(ticp==m_htiPl)
		{
			m_SqMapSet.SetPlName(m_FileTree.GetItemData(tic),strn);
		}
		*pResult=TRUE;
	}
	else
	{
		*pResult=FALSE;
	}
	
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
void CMainFrame::PopupBoptm(int x,int y)
{
	CMenu menu;
	menu.CreatePopupMenu();
	CString str;
	u8 l,s;
	switch(m_TreeSelCls)
	{
	case FTS_ROM:
		menu.AppendMenu(MF_GRAYED,0,_T(">>>ROM"));
		menu.AppendMenu(0,ID_BOPTM_ROM,_T("编辑ROM信息"));
		break;
	case FTS_MAP:
		menu.AppendMenu(MF_GRAYED,0,_T(">>>地图"));
		break;
	case FTS_BGF:
		menu.AppendMenu(MF_GRAYED,0,_T(">>>背景库"));
		menu.AppendMenu(0,ID_BOPTM_NEW,_T("导入图片/贴图文件"));
		break;
	case FTS_GLF:
		menu.AppendMenu(MF_GRAYED,0,_T(">>>贴图库"));
		menu.AppendMenu(0,ID_BOPTM_NEW,_T("新建贴图"));
		break;
	case FTS_PLF:
		menu.AppendMenu(MF_GRAYED,0,_T(">>>调色板库"));
		break;
	case FTS_STAGE:
		m_SqMapSet.GetStageEntry(m_TreeSelIdx,&l,&s);
		str.Format(_T(">>>关卡 %d-%d"),l,s);
		menu.AppendMenu(MF_GRAYED,0,str);
		menu.AppendMenu(0,ID_BOPTM_ADDSTEP,_T("增加Step"));
		break;
	case FTS_STEP:
		m_SqMapSet.GetStageEntry(m_TreeSelIdx,&l,&s);
		str.Format(_T(">>>Step %d-%d-%02d"),l,s,m_TreeSelIdx2);
		menu.AppendMenu(MF_GRAYED,0,str);
		menu.AppendMenu(0,ID_BOPTM_EDIT,_T("编辑地图"));
		menu.AppendMenu(0,ID_BOPTM_CHANGELIB,_T("更改关联库"));
		menu.AppendMenu(0,ID_BOPTM_TEXMAP,_T("编辑贴图映射"));
		menu.AppendMenu(0,ID_BOPTM_MOVE,_T("移动Step"));
		menu.AppendMenu(0,ID_BOPTM_COPY,_T("复制Step"));
		menu.AppendMenu(0,ID_BOPTM_EXPORT,_T("导出地图文件"));
		menu.AppendMenu(0,ID_BOPTM_IMPORT,_T("导入地图文件"));
		menu.AppendMenu(0,ID_BOPTM_DELETE,_T("删除step"));
		break;
	case FTS_BG:case FTS_GL:case FTS_PL:{
		char strn[16];
		if(m_TreeSelCls==FTS_BG)m_SqMapSet.GetBgName(m_TreeSelIdx,strn);
		else if(m_TreeSelCls==FTS_GL)m_SqMapSet.GetGlName(m_TreeSelIdx,strn);
		else if(m_TreeSelCls==FTS_PL)m_SqMapSet.GetPlName(m_TreeSelIdx,strn);
		str.Format(_T(">>>%s ")FORMAT_A2T,
			m_TreeSelCls==FTS_BG?_T("背景"):
			(m_TreeSelCls==FTS_GL?_T("贴图"):_T("调色板")),strn);
		menu.AppendMenu(MF_GRAYED,0,str);
		if(m_TreeSelCls!=FTS_BG)menu.AppendMenu(0,ID_BOPTM_EDIT,_T("编辑"));
		menu.AppendMenu(0,ID_BOPTM_RENAME,_T("重命名"));
		if(m_TreeSelCls==FTS_BG)menu.AppendMenu(0,ID_BOPTM_NEW,_T("导入图片/贴图文件"));
		menu.AppendMenu(0,ID_BOPTM_COPY,_T("复制"));
		menu.AppendMenu(0,ID_BOPTM_EXPORT,_T("导出"));
		menu.AppendMenu(0,ID_BOPTM_DELETE,_T("删除"));
		if(m_TreeSelCls==FTS_GL)menu.AppendMenu(0,ID_BOPTM_NEW,_T("新建贴图"));
		break;}
	default:case FTS_0:
		menu.AppendMenu(MF_GRAYED,0,_T("未选择任何项"));
		break;
	}

	
	menu.TrackPopupMenu(
		GetSystemMetrics(SM_MENUDROPALIGNMENT)?
		TPM_RIGHTALIGN:TPM_LEFTALIGN,
		x,y,this);
	menu.DestroyMenu();
}
void CMainFrame::OnButtonOperation()
{
	if(!m_SqMapSet.IsLoaded())return;

	RECT brect;
	m_ButtonOption.GetWindowRect(&brect);
	PopupBoptm(brect.right,brect.top);

}

void CMainFrame::OnBoptmRename()
{
	ASSERT(m_SqMapSet.IsLoaded());
	
	HTREEITEM htiEntry;
	switch(m_TreeSelCls)
	{
	case FTS_BG:
		htiEntry=m_htiBg;
		break;
	case FTS_GL:
		htiEntry=m_htiGl;
		break;
	case FTS_PL:
		htiEntry=m_htiPl;
		break;
	}
	
	htiEntry=m_FileTree.GetChildItem(htiEntry);
	while(m_FileTree.GetItemData(htiEntry)!=m_TreeSelIdx)
		htiEntry=m_FileTree.GetNextItem(htiEntry,TVGN_NEXT);
	m_FileTree.EditLabel(htiEntry);
	

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
			MessageBox(_T("在一关里至少有一个Step"),0,MB_ICONERROR);
			return;
		}
		str.Format(_T("确定要删除Step %d ?"),m_TreeSelIdx2);
		if(MessageBox(str,_T("警告"),MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON2)!=IDYES)return;
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
	if((m_SqMapSet.GetSecitemCount(SiSwitch)==1 && m_TreeSelCls==FTS_PL)||
		(m_SqMapSet.GetSecitemCount(SiSwitch)==2 && m_TreeSelCls!=FTS_PL))
	{
		MessageBox(_T("库中至少要有一个文件"),0,MB_ICONERROR);
		return;
	}
	
	char strn[16];
	m_SqMapSet.GetSecitemName(SiSwitch,m_TreeSelIdx,strn);
	if(strcmp(strn,TEXTURE__CHAINBOMB)==0)
	{
		MessageBox(_T("不可以删除\"chainbomb.bin\"文件！"),0,MB_ICONERROR);
		return;
	}
	str.Format(_T("确定要删除 \"")FORMAT_A2T _T("\"?"),strn);
	if(MessageBox(str,_T("警告"),MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON2)!=IDYES)return;
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
		u16 newstep=m_SqMapSet.NewStep(m_TreeSelIdx);
		u8 a,b,c,d;
		m_SqMapSet.GetStepInfo(m_TreeSelIdx,(u16)m_TreeSelIdx2,&a,&b,&c,&d);
		m_SqMapSet.SetStepInfo(m_TreeSelIdx,newstep,a,b,c,d);
		u8 *pBuf;
		u32 bufLen;
		pBuf=m_SqMapSet.GetMxpBuffer(m_TreeSelIdx,(u16)m_TreeSelIdx2,&bufLen);
		memcpy(m_SqMapSet.ResizeMxpBuffer(m_TreeSelIdx,newstep,bufLen),pBuf,bufLen);
		pBuf=m_SqMapSet.GetDoeBuffer(m_TreeSelIdx,(u16)m_TreeSelIdx2,&bufLen);
		memcpy(m_SqMapSet.ResizeDoeBuffer(m_TreeSelIdx,newstep,bufLen),pBuf,bufLen);
		m_SqMapSet.RepairMxpHeader(m_TreeSelIdx,(u16)m_TreeSelIdx2);
		CString str;
		str.Format(_T("Step%02d"),newstep);
		m_FileTree.Select(m_FileTree.InsertItem(TVIF_TEXT|TVIF_PARAM,str,0,0,0,0,newstep,m_htiMapi[m_TreeSelIdx],TVI_LAST),TVGN_CARET);
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
void CMainFrame::OnBoptmEdit()
{
	if(m_TreeSelCls==FTS_STEP)
	{
		CDlgMapEdit dlg;
		u8 fbi,bbi,pli;
		dlg.m_Ma.Load(m_SqMapSet.GetMxpBuffer(m_TreeSelIdx,(u16)m_TreeSelIdx2));
		dlg.m_De.Load(m_SqMapSet.GetDoeBuffer(m_TreeSelIdx,(u16)m_TreeSelIdx2));
		m_SqMapSet.GetStepInfo(m_TreeSelIdx,(u16)m_TreeSelIdx2,0,&bbi,&fbi,&pli);
		dlg.m_Fb.Load(m_SqMapSet.GetGlBuffer(fbi,0));
		dlg.m_Bb.Load(m_SqMapSet.GetGlBuffer(bbi,0));
		SqPl1 sqpl1;
		if(pli!=0xFF && sqpl1.Load(m_SqMapSet.GetPlBuffer(pli,0)))
			dlg.m_Bb.pSqPl1=&sqpl1;
		else dlg.m_Bb.pSqPl1=0;

		dlg.DoModal();
		dlg.m_Ma.Make(m_SqMapSet.ResizeMxpBuffer(m_TreeSelIdx,(u16)m_TreeSelIdx2,
			dlg.m_Ma.MakeLen()));
		PaintStepPrvw(m_TreeSelIdx,(u16)m_TreeSelIdx2);
	}
}

void CMainFrame::OnFileTreeDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(m_TreeSelCls==FTS_STEP)
	{
		OnBoptmEdit();
	}
	if(m_TreeSelCls==FTS_ROM)
	{
		OnBoptmRom();
	}

	*pResult=0;
}

void CMainFrame::OnBoptmExport()
{
	CFile file;
	CString str;
	char strn[16];
	TCHAR strPath[1000]={0};
	GetPrivateProfileString(_T("FILE"),_T("ExportPath"),0,strPath,999,ProfilePath);
	if(m_TreeSelCls==FTS_STEP)
	{
		
		u8 lv,st;
		m_SqMapSet.GetStageEntry(m_TreeSelIdx,&lv,&st);
		str.Format(_T("a%ds%ds%d.")_T(EXFEX_MXP),lv,st,m_TreeSelIdx2);

		
		CFileDialog filedlg(FALSE,_T(EXFEX_MXP),str,OFN_OVERWRITEPROMPT,
			_T("SqSq Map File|*.")_T(EXFEX_MXP)_T("||"),this);
		filedlg.GetOFN().lpstrInitialDir=strPath;
		filedlg.GetOFN().lpstrTitle=_T("导出至...");
		if(filedlg.DoModal()==IDCANCEL)return;
		CString strFileName=filedlg.GetPathName(),strFOName=filedlg.GetFileName();
		WritePrivateProfileString(_T("FILE"),_T("ExportPath"),
		strFileName.Left(strFileName.GetLength()-strFOName.GetLength()),ProfilePath);
		if(!file.Open(strFileName,CFile::modeCreate|CFile::modeWrite))
		{
			MessageBox(_T("未能创建文件"),_T("Error"),MB_ICONERROR);
			return;
		}
		u8 *p;
		u32 l;
		p=m_SqMapSet.GetMxpBuffer(m_TreeSelIdx,(u16)m_TreeSelIdx2,&l);
		file.Write(p,l);
		file.Close();
	}
	else if(m_TreeSelCls==FTS_BG)
	{
		m_SqMapSet.GetBgName(m_TreeSelIdx,strn);
		str.Format(FORMAT_A2T /*_T(".")_T(EXFEX_NSBTX)*/,strn);

		CFileDialog filedlg(FALSE,_T(EXFEX_NSBTX),str,OFN_OVERWRITEPROMPT,
			_T("Nitro Texture|*.")_T(EXFEX_NSBTX)_T("||"),this);
		filedlg.GetOFN().lpstrInitialDir=strPath;
		filedlg.GetOFN().lpstrTitle=_T("导出至...");
		if(filedlg.DoModal()==IDCANCEL)return;
		CString strFileName=filedlg.GetPathName();
		WritePrivateProfileString(_T("FILE"),_T("ExportPath"),strFileName,ProfilePath);
		if(!file.Open(strFileName,CFile::modeCreate|CFile::modeWrite))
		{
			MessageBox(_T("未能创建文件."),_T("Error"),MB_ICONERROR);
			return;
		}
		u8 *p;
		u32 l;
		p=m_SqMapSet.GetBgBuffer(m_TreeSelIdx,&l);
		file.Write(p,l);
		file.Close();
	}
	else if(m_TreeSelCls==FTS_GL)
	{
		m_SqMapSet.GetGlName(m_TreeSelIdx,strn);
		str.Format(FORMAT_A2T _T(".")_T(EXFEX_BIN),strn);

		CFileDialog filedlg(FALSE,_T(EXFEX_BIN),str,OFN_OVERWRITEPROMPT,
			_T("SqSq Texture File|*.")_T(EXFEX_BIN)_T("||"),this);
		filedlg.GetOFN().lpstrInitialDir=strPath;
		filedlg.GetOFN().lpstrTitle=_T("导出至...");
		if(filedlg.DoModal()==IDCANCEL)return;
		CString strFileName=filedlg.GetPathName();
		WritePrivateProfileString(_T("FILE"),_T("ExportPath"),strFileName,ProfilePath);
		if(!file.Open(strFileName,CFile::modeCreate|CFile::modeWrite))
		{
			MessageBox(_T("未能创建文件."),_T("Error"),MB_ICONERROR);
			return;
		}
		u8 *p;
		u32 l;
		p=m_SqMapSet.GetGlBuffer(m_TreeSelIdx,&l);
		file.Write(p,l);
		file.Close();
	}
	else if(m_TreeSelCls==FTS_PL)
	{
		m_SqMapSet.GetPlName(m_TreeSelIdx,strn);
		str.Format(FORMAT_A2T _T(".")_T(EXFEX_PAL),strn);

		CFileDialog filedlg(FALSE,_T(EXFEX_PAL),str,OFN_OVERWRITEPROMPT,
			_T("SqSq Palette File|*.")_T(EXFEX_PAL)_T("||"),this);
		filedlg.GetOFN().lpstrInitialDir=strPath;
		filedlg.GetOFN().lpstrTitle=_T("导出至...");
		if(filedlg.DoModal()==IDCANCEL)return;
		CString strFileName=filedlg.GetPathName();
		WritePrivateProfileString(_T("FILE"),_T("ExportPath"),strFileName,ProfilePath);
		if(!file.Open(strFileName,CFile::modeCreate|CFile::modeWrite))
		{
			MessageBox(_T("未能创建文件."),_T("Error"),MB_ICONERROR);
			return;
		}
		u8 *p;
		u32 l;
		p=m_SqMapSet.GetPlBuffer(m_TreeSelIdx,&l);
		file.Write(p,l);
		file.Close();
	}
	else ASSERT(FALSE);
}
void CMainFrame::OnBoptmImport()
{
}
void CMainFrame::OnBoptmNew()
{
	if(m_TreeSelCls==FTS_GL ||m_TreeSelCls==FTS_GLF)
	{
		u8 SiSwitch=1;
		HTREEITEM htiEntry=m_htiGl;

		//generate a new name
		char strn[16];int ni=1;
		do{
			sprintf(strn,"new%d",ni++);
		}while(m_SqMapSet.FindSecitem(0,strn)!=0xFFFFFFFF);

		//new secitem in SqMapSet and copy the data
		SqB b;
		b.LoadDefault(false);
		b.Make(m_SqMapSet.NewSecitem(SiSwitch,b.MakeLen(),strn));

		//new secitem in file tree
		CString str;
		str.Format(FORMAT_A2T,strn);
		m_FileTree.SelectItem(m_FileTree.InsertItem(TVIF_TEXT|TVIF_PARAM,str,0,0,0,0,
			m_SqMapSet.GetSecitemCount(SiSwitch)-1,htiEntry,TVI_LAST));
	}
	else if(m_TreeSelCls==FTS_BG ||m_TreeSelCls==FTS_BGF)
	{
		char strn[16];

		CFileDialog filedlg(TRUE,0,0,0,
			_T("Image|*.bmp;*.dib;*.jpg;*.jpeg;*.gif;*.png;*.nsbtx|")
			_T("BMP|*.bmp;*.dib|")
			_T("JPEG|*.jpg;*.jpeg|")
			_T("GIF|*.gif|")
			_T("PNG|*.png|")
			_T("Nitro Texture|*.nsbtx|")
			_T("All files(*.*)|*.*|")
			_T("|"),this);
		filedlg.GetOFN().lpstrTitle=_T("选择图片导入成为游戏背景...");
		if(filedlg.DoModal()==IDCANCEL)return;
		CString strFileName=filedlg.GetPathName();
		CImage Img;
		if(FAILED(Img.Load(strFileName)))
		{
			//*.nsbtx?
			CFile file;
			if(!file.Open(strFileName,CFile::modeRead))
			{
				MessageBox(_T("打开文件失败"),_T("失败"),MB_ICONERROR);
				return;
			}
			u32 magic;
			file.Read(&magic,4);
			if(magic!=0x30585442)
			{
				file.Close();
				MessageBox(_T("未能识别图片格式"),_T("失败"),MB_ICONERROR);
				return;
			}
			file.Seek(CFile::begin,0);

			//generate a new name
			int ni=1;
			do{
				sprintf(strn,"new%d",ni++);
			}while(m_SqMapSet.FindSecitem(0,strn)!=0xFFFFFFFF);

			file.Read(m_SqMapSet.NewBg((u32)file.GetLength(),strn),(UINT)file.GetLength());
			file.Close();
		}
		else
		{

			CDlgImportBg dlg;
			dlg.m_ImageIn=&Img;
			if(dlg.DoModal()!=IDOK)return;
			
			//generate a new name
			int ni=1;
			do{
				sprintf(strn,"new%d",ni++);
			}while(m_SqMapSet.FindSecitem(0,strn)!=0xFFFFFFFF);

			memcpy(m_SqMapSet.NewBg(dlg.m_DataLen,strn),dlg.m_DataBuf,dlg.m_DataLen);
		}
		

		//new secitem in file tree
		CString str;
		str.Format(FORMAT_A2T,strn);
		m_FileTree.SelectItem(m_FileTree.InsertItem(TVIF_TEXT|TVIF_PARAM,str,0,0,0,0,
			m_SqMapSet.GetBgCount()-1,m_htiBg,TVI_LAST));
	}
}

void CMainFrame::OnFileTreeRclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint p,p2;
	if(m_SqMapSet.IsLoaded() && GetCursorPos(&p))
	{
		p2=p;

		m_FileTree.ScreenToClient(&p);

		UINT uflags;
		HTREEITEM hitem = m_FileTree.HitTest(p, &uflags);
		if ((hitem != NULL) && (TVHT_ONITEM & uflags))
		{
			m_FileTree.Select(hitem,TVGN_CARET);
			PopupBoptm(p2.x,p2.y);
		}
	}
	*pResult=0;
}

void CMainFrame::OnBoptmRom()
{
	if(!m_SqMapSet.IsLoaded())return;
	CDlgRomInfo dlg;
	dlg.m_RomInfo=m_SqMapSet.m_RomInfo;
	if(dlg.DoModal()==IDOK)m_SqMapSet.m_RomInfo=dlg.m_RomInfo;
}
void CMainFrame::OnBoptmTexMap()
{
	if(m_TreeSelCls==FTS_STEP)
	{
		CDlgTexMapping dlg;
		u8 fbi,bbi;
		dlg.m_Ma.Load(m_SqMapSet.GetMxpBuffer(m_TreeSelIdx,(u16)m_TreeSelIdx2));
		m_SqMapSet.GetStepInfo(m_TreeSelIdx,(u16)m_TreeSelIdx2,0,&bbi,&fbi,0);
		dlg.m_Fb.Load(m_SqMapSet.GetGlBuffer(fbi,0));
		dlg.m_Bb.Load(m_SqMapSet.GetGlBuffer(bbi,0));
		dlg.m_FbIndex=fbi;
		dlg.m_BbIndex=bbi;
		dlg.m_StageIdx=m_TreeSelIdx;
		dlg.m_StepIdx=(u16)m_TreeSelIdx2;
		dlg.m_pMapSet=&m_SqMapSet;

		dlg.DoModal();
		dlg.m_Ma.Make(m_SqMapSet.ResizeMxpBuffer(m_TreeSelIdx,(u16)m_TreeSelIdx2,
			dlg.m_Ma.MakeLen()));
	}
	
}