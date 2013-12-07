#include "stdafx.h"
#include "Main.h"
#include <wininet.h>
#pragma comment(lib,"wininet.lib")

static bool GetInternetURLText(LPSTR lpcInternetURL, char* buff,ULONG MaxLen)
{   
    HINTERNET hSession;   
    hSession = InternetOpen(_T("WinInet"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);   
	__try
    {       
       if(hSession != NULL)        
       {          
           HINTERNET hRequest;            
           hRequest = InternetOpenUrlA(hSession,lpcInternetURL, NULL,0, INTERNET_FLAG_RELOAD, 0);        
           __try         
           {                
              if(hRequest != NULL)        
              {           
                  DWORD dwBytesRead;                  
                  char *szBuffer = new char[MaxLen];
				  ZeroMemory(szBuffer,MaxLen);
 
                  if(InternetReadFile(hRequest, szBuffer, MaxLen, &dwBytesRead))           
                  {                 
                     MoveMemory(buff, szBuffer, MaxLen);   
                     delete[] szBuffer;
                     return true;              
                  }
				  delete[] szBuffer;
              }           
           }__finally     
           {              
              InternetCloseHandle(hRequest);  
           }       
       }   
    }__finally    
    {       
       InternetCloseHandle(hSession);  
    }   
    return false;
}
void VersionFromInternet()
{
	char buf[1024];
	GetInternetURLText(
		"https://raw.github.com/wwylele/SqueakSquash/master/VS2008/ver",
		buf,1024);
	PrintLog(buf);

	
}