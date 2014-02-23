

#include "stdafx.h"
#include "DlgObj.h"
#include "Evc.h"
#include "Foe.h"
#include "Door.h"

const TCHAR hextran[]=_T("0123456789ABCDEF");
#define FORMAT_OBJ _T("%d:%s")

IMPLEMENT_DYNAMIC(CDlgObj, CDialog)

CDlgObj::CDlgObj(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgObj::IDD, pParent)
{

}

CDlgObj::~CDlgObj()
{
}

void CDlgObj::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_OBJ_PARAM, m_EditObjParam);
	DDX_Control(pDX, IDC_EDIT_OBJ_X, m_EditObjX);
	DDX_Control(pDX, IDC_EDIT_OBJ_XP, m_EditObjXp);
	DDX_Control(pDX, IDC_EDIT_OBJ_Y, m_EditObjY);
	DDX_Control(pDX, IDC_EDIT_OBJ_YP, m_EditObjYp);
	DDX_Control(pDX, IDC_COMBO_OBJ_CLASS, m_ComboClass);
}


BEGIN_MESSAGE_MAP(CDlgObj, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJ_CLASS, &CDlgObj::OnCbnSelchangeComboObjClass)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CHexEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()

CHexEdit::CHexEdit()
{
	m_Font.Attach(GetStockObject(SYSTEM_FIXED_FONT));
}
CHexEdit::~CHexEdit()
{
	m_Font.Detach();
}
void CHexEdit::Init()
{

	SetFont(&m_Font);

}

void CHexEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UINT input;
	if(nChar>=(UINT)'0' && nChar<=(UINT)'9')
	{
		input=nChar-'0';
	}
	else if(nChar>=(UINT)'A' && nChar<=(UINT)'F')
	{
		input=nChar-(UINT)'A'+10;
	}
	else if(nChar>=(UINT)'a' && nChar<=(UINT)'f')
	{
		input=nChar-(UINT)'a'+10;
	}
	else 
	{
		MessageBeep(MB_OK);
		return;
	}

	int start,end;
	GetSel(start,end);
	int line=start/26;
	int n=start%26;
	if(n%3==2){++n;}
	if(n/3==8){n=0;++line;}
	int high=1-n%3;
	DWORD datai=line*8+n/3;
	if(datai>=m_DataLen)
	{
		MessageBeep(MB_OK);
		return;
	}

	if(high)
	{
		((BYTE*)m_pAttach)[datai]=
			( ((BYTE*)m_pAttach)[datai]&0xF )|( input<<4 );
	}
	else
	{
		((BYTE*)m_pAttach)[datai]=
			( ((BYTE*)m_pAttach)[datai]&0xF0 )|( input );
	}

	start=datai/8*26 + (datai%8)*3 + 1-high;
	SetSel(start,start+1);
	TCHAR str[2];
	str[0]=hextran[input];
	str[1]=0;
	ReplaceSel(str);
	SetSel(start+1,start+1);
}
void CHexEdit::UpdateText()
{
	CString str;
	TCHAR strbb[]=_T("XXB");

	for(DWORD i=0;i<m_DataLen;++i)
	{
		strbb[0]=hextran[((BYTE*)m_pAttach)[i]>>4];
		strbb[1]=hextran[((BYTE*)m_pAttach)[i]&0xF];
		if(i<(DWORD)m_charBreak.GetLength())
			strbb[2]=m_charBreak[i];
		else 
			strbb[2]=_T(' ');
		str+=strbb;
		if(i%8==7)str+=_T("\r\n");
	}
	SetWindowText(str);
}
void CHexEdit::OnRButtonDown(UINT nFlags, CPoint point)
{
	//Disable the popup menu
}
BOOL CDlgObj::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(title);

	

	CString str;
	str.Format(_T("%d"),x>>4);
	m_EditObjX.SetWindowText(str);
	m_EditObjX.SetLimitText(3);
	str.Format(_T("%c"),hextran[x&0xF]);
	m_EditObjXp.SetWindowText(str);
	m_EditObjXp.SetLimitText(1);
	str.Format(_T("%d"),y>>4);
	m_EditObjY.SetWindowText(str);
	m_EditObjY.SetLimitText(3);
	str.Format(_T("%c"),hextran[y&0xF]);
	m_EditObjYp.SetWindowText(str);
	m_EditObjYp.SetLimitText(1);
	m_EditObjParam.Init();
	m_EditObjParam.m_pAttach=param;
	if(objgroup==FOE)
	{
		for(u16 i=0;FoeDesc[i].name;++i)
		{
			str.Format(FORMAT_OBJ,i,FoeDesc[i].name);
			m_ComboClass.AddString(str);
			if(i==class_id)m_ComboClass.SelectString(-1,str);
		}
		m_EditObjParam.m_DataLen=8;
	}
	else if(objgroup==SUP)
	{
		for(u16 i=0;SupDesc[i].name;++i)
		{
			str.Format(FORMAT_OBJ,i,SupDesc[i].name);
			m_ComboClass.AddString(str);
			if(i==class_id)m_ComboClass.SelectString(-1,str);
		}
		m_EditObjParam.m_DataLen=8;
	}
	else if(objgroup==DOOR)
	{
		m_EditObjXp.EnableWindow(FALSE);
		m_EditObjYp.EnableWindow(FALSE);
		for(u16 i=0;DoorDesc[i].name;++i)
		{
			str.Format(FORMAT_OBJ,DoorDesc[i].class_id,DoorDesc[i].name);
			m_ComboClass.AddString(str);
			if(DoorDesc[i].class_id==class_id)m_ComboClass.SelectString(-1,str);
		}
		m_EditObjParam.m_DataLen=2;
	}
	else if(objgroup==MCTRL)
	{
		for(u8 i=0;EvcDesc[i].DataLen;++i)
		{
			str.Format(FORMAT_OBJ,i,EvcDesc[i].Name);
			m_ComboClass.AddString(str);
			if((u16)i==class_id)m_ComboClass.SelectString(-1,str);
		}
		m_EditObjParam.m_DataLen=2+GetEvcDataLen((u8)class_id);
		m_EditObjParam.m_charBreak=_T(" |");
	}
	m_EditObjParam.UpdateText();

	return FALSE;
}

