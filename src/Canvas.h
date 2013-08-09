#pragma once

class CCanvas
{
public:
	CCanvas(void);
	~CCanvas(void);
	void Create(int w,int h);
	inline void SetPixel(int x,int y,COLORREF clr)
	{
		ASSERT(x>-1 && x<w);
		ASSERT(y>-1 && y<h);
		int p=(x+(h-1-y)*w)*3;
		pCanvas[p+2]=GetRValue(clr);
		pCanvas[p+1]=GetGValue(clr);
		pCanvas[p]=GetBValue(clr);
	}
	inline COLORREF GetPixel(int x,int y)
	{
		ASSERT(x>-1 && x<w);
		ASSERT(y>-1 && y<h);
		int p=(x+(h-1-y)*w)*3;
		return RGB(pCanvas[p+2],pCanvas[p+1],pCanvas[p]);
	}
	void Present(HDC hdc,
		int xDest,int yDest,int DestWidth,int DestHeight,
		int xSrc,int ySrc,int SrcWidth=0,int SrcHeight=0);

protected:
	BYTE *pCanvas;
	int w;
	int h;
};
