#include "stdafx.h"
#include "SqB.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
COLORREF R5G5B5X1toR8G8B8X8(u16 value)
{
	return RGB(
		(value&0x1F)*255/31,
		((value&0x3E0)>>5)*255/31,
		(value>>10)*255/31);
}
u16 R8G8B8X8toR5G5B5X1(COLORREF value)
{
	return (GetRValue(value)*31/255)|
		((GetGValue(value)*31/255)<<5)|
		((GetBValue(value)*31/255)<<10);
}
u8 TILE::Get(u8 x,u8 y)
{
	u8& k=dt[y*4+x/2];
	return x%2 ? k>>4:k&0xF;
}
void TILE::Set(u8 x,u8 y,u8 value)
{
	u8& k=dt[y*4+x/2];
	if(x%2)
	{
		k=(k&0xF)|(value<<4);
		
	}
	else
	{
		k=(k&0xF0)|(value&0xF);
	}
}
bool TILE::operator ==(TILE& value)
{
	for(int i=0;i<32;i++)
	{
		if(dt[i]!=value.dt[i])return false;
	}
	return true;
}

SqB::SqB():pTile(0),pPalLine(0)
{
}
SqB::~SqB()
{
	Unload();
}
bool SqB::IsLoaded()
{
	return pTile!=0;
}
void SqB::Unload()
{
	if(pTile)delete[]pTile;
	if(pPalLine)delete[]pPalLine;
}
bool SqB::Load(const u8* psrc)
{
	ASSERT(*(u32*)psrc==0x10);
	ASSERT(*(u32*)(psrc+4)==0x200);
	ASSERT(*(u32*)(psrc+8)==*(u32*)(psrc+12) *32);
	memcpy(Pal,psrc+0x10,0x200);
	Unload();
	TileCount=*(u32*)(psrc+12);
	pTile=new TILE[TileCount];
	pPalLine=new u8[TileCount];
	memcpy(pTile,psrc+0x210,sizeof(TILE)*TileCount);
	memcpy(pPalLine,psrc+0x210+sizeof(TILE)*TileCount,TileCount);
	return true;
}
TILE& SqB::Tile(u32 i)
{
	return pTile[i];
}
u8& SqB::PalLine(u32 i)
{
	return pPalLine[i];
}
u32 SqB::GetTileCount(){return TileCount;}
void SqB::DrawTile(CDC* pDC,u16 chardt,int x,int y,bool flip)
{

	
	bool flipx=chardt&1024 ? true:false;
	flipx=flipx&&flip;
	bool flipy=chardt&2048 ? true:false;
	flipy=flipy&&flip;
	if(flip)chardt&=0x3FF;
	for(int bx=0;bx<8;++bx)for(int by=0;by<8;++by)
	{
		pDC->SetPixel(x+bx,y+by,R5G5B5X1toR8G8B8X8(Pal[pTile[chardt].Get(flipx?7-bx:bx,flipy?7-by:by)|(pPalLine[chardt]<<4)]));
	}
}