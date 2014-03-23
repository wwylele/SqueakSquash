#include "stdafx.h"
#include "HexEdit.h"

const TCHAR hextran[]=_T("0123456789ABCDEF");

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