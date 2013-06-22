

#include "stdafx.h"
#include "Main.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_OPEN,&CMainFrame::OnTbOpen)
	ON_WM_SIZE()
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, &CMainFrame::OnTtnNeedText)
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
		TVS_HASLINES|TVS_SHOWSELALWAYS|TVS_SINGLEEXPAND ,
		ZeroRect,this,0);
	
	return 0;
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



void CMainFrame::OnTbOpen()
{
	TCHAR strPath[1000]={0};
	//GetPrivateProfileString(_T("FILE"),_T("Path"),0,strPath,999,ProfilePath);

	
	CFileDialog filedlg(TRUE,0,0,OFN_HIDEREADONLY,
		_T("SqueakSquash map set|*.*||"),this);
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
		MessageBox(_T("SqMapSet:Fail to load"),_T("Error"),MB_ICONERROR);
		file.Close();
		return;
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
#ifdef _UNICODE
		str.Format(L"%S",strn);
#else
		str.Format("%s",strn);
#endif
		m_FileTree.InsertItem(TVIF_TEXT|TVIF_PARAM,str,0,0,0,0,i,m_htiBg,TVI_LAST);
	}
	for(u32 i=0;i<m_SqMapSet.GetGlCount();++i)
	{
		m_SqMapSet.GetGlName(i,strn);
#ifdef _UNICODE
		str.Format(L"%S",strn);
#else
		str.Format("%s",strn);
#endif
		m_FileTree.InsertItem(TVIF_TEXT|TVIF_PARAM,str,0,0,0,0,i,m_htiGl,TVI_LAST);
	}
	for(u32 i=0;i<m_SqMapSet.GetPlCount();++i)
	{
		m_SqMapSet.GetPlName(i,strn);
#ifdef _UNICODE
		str.Format(L"%S",strn);
#else
		str.Format("%s",strn);
#endif
		m_FileTree.InsertItem(TVIF_TEXT|TVIF_PARAM,str,0,0,0,0,i,m_htiPl,TVI_LAST);
	}
}



void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	m_FileTree.MoveWindow(0,30,200,cy-35);
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