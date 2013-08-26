

#include "stdafx.h"
#include "DlgMakeRom.h"




IMPLEMENT_DYNAMIC(CDlgMakeRom, CDialog)

CDlgMakeRom::CDlgMakeRom(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMakeRom::IDD, pParent)
{

}

CDlgMakeRom::~CDlgMakeRom()
{
}

void CDlgMakeRom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MR_ROM, m_EditRom);
	DDX_Control(pDX, IDC_EDIT_MR_NEWROM, m_EditNewRom);
	DDX_Control(pDX, IDC_EDIT_MR_MAPSET, m_EditMapSet);
}


BEGIN_MESSAGE_MAP(CDlgMakeRom, CDialog)
	ON_COMMAND(IDC_BUTTON_MR_ROM, &CDlgMakeRom::OnMrRom)
	ON_COMMAND(IDC_BUTTON_MR_NEWROM, &CDlgMakeRom::OnMrNewrom)
	ON_COMMAND(IDC_BUTTON_MAKEROM, &CDlgMakeRom::OnMakeRom)
END_MESSAGE_MAP()



void CDlgMakeRom::OnMrRom()
{
	TCHAR strPath[1000]={0};
	GetPrivateProfileString(_T("FILE"),_T("RomPath"),0,strPath,999,ProfilePath);
	
	CFileDialog filedlg(TRUE,0,0,0,
		_T("Nitro ROM(*.nds;*.bin)|*.nds;*.bin|")
		_T("All files(*.*)|*.*|")
		_T("|"),this);
	filedlg.GetOFN().lpstrInitialDir=strPath;
	filedlg.GetOFN().lpstrTitle=_T("Set ROM Path");
	if(filedlg.DoModal()==IDCANCEL)return;
	CString strFileName=filedlg.GetPathName();
	WritePrivateProfileString(_T("FILE"),_T("RomPath"),strFileName,ProfilePath);
	m_EditRom.SetWindowText(strFileName);
	
}

void CDlgMakeRom::OnMrNewrom()
{
	TCHAR strPath[1000]={0};
	GetPrivateProfileString(_T("FILE"),_T("NewRomPath"),0,strPath,999,ProfilePath);
	CFileDialog filedlg(FALSE,_T("nds"),_T("NewKirby.nds"),OFN_OVERWRITEPROMPT,
		_T("Nitro ROM(*.nds)|*.nds|")_T("|"),this);
	filedlg.GetOFN().lpstrInitialDir=strPath;
	filedlg.GetOFN().lpstrTitle=_T("Set new ROM path...");
	if(filedlg.DoModal()==IDCANCEL)return;
	CString strFileName=filedlg.GetPathName();
	WritePrivateProfileString(_T("FILE"),_T("NewRomPath"),strFileName,ProfilePath);
	m_EditNewRom.SetWindowText(strFileName);
}

void CDlgMakeRom::OnMakeRom()
{
	CString strRom,strNewRom;
	m_EditRom.GetWindowText(strRom);
	m_EditNewRom.GetWindowText(strNewRom);
	if(!CopyFile(strRom,strNewRom,FALSE))
	{
		MessageBox(_T("Fail to Open the ROM or fail to Create the new ROM."),
			_T("Fail"),MB_ICONERROR);
		return;
	}
	CFile file;
	if(!file.Open(strNewRom,CFile::modeReadWrite))
	{
		MessageBox(_T("Fail to Create the new ROM."),
			_T("Fail"),MB_ICONERROR);
		return;
	}
	CWndWait::ShowWndWait();
	if(!m_pMapSet->MakeRom(file))
	{
		CWndWait::HideWndWait();
		MessageBox(_T("Fail to Make ROM."),
			_T("Fail"),MB_ICONERROR);
		file.Close();
		return;
	}
	CWndWait::HideWndWait();
	MessageBox(_T("Success to Make ROM"),_T("Success"),0);
	file.Close();
}

BOOL CDlgMakeRom::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_EditMapSet.SetWindowText(_T("This map set."));

	return TRUE;
}
