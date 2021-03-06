﻿
//UTF8♔


#include "stdafx.h"
#include "Main.h"
#include "Det.h"

const DetDesc DetDesc0[]=
{
	{0x00000000,_T("无"),				0,0},	
	{0x19000000,_T("积雪"),				5,2},
	{0x29000000,_T("冰面"),				5,0},
	{0xFFFFFFFE},

	{0x00810000,_T("导火索←"),			2,1},
	{0x00830000,_T("导火索─"),			7,1},
	{0x00820000,_T("导火索→"),			3,1},
	{0x00840000,_T("导火索↑"),			0,1},
	{0x008C0000,_T("导火索│"),			4,1},
	{0x00880000,_T("导火索↓"),			1,1},
	{0x00850000,_T("导火索┌"),			5,1},
	{0x00860000,_T("导火索┐"),			6,1},
	{0x00890000,_T("导火索└"),			8,1},
	{0x008A0000,_T("导火索┘"),			9,1},
	
	{0xFFFFFFFE},


	{0x38400000,_T("绳子"),				1,3},
	{0x38410000,_T("绳子底部"),			2,3},
	{0x38420000,_T("绳子顶部"),			0,3},
	{0x38440000,_T("钢缆"),				4,3},
	{0x38450000,_T("钢缆底部"),			5,3},
	{0x38460000,_T("钢缆顶部"),			3,3},
	{0x38480000,_T("机关绳子?"),		7,3},
	{0x38490000,_T("机关绳子底部?"),	8,3},
	{0x384A0000,_T("机关绳子顶部?"),	6,3},
	{0xFFFFFFFE},


	{0x42000000,_T("电火花"),			0,4},
	{0x3A004000,_T("电链"),				7,7},
	{0x80000000,_T("火焰"),				5,4},
	

	{0xFFFFFFFE},
	{0x05004000,_T("大星星砖 0"),		5,6},
	{0x05104000,_T("大星星砖 1"),		6,6},
	{0x00000400,_T("隐藏连锁"),			0,5},
	{0x38000800,_T("燃烧连锁"),			5,5},
	{0x38001000,_T("爆炸连锁"),			6,5},
	
	{0xFFFFFFFE},

	{0x05204000,_T("大星星砖 2"),		7,6},
	{0x05304000,_T("大星星砖 3"),		8,6},
	{0x04004000,_T("星星砖"),			4,6},
	{0x38005000,_T("爆炸机关"),			8,5},
	{0x39004000,_T("木桩"),				6,7},
	{0x00004000,_T("土块"),				0,6},
	{0x01004000,_T("硬砖"),				1,6},
	{0x03004000,_T("易碎砖A"),	3,6},
	{0x03000000,_T("易碎砖B"),	0,2},
	{0x02004000,_T("钢砖"),				2,6},
	{0x38004800,_T("燃烧机关"),			7,5},
	{0x28004000,_T("两次击打冰块"),	4,7},
	{0x30004000,_T("两次击打土块"),	5,7},
	{0xFFFFFFFE},
	{0x08004000,_T("土"),				0,7},
	{0x10004000,_T("沙子"),				1,7},
	{0x18004000,_T("雪"),				2,7},
	{0x20004000,_T("云"),				3,7},
	{0x2000C028,_T("斜坡云 0"),			0,8},
	{0x2000C068,_T("斜坡云 1"),			1,8},
	{0x2000C0A8,_T("斜坡云 2"),			2,8},
	{0x2000C0E8,_T("斜坡云 3"),			3,8},



	{0xFFFFFFFF}
};

BMP1b16x16 Det0Bmp[sizeof(DetDesc0)/sizeof(DetDesc)];
const DetDesc* GetDet0Desc(u8 stockx,u8 stocky)
{
	u8 x=0,y=0;
	for(int i=0;DetDesc0[i].code!=0xFFFFFFFF;++i)
	{
		if(DetDesc0[i].code==0xFFFFFFFE)x=0,++y;
		else{
			if(y>stocky)break;
			if(stockx==x && stocky==y)return &DetDesc0[i];

			++x;
		}
	}
	return 0;
}

