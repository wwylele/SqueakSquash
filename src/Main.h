
//
#pragma once

#ifndef __AFXWIN_H__
	#error "stdafx.h"
#endif

#include "../VS2008/resource.h"      

void OpenConsole();
#define PrintLog printf

class CSqsqApp : public CWinApp
{
public:
	CSqsqApp();



public:
	virtual BOOL InitInstance();



public:

	DECLARE_MESSAGE_MAP()
};
