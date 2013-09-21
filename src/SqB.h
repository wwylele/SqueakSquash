#pragma once
#include "Nitro.h"
#include "SqPl1.h"


class SqB
{
public:
	SqB(void);
	~SqB(void);
	bool Load(const u8* psrc);
	inline bool IsLoaded(){return pTile!=0;}
	void Unload();
	u32 MakeLen();
	void Make(u8* pdst);

	Nitro::Color15 Pal[0x100];
	inline Nitro::Color15 Pal_Ani(u8 line,u8 i)
	{
		ASSERT(line<16 && i<16);
		if(pSqPl1 && pSqPl1->GetFrameCount(line))
			return pSqPl1->GetFrameAni(line)[i];
		else return Pal[(line<<4)|i];
	}
	inline Nitro::Tile& Tile(u32 i,bool ani=false){
		ASSERT(IsLoaded());
		if(ani &&TileCount>0x400 && i>=0x3E0 && i<0x400)
		{
			return pTile[i+(CurrentFrame+1)*32];
		}
		return pTile[i];
	}
	inline u8& PalLine(u32 i,bool ani=false){
		ASSERT(IsLoaded());
		if(ani &&TileCount>0x400 && i>=0x3E0 && i<0x400)
		{
			return pPalLine[i+(CurrentFrame+1)*32];
		}
		return pPalLine[i];
	}
	inline u32 GetTileCount(){ASSERT(IsLoaded());return TileCount;}
	inline bool TicketIn(){
		bool r=false;
		if(TileCount>0x400){
			++CurrentTime;
			if(CurrentTime==TileTimeDelta[CurrentFrame])
			{
				CurrentTime=0;
				++CurrentFrame;
				if(CurrentFrame==4)CurrentFrame=0;
				r=true;
			}
		}
		if(pSqPl1 && pSqPl1->TickedIn())r=true;
		return r;
	}
	inline void TicketClear(){
		CurrentTime=0;CurrentFrame=0;
	}
	u8 TileTimeDelta[4];

	SqPl1 *pSqPl1;

	void DrawTile(CDC* pDC,Nitro::CharData chardt,int x,int y,
		bool flip=false,bool transparent=false);
private:
	
	u32 TileCount;
	Nitro::Tile *pTile;
	u8 *pPalLine;
	
	u8 CurrentTime;
	u8 CurrentFrame;
};