void InitDet0Bmp(CDC* pDC)
{
	CBitmap bmp;
	bmp.LoadBitmap(IDB_DET0);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap* oldbmp=dc.SelectObject(&bmp);

	for(int i=0;DetDesc0[i].code!=0xFFFFFFFF;++i)
	{
		if(DetDesc0[i].code!=0xFFFFFFFE)for(u8 x=0;x<16;++x)for(u8 y=0;y<16;++y)
		{
			Det0Bmp[i].Set(x,y,
				dc.GetPixel(DetDesc0[i].tex_x*16+x,DetDesc0[i].tex_y*16+y)?false:true);
		}
	}

	dc.SelectObject(oldbmp);
	dc.DeleteDC();
	bmp.DeleteObject();
}

const DetDesc* GetDet0Desc(u32 code,BMP1b16x16** GetDetBmp)
{
	for(int i=0;DetDesc0[i].code!=0xFFFFFFFF;++i)
	{
		if(DetDesc0[i].code==code){
			if(GetDetBmp)*GetDetBmp=&Det0Bmp[i];
			return &DetDesc0[i];
		}
	}
	return 0;
}


const u32 Det1List[]={
#define DET1_AIR_ZONE_A(air) \
	0x00000000|air, \
	0x00100000|air,0x00140000|air,0x00180000|air,0x001C0000|air, \
	0x00200000|air,0x00240000|air,0x00280000|air,0x002C0000|air, \
	0x00300000|air,0x00340000|air,0x00380000|air,0x003C0000|air, \
	0x00400000|air,0x00440000|air,0x00480000|air,0x004C0000|air,
#define DET1_AIR_ZONE_B(air) \
	0x00800000|air,0x00810000|air,0x00820000|air,0x00830000|air, \
	0x00840000|air,0x00850000|air,0x00860000|air,0x00870000|air, \
	0x00880000|air,0x00890000|air,0x008A0000|air,0x008B0000|air, \
	0x008C0000|air,0x008D0000|air,0x008E0000|air,0x008F0000|air, \
	0x00900000|air,0x00910000|air,0x00920000|air,0x00930000|air, \
	0x00940000|air,0x00950000|air,0x00960000|air,0x00970000|air, \
	0x00980000|air,0x00990000|air,0x009A0000|air,0x009B0000|air, \
	0x009C0000|air,0x009D0000|air,0x009E0000|air,0x009F0000|air,


	DET1_AIR_ZONE_A(0x00000000)
	0x00000100,
	0x00002000,
	0x28004000,
	0xFFFFFFFE,
	DET1_AIR_ZONE_B(0x00000000)
	0xFFFFFFFE,
	DET1_AIR_ZONE_A(0x40000000)
	0xFFFFFFFE,
	DET1_AIR_ZONE_B(0x40000000)
	0xFFFFFFFE,
	DET1_AIR_ZONE_A(0x80000000)
	0xFFFFFFFE,
	DET1_AIR_ZONE_B(0x80000000)
	0xFFFFFFFE,

#undef DET1_AIR_ZONE_A
#undef DET1_AIR_ZONE_B

	

#define DET1_MTR_ZONE(mtr) \
	0x00008200|mtr,0x00008214|mtr,0x0000821C|mtr,0x00008228|mtr,0x000082E8|mtr,0x000082FC|mtr,0x000082F4|mtr, \
	0x00008300|mtr,0x00008314|mtr,0x0000831C|mtr,0x00008328|mtr,0x000083E8|mtr,0x000083FC|mtr,0x000083F4|mtr, \
	0x00004000|mtr,0x0000C014|mtr,0x0000C01C|mtr,0x0000C028|mtr,0x0000C0E8|mtr,0x0000C0FC|mtr,0x0000C0F4|mtr, \
	0x00008100|mtr,0x0000C07C|mtr,0x0000C074|mtr,0x0000C068|mtr,0x0000C0A8|mtr,0x0000C094|mtr,0x0000C09C|mtr, \
	0xFFFFFFFE,

	DET1_MTR_ZONE(0x00000000)
	DET1_MTR_ZONE(0x08000000)
	DET1_MTR_ZONE(0x10000000)
	DET1_MTR_ZONE(0x18000000)
	DET1_MTR_ZONE(0x20000000)
#undef DER_MTR_ZONE

	0xFFFFFFFF
};


