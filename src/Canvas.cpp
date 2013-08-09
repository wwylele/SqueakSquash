#include "StdAfx.h"
#include "Canvas.h"

CCanvas::CCanvas(void):pCanvas(0)
{
}

CCanvas::~CCanvas(void)
{
	if(pCanvas)delete[] pCanvas;
}
void CCanvas::Create(int w_, int h_)
{
	ASSERT(w_>0);
	ASSERT(h_>0);
	if(pCanvas)delete[] pCanvas;
	w=w_;
	h=h_;
	pCanvas=new BYTE[w*h*3];
}

void CCanvas::Present(HDC hdc,
		int xDest,int yDest,int DestWidth,int DestHeight,
		int xSrc,int ySrc,int SrcWidth,int SrcHeight)
{
	if(!pCanvas)return;
	BITMAPINFO binfo;
	binfo.bmiHeader.biSize=sizeof(BITMAPINFO);
	binfo.bmiHeader.biWidth=w;
	binfo.bmiHeader.biHeight=h;
	binfo.bmiHeader.biPlanes=1;
	binfo.bmiHeader.biBitCount=24;
	binfo.bmiHeader.biCompression=BI_RGB;
	binfo.bmiHeader.biSizeImage=0;
	binfo.bmiHeader.biClrUsed=0;
	binfo.bmiHeader.biClrImportant=0;

	if(!SrcWidth)SrcWidth=DestWidth;
	if(!SrcHeight)SrcHeight=DestHeight;
	StretchDIBits(hdc,xDest,yDest,DestWidth,DestHeight,xSrc,h-SrcHeight-ySrc,SrcWidth,SrcHeight,
		pCanvas,&binfo,DIB_RGB_COLORS,SRCCOPY);
}