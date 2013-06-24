#pragma once
COLORREF R5G5B5X1toR8G8B8X8(u16 value);
u16 R8G8B8X8toR5G5B5X1(COLORREF value);
struct TILE
{
	u8 dt[32];
	u8 Get(u8 x,u8 y);
	
	void Set(u8 x,u8 y,u8 value);
	bool operator ==(TILE& value);
};
class SqB
{
public:
	SqB(void);
	~SqB(void);
	bool Load(const u8* psrc);
	bool IsLoaded();
	void Unload();

	u16 Pal[0x100];
	TILE& Tile(u32 i);
	u8& PalLine(u32 i);
	u32 GetTileCount();

	void DrawTile(CDC* pDC,u16 chardt,int x,int y,
		bool flip=false,bool transparent=false);
private:
	
	u32 TileCount;
	TILE *pTile;
	u8 *pPalLine;
	
};