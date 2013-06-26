#pragma once
#include "Nitro.h"



class SqB
{
public:
	SqB(void);
	~SqB(void);
	bool Load(const u8* psrc);
	inline bool IsLoaded(){return pTile!=0;}
	void Unload();

	Nitro::Color15 Pal[0x100];
	inline Nitro::Tile& Tile(u32 i){ASSERT(IsLoaded());return pTile[i];}
	inline u8& PalLine(u32 i){ASSERT(IsLoaded());return pPalLine[i];}
	inline u32 GetTileCount(){ASSERT(IsLoaded());return TileCount;}

	void DrawTile(CDC* pDC,u16 chardt,int x,int y,
		bool flip=false,bool transparent=false);
private:
	
	u32 TileCount;
	Nitro::Tile *pTile;
	u8 *pPalLine;
	
};