void CDlgObj::OnOK()
{
	CString str;
	m_EditObjX.GetWindowText(str);
	x=_ttoi(str)<<4;
	m_EditObjXp.GetWindowText(str);
	if(str[0]>=(UINT)'0' && str[0]<=(UINT)'9')
	{
		x|=str[0]-'0';
	}
	else if(str[0]>=(UINT)'A' && str[0]<=(UINT)'F')
	{
		x|=str[0]-(UINT)'A'+10;
	}
	else if(str[0]>=(UINT)'a' && str[0]<=(UINT)'f')
	{
		x|=str[0]-(UINT)'a'+10;
	}
	else
	{
		m_EditObjXp.SetFocus();
		m_EditObjXp.SetSel(0,-1);
		MessageBeep(MB_OK);
		return;
	}
	m_EditObjY.GetWindowText(str);
	y=_ttoi(str)<<4;
	m_EditObjYp.GetWindowText(str);
	if(str[0]>=(UINT)'0' && str[0]<=(UINT)'9')
	{
		y|=str[0]-'0';
	}
	else if(str[0]>=(UINT)'A' && str[0]<=(UINT)'F')
	{
		y|=str[0]-(UINT)'A'+10;
	}
	else if(str[0]>=(UINT)'a' && str[0]<=(UINT)'f')
	{
		y|=str[0]-(UINT)'a'+10;
	}
	else
	{
		m_EditObjYp.SetFocus();
		m_EditObjYp.SetSel(0,-1);
		MessageBeep(MB_OK);
		return;
	}
	CDialog::OnOK();
}

void CDlgObj::OnCbnSelchangeComboObjClass()
{
	CString str;
	m_ComboClass.GetWindowText(str);
	int c;
	TCHAR dummybuf[100];
	_stscanf(str,FORMAT_OBJ,&c,dummybuf);
	class_id=(u16)c;
	if(objgroup==MCTRL)
	{
		m_EditObjParam.m_DataLen=2+GetEvcDataLen((u8)class_id);
		ZeroMemory(param+2,GetEvcDataLen((u8)class_id));
		m_EditObjParam.UpdateText();
	}
}
