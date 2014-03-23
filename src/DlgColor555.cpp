
#include "stdafx.h"
#include "Main.h"
#include "DlgColor555.h"
#include "math.h"
#include "Nitro.h"

#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v2)))
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))
typedef struct
{
    BYTE  red;              // [0,255]
    BYTE  green;            // [0,255]
    BYTE  blue;             // [0,255]
}COLOR_RGB;
typedef struct
{
    float hue;              // [0,360]
    float saturation;       // [0,100]
    float luminance;        // [0,100]
}COLOR_HSL;
// Converts HSL to RGB
static void HSLtoRGB(const COLOR_HSL *hsl, COLOR_RGB *rgb)
{
    float h = hsl->hue;                  // h must be [0, 360]
    float s = hsl->saturation/100.f; // s must be [0, 1]
    float l = hsl->luminance/100.f;      // l must be [0, 1]
    float R, G, B;
    if(hsl->saturation == 0)
    {
        // achromatic color (gray scale)
        R = G = B = l*255.f;
	}
	else
    {
        float q = (l<0.5f)?(l * (1.0f+s)):(l+s - (l*s));
        float p = (2.0f * l) - q;
        float Hk = h/360.0f;
        float T[3];
        T[0] = Hk + 1.0f/3; // Tr  
        T[1] = Hk;              // Tb
        T[2] = Hk - 1.0f/3; // Tg
        for(int i=0; i<3; i++)
        {
            if(T[i] < 0) T[i] += 1.0f;
            if(T[i] > 1) T[i] -= 1.0f;
            if((T[i]*6) < 1)
            {
                T[i] = p + ((q-p)*6.0f*T[i]);
            }
            else if((T[i]*2.0f) < 1) //(1.0/6.0)<=T[i] && T[i]<0.5
            {
                T[i] = q;
            }
            else if((T[i]*3.0f) < 2) // 0.5<=T[i] && T[i]<(2.0/3.0)
            {
                T[i] = p + (q-p) * ((2.0f/3.0f) - T[i]) * 6.0f;
            }
            else T[i] = p;
        }
        R = T[0]*255.0f;
        G = T[1]*255.0f;
        B = T[2]*255.0f;
    }
    rgb->red = (BYTE)((R>255)? 255 : ((R<0)?0 : R));
    rgb->green = (BYTE)((G>255)? 255 : ((G<0)?0 : G));
    rgb->blue = (BYTE)((B>255)? 255 : ((B<0)?0 : B));
}


IMPLEMENT_DYNAMIC(CDlgColor555, CDialog)

CDlgColor555::CDlgColor555(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgColor555::IDD, pParent)
{

}

CDlgColor555::~CDlgColor555()
{
}

void CDlgColor555::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgColor555, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()



void CDlgColor555::OnPaint()
{
	CPaintDC dc(this);
	int R=200;
	int px,py;
	float r,theta;
	COLOR_HSL chsl;
	COLOR_RGB crgb;
	COLORREF rgb;
	Nitro::Color15 c555,c5552;
	for(int x=0;x<R*2;++x)for(int y=0;y<R*2;++y)
	{
		px=x-R;
		py=y-R;
		if(px*px+py*py<=R*R)
		{
			r=sqrt((float)(px*px+py*py));
			theta=atan2((float)px,(float)py);
			chsl.hue=theta/(2*3.14159265358979323846f)*360.0f;
			chsl.luminance=50.0f;
			chsl.saturation=r*100.0f/R;
			HSLtoRGB(&chsl,&crgb);
			rgb=RGB(crgb.red,crgb.green,crgb.blue);
			c555=Nitro::Color24to15(rgb);
			rgb=Nitro::Color15to24(c555);
			c5552=Nitro::Color24to15(rgb);
			ASSERT(memcmp(&c555,&c5552,2)==0);
			dc.SetPixel(x,y,rgb);
		}
	}
}