BMP3216x16 Det1Bmp[sizeof(Det1List)/sizeof(u32)];
BMP3216x16* GetDet1Bmp(u32 code)
{
	for(int i=0;Det1List[i]!=0xFFFFFFFF;++i)
	{
		if(Det1List[i]==code){
			return &Det1Bmp[i];
		}
	}
	return 0;
}
void InitDet1Bmp(CDC* pDC)
{
	CBitmap bmp;
	bmp.LoadBitmap(IDB_DET1);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap* oldbmp=dc.SelectObject(&bmp);

	int ox,oy;
	COLORREF vclr,c;
	int mtrzone_accum=0;
	for(int i=0;Det1List[i]!=0xFFFFFFFF;++i)
	{
		if(Det1List[i]==0xFFFFFFFE)vclr=RGB(255,255,255),ox=0,oy=0;//"\n"
		else if(Det1List[i]==0x00000100)ox=2,oy=0;//ladder
		else if(Det1List[i]==0x00002000)ox=1,oy=0,vclr=RGB(255,0,0);//death
		else if(Det1List[i]==0x28004000)ox=3,oy=0,vclr=RGB(255,100,0);//magma
		else if(!(Det1List[i]&0xFFFF))//Air Zone
		{
			if((Det1List[i]&0xF0000000)==0x40000000)vclr=RGB(127,225,255);//Water
			else if((Det1List[i]&0xF0000000)==0x80000000)vclr=RGB(255,192,255);//Debug Air
			else /*if((Det1List[i]&0xF0000000)==0x00000000)*/vclr=RGB(255,255,255);//Air

			if((Det1List[i]&0x00FF0000)==0)ox=0,oy=0;//Zero
			else if((Det1List[i]&0x00FF0000)<0x00800000)//Wind
			{
				oy=1;
				ox=((Det1List[i]&0x00FF0000)-0x00100000)>>(16+2);
			}
			else  //Door
			{
				oy=(Det1List[i]&0x00F00000)==0x00800000?2:3;
				ox=(Det1List[i]&0x000F0000)>>16;
			}
		}
		else //Mtr Zone
		{
			if((Det1List[i]&0xFF000000)==0x00000000)vclr=RGB(255,189,0);
			else if((Det1List[i]&0xFF000000)==0x08000000)vclr=RGB(123,123,180);
			else if((Det1List[i]&0xFF000000)==0x10000000)vclr=RGB(100,100,100);
			else if((Det1List[i]&0xFF000000)==0x18000000)vclr=RGB(120,70,10);
			else /*if((Det1List[i]&0xFF000000)==0x20000000)*/vclr=RGB(0,131,255);
			
			ox=mtrzone_accum%14;
			oy=(mtrzone_accum/14)%2+4;
			mtrzone_accum++;

		}


		for(int x=0;x<16;++x)for(int y=0;y<16;++y)
		{
			c=dc.GetPixel(x+ox*16,y+oy*16);
			if(c==RGB(255,255,255))Det1Bmp[i].data[x][y]=RGB(255,255,255);
			else if(c==0)Det1Bmp[i].data[x][y]=0;
			else Det1Bmp[i].data[x][y]=vclr;
		}
	}

	dc.SelectObject(oldbmp);
	dc.DeleteDC();
	bmp.DeleteObject();
}
u32 GetDet1CodeByStockCoord(u8 stockx,u8 stocky)
{
	u8 x=0,y=0;
	for(int i=0;Det1List[i]!=0xFFFFFFFF;++i)
	{
		if(Det1List[i]==0xFFFFFFFE)x=0,++y;
		else{
			if(y>stocky)break;
			if(stockx==x && stocky==y)return Det1List[i];
			++x;
		}
	}
	return 0xFFFFFFFF;
}
void GetDet1Desc(u32 code,CString &str)
{
	if(code==0x00000100)str=_T("梯子");
	else if(code==0x00002000)str=_T("即死点");
	else if(code==0x28004000)str=_T("岩浆");
	else if(!(code&0xFFFF))//Air Zone
	{
		if((code&0xF0000000)==0x40000000)str=_T("水");//Water
		else if((code&0xF0000000)==0x80000000)str=_T("未知");//Debug Air
		else /*if((code&0xF0000000)==0x00000000)*/str=_T("空气");//Air

		if((code&0x00FF0000)==0)str+=_T("");//Zero
		else if((code&0x00FF0000)<0x00800000)//Wind
		{
			switch(code&0x00FF0000)
			{
			case 0x100000:str+=_T("↑Lv1");break;
			case 0x140000:str+=_T("→Lv1");break;
			case 0x180000:str+=_T("↓Lv1");break;
			case 0x1C0000:str+=_T("←Lv1");break;
			case 0x200000:str+=_T("↑Lv2");break;
			case 0x240000:str+=_T("→Lv2");break;
			case 0x280000:str+=_T("↓Lv2");break;
			case 0x2C0000:str+=_T("←Lv2");break;
			case 0x300000:str+=_T("↑Lv3");break;
			case 0x340000:str+=_T("→Lv3");break;
			case 0x380000:str+=_T("↓Lv3");break;
			case 0x3C0000:str+=_T("←Lv3");break;
			case 0x400000:str+=_T("↑刺");break;
			case 0x440000:str+=_T("→刺");break;
			case 0x480000:str+=_T("↓刺");break;
			case 0x4C0000:str+=_T("←刺");break;
			}
		}
		else  //Door
		{
			CString str2;
			str2.Format(_T("传送点%d"),(code&0x007F0000)>>16);
			str+=str2;
		}
	}
	else //Mtr Zone
	{
		switch(code&0xFFFF)
		{
		case 0x00008200:str=_T("单向平台");break;
		case 0x00008214:str=_T("1/2左斜单向平台A");break;
		case 0x0000821C:str=_T("1/2左斜单向平台B");break;
		case 0x00008228:str=_T("左斜单向平台");break;
		case 0x000082E8:str=_T("右斜单向平台");break;
		case 0x000082FC:str=_T("1/2右斜单向平台A");break;
		case 0x000082F4:str=_T("1/2右斜单向平台B");break;
		case 0x00008300:str=_T("双向平台");break;
		case 0x00008314:str=_T("1/2左斜双向平台A");break;
		case 0x0000831C:str=_T("1/2左斜双向平台B");break;
		case 0x00008328:str=_T("左斜双向平台");break;
		case 0x000083E8:str=_T("右斜双向平台");break;
		case 0x000083FC:str=_T("1/2右斜双向平台A");break;
		case 0x000083F4:str=_T("1/2右斜双向平台B");break;
		case 0x00004000:str=_T("实心砖块");break;
		case 0x0000C014:str=_T("1/2左斜坡A");break;
		case 0x0000C01C:str=_T("1/2左斜坡B");break;
		case 0x0000C028:str=_T("左斜坡");break;
		case 0x0000C0E8:str=_T("右斜坡");break;
		case 0x0000C0FC:str=_T("1/2右斜坡A");break;
		case 0x0000C0F4:str=_T("1/2有斜坡B");break;
		case 0x00008100:str=_T("梯子顶");break;
		case 0x0000C07C:str=_T("1/2左斜顶A");break;
		case 0x0000C074:str=_T("1/2左斜顶B");break;
		case 0x0000C068:str=_T("左斜顶");break;
		case 0x0000C0A8:str=_T("右斜顶");break;
		case 0x0000C094:str=_T("1/2右斜顶A");break;
		case 0x0000C09C:str=_T("1/2右斜顶B");break;
		}

	}
}