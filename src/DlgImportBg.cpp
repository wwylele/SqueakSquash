

#include "stdafx.h"
#include "Main.h"
#include "DlgImportBg.h"
#include "Nitro.h"

struct BG_STYLE
{
	UINT DataTempRes;
	TCHAR* Desc;
	enum{HORI,VERT,SQUA}Shape;
} BgStyle[]={
	{IDR_NSBTX_TEMP_HORI_00,_T("512*256;Type 1"),BG_STYLE::HORI},
	{IDR_NSBTX_TEMP_HORI_08,_T("512*256;Type 2"),BG_STYLE::HORI},
	{IDR_NSBTX_TEMP_HORI_0A,_T("512*256;Type 3"),BG_STYLE::HORI},
	{IDR_NSBTX_TEMP_HORI_1A,_T("512*256;Type 4"),BG_STYLE::HORI},
   {IDR_NSBTX_TEMP_HORI_20A,_T("512*256;Type 5"),BG_STYLE::HORI},
	{IDR_NSBTX_TEMP_VERT   ,_T("256*512"),       BG_STYLE::VERT},
	{IDR_NSBTX_TEMP_SQUA_19,_T("256*256;Type 1"),BG_STYLE::SQUA},
	{IDR_NSBTX_TEMP_SQUA_1D,_T("256*256;Type 2"),BG_STYLE::SQUA},
	{IDR_NSBTX_TEMP_SQUA_24,_T("256*256;Type 3"),BG_STYLE::SQUA},
	{0}
};


IMPLEMENT_DYNAMIC(CDlgImportBg, CDialog)

CDlgImportBg::CDlgImportBg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImportBg::IDD, pParent)
{
	m_DataBuf=new u8[131748];
}

CDlgImportBg::~CDlgImportBg()
{
	delete[]m_DataBuf;
}

void CDlgImportBg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_IMPORTBG, m_ComboBgStyle);
	DDX_Control(pDX, IDC_PROGRESS_IMPORTBG, m_Progress);
}


BEGIN_MESSAGE_MAP(CDlgImportBg, CDialog)
	ON_WM_DRAWITEM()
	ON_CBN_SELCHANGE(IDC_COMBO_IMPORTBG, &CDlgImportBg::OnCbnSelchangeComboImportbg)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTBG, &CDlgImportBg::OnBnClickedButtonImportbg)
END_MESSAGE_MAP()



BOOL CDlgImportBg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for(int i=0;BgStyle[i].DataTempRes;++i)
	{
		m_ComboBgStyle.AddString(BgStyle[i].Desc);
	}
	m_ComboBgStyle.SelectString(-1,BgStyle[0].Desc);

	m_Progress.SetRange(0,14);
	return TRUE; 
}
DWORD WINAPI CDlgImportBg::Thread_ImportBg(LPVOID lpThreadParameter)
{
	((CDlgImportBg*)lpThreadParameter)->Thread_ImportBg_Entry();

	return 0;
}
void CDlgImportBg::Thread_ImportBg_Entry()
{

	

	int bgstyle=m_ComboBgStyle.GetCurSel();
	int w=BgStyle[bgstyle].Shape==BG_STYLE::HORI?512:256;
	int h=BgStyle[bgstyle].Shape==BG_STYLE::VERT?512:256;

	HRSRC hResInfo=FindResource(AfxGetApp()->m_hInstance,
		MAKEINTRESOURCE(BgStyle[bgstyle].DataTempRes/*IDR_NSBTX_TEMPLET_0*/),_T("bin")); 
	HGLOBAL hResData=LoadResource(AfxGetApp()->m_hInstance,hResInfo);
	void *pRes=LockResource(hResData);
	u8* pdata=m_DataBuf;
	memcpy(pdata,pRes,0xA4);
	UnlockResource(hResData);
	u8* pbmp=pdata+0xA4;
	u8* pplt=pbmp+w*h;
	m_DataLen=0xA4+w*h+512;
	ZeroMemory(pbmp,w*h);
	ZeroMemory(pplt,512);

	void BmpToPlt256(
		const Nitro::Color15 BmpIn[],
		u32 Size,
		COLORREF BackColor,
		u8 BmpOut[],
		Nitro::Color15 PltOut[],

		CWnd* pWndUpdate,
		CProgressCtrl *pProgress);

	Nitro::Color15 *BmpIn=new Nitro::Color15[512*256];
	for(int y=0;y<h;++y)for(int x=0;x<w;++x)
	{
		
		if(x<m_ImageIn->GetWidth()&&y<m_ImageIn->GetHeight())
			BmpIn[y*w+x]=Nitro::Color24to15(m_ImageIn->GetPixel(x,y));
		else BmpIn[y*w+x].r=BmpIn[y*w+x].g=BmpIn[y*w+x].b=BmpIn[y*w+x].x=0;
	}
	BmpToPlt256(BmpIn,w*h,RGB(0,0,0),pbmp,(Nitro::Color15*)pplt,this,&m_Progress);
	delete[]BmpIn;

	PostMessage(WM_COMMAND,IDOK,0);
}
void CDlgImportBg::OnBnClickedButtonImportbg()
{

	GetDlgItem(IDC_BUTTON_IMPORTBG)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	EnableWindow(FALSE);


	CreateThread(0,0,Thread_ImportBg,this,0,0);

	
}

void CDlgImportBg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	if(nIDCtl==IDC_STATIC_IMPORTBG)
	{
		int bgstyle=m_ComboBgStyle.GetCurSel();
		RECT rc;
		GetDlgItem(IDC_STATIC_IMPORTBG)->GetClientRect(&rc);
		int s=min(rc.right,rc.bottom);
		FillRect(lpDrawItemStruct->hDC,&rc,(HBRUSH)GetStockObject(BLACK_BRUSH));
		switch(BgStyle[bgstyle].Shape)
		{
		case BG_STYLE::HORI:
			m_ImageIn->StretchBlt(lpDrawItemStruct->hDC,
				0,s/4,s,s/2,
				0,0,512,256);
			break;
		case BG_STYLE::VERT:
			m_ImageIn->StretchBlt(lpDrawItemStruct->hDC,
				s/4,0,s/2,s,
				0,0,256,512);
			break;
		case BG_STYLE::SQUA:
			m_ImageIn->StretchBlt(lpDrawItemStruct->hDC,
				0,0,s,s,
				0,0,256,256);
			break;
		}
	}

}

void CDlgImportBg::OnCbnSelchangeComboImportbg()
{
	GetDlgItem(IDC_STATIC_IMPORTBG)->RedrawWindow();
}


void CDlgImportBg::OnOK()
{



	CDialog::OnOK();
}
