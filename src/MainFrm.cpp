

#include "stdafx.h"
#include "Main.h"

#include "MainFrm.h"
#include "SqB.h"

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
	m_StaticDesc.MoveWindow(202,30,400,200);
	m_StaticPrvw.MoveWindow(202,250,cx-220,cy-280);
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
		str.Format(FORMAT_S2S,strn);
		m_FileTree.InsertItem(TVIF_TEXT|TVIF_PARAM,str,0,0,0,0,i,m_htiBg,TVI_LAST);
	}
	for(u32 i=0;i<m_SqMapSet.GetGlCount();++i)
	{
		m_SqMapSet.GetGlName(i,strn);
		str.Format(FORMAT_S2S,strn);
		m_FileTree.InsertItem(TVIF_TEXT|TVIF_PARAM,str,0,0,0,0,i,m_htiGl,TVI_LAST);
	}
	for(u32 i=0;i<m_SqMapSet.GetPlCount();++i)
	{
		m_SqMapSet.GetPlName(i,strn);
		str.Format(FORMAT_S2S,strn);
		m_FileTree.InsertItem(TVIF_TEXT|TVIF_PARAM,str,0,0,0,0,i,m_htiPl,TVI_LAST);
	}
}




void CMainFrame::PaintGlPrvw(u32 index)
{
	SqB sqb;
	sqb.Load(m_SqMapSet.GetGlBuffer(index,0));
	m_DCPrvw.FillRect((LPRECT)&CRect(0,0,BMP_PRVW_W,BMP_PRVW_H),&CBrush((COLORREF)0));
	for(u32 i=0;i<sqb.GetTileCount();++i)
	{
		sqb.DrawTile(&m_DCPrvw,i,(i%32)*8,i/32*8);
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
			str=_T("ROM");
			m_StaticDesc.SetWindowText(str);
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
			str.Format(_T("Background:")FORMAT_S2S,strn);
			m_StaticDesc.SetWindowText(str);
		}
		else if(m_FileTree.GetParentItem(tic)==m_htiGl)
		{
			m_SqMapSet.GetGlName(m_FileTree.GetItemData(tic),strn);
			str.Format(_T("Texture:")FORMAT_S2S,strn);
			m_StaticDesc.SetWindowText(str);
			PaintGlPrvw(m_FileTree.GetItemData(tic));
		}
		else if(m_FileTree.GetParentItem(tic)==m_htiPl)
		{
			m_SqMapSet.GetPlName(m_FileTree.GetItemData(tic),strn);
			str.Format(_T("Palatte:")FORMAT_S2S,strn);
			m_StaticDesc.SetWindowText(str);
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
			}
		}
	}

	*pResult=0;
}


void CMainFrame::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(nIDCtl==ID_STATIC_PRVW)
	{
		BitBlt(lpDrawItemStruct->hDC,0,0,500,500,m_DCPrvw.GetSafeHdc(),0,0,SRCCOPY);
	}
	//CFrameWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
