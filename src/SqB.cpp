#include "stdafx.h"

#include "SqB.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif




SqB::SqB():pTile(0),pPalLine(0)
{
}
SqB::~SqB()
{
	Unload();
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
	pTile=new Nitro::Tile[TileCount];
	pPalLine=new u8[TileCount];
	memcpy(pTile,psrc+0x210,sizeof(Nitro::Tile)*TileCount);
	memcpy(pPalLine,psrc+0x210+sizeof(Nitro::Tile)*TileCount,TileCount);
	return true;
}

u32 SqB::MakeLen()
{
	ASSERT(IsLoaded());
	return 0x10+0x200+TileCount*sizeof(Nitro::Tile)+TileCount;
}
void SqB::Make(u8* pdst)
{
	ASSERT(IsLoaded());
	*(u32*)pdst=0x10;pdst+=4;
	*(u32*)pdst=0x200;pdst+=4;
	*(u32*)pdst=sizeof(Nitro::Tile)*TileCount;pdst+=4;
	*(u32*)pdst=TileCount;pdst+=4;
	memcpy(pdst,Pal,0x200);pdst+=0x200;
	memcpy(pdst,pTile,sizeof(Nitro::Tile)*TileCount);pdst+=sizeof(Nitro::Tile)*TileCount;
	memcpy(pdst,pPalLine,TileCount);
}

void SqB::DrawTile(CDC* pDC,Nitro::CharData chardt,int x,int y,bool flip,bool tran)
{
	ASSERT(IsLoaded());
	
	bool flipx=chardt.flipx ? true:false;
	flipx=flipx&&flip;
	bool flipy=chardt.flipy ? true:false;
	flipy=flipy&&flip;
	u16 ci;
	if(flip)ci=chardt.tile;else ci=*(u16*)&chardt;
	u8 pali;
	for(int bx=0;bx<8;++bx)for(int by=0;by<8;++by)
	{
		pali=pTile[ci].Get(flipx?7-bx:bx,flipy?7-by:by);
		if(pali||(!tran))pDC->SetPixel(x+bx,y+by,Nitro::Color15to24(Pal[pali|(pPalLine[ci]<<4)]));
	}
}