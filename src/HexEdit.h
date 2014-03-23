#pragma once
class CHexEdit : public CEdit
{
public:
	DECLARE_MESSAGE_MAP()
public:
	CHexEdit();
	virtual ~CHexEdit();
	void *m_pAttach;
	DWORD m_DataLen;
	CString m_charBreak;
	void UpdateText();
private:
	CFont m_Font;
public:
	void Init();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};