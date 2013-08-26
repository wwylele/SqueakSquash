#pragma once
#include "Nitro.h"
class SqMa
{
private:
	
	enum{
		MAGIC=0x614D,//"Ma"
	};
public:
	struct GridData
	{
		u16 gra[3];
		u32 det[3];
		u8 t;
	};
	struct BLOCK_MAPPING
	{
		Nitro::CharData mapping[4];
	};
	struct DOOR
	{
		u8 dt[4];
		u16 x;
		u16 y;
		u8 dst_step;
		u8 unk;
	};

	struct S12H_SCRIPT
	{
		u8 BackgroundScript[6];
		u8 TileTimeDelta[4];
	};
	struct GRA_SCRIPT
	{
		u16 BlockMappingIndex;
		u8 FrameCount;
		u8 BlockMappingPlane;
		u8 TimeDelta[8];

	};
private:
	struct S12HEADER
	{
		S12H_SCRIPT HScript;
		u16 GraScriptCount;
		//GRA_SCRIPT GraScript[GraScriptCount];
	};
	
	struct tMapAttribute
	{

		u8 LevelD;
		u8 StageD;
		u16 x06;//=0x0101
		u8 Boss;
		u8 x09;//?
		u8 x0A;//?
		u8 Bgm;
	};
	struct Header
	{
		u16 Magic;
		u16 HeaderSize;//=0x44
		tMapAttribute Attribute;
		u32 SectionOff[13];
		u32 x40;//=0
	};
public:
	SqMa(void);
	~SqMa(void);
	
	bool Load(const u8* psrc);
	inline bool IsLoaded(){return pGrid!=0;}
	void Unload();
	u32 MakeLen();
	void Make(u8* pdst);

	//Section -1
	tMapAttribute MapAttribute;

	//Section 3~8~9
	inline GridData & Grid(u8 x,u8 y)
	{
		ASSERT(IsLoaded());
		ASSERT(x<w);
		ASSERT(y<h);
		return pGrid[x+y*w];
	}

	//Section 0
	inline u8 GetW(){ASSERT(IsLoaded());return w;}
	inline u8 GetH(){ASSERT(IsLoaded());return h;}

	//Section1~2
	inline BLOCK_MAPPING& BlockMappingA(u16 i)
	{
		ASSERT(IsLoaded());
		ASSERT(i<BlockMappingCountA);
		return pBlockMappingA[i];
	}
	inline BLOCK_MAPPING& BlockMappingB(u16 i)
	{
		ASSERT(IsLoaded());
		ASSERT(i<BlockMappingCountB);
		return pBlockMappingB[i];
	}
	inline u16 GetBlockMappingACount(){ASSERT(IsLoaded());return BlockMappingCountA;};
	inline u16 GetBlockMappingBCount(){ASSERT(IsLoaded());return BlockMappingCountB;};

	//Section 10
	inline u8* Section10(){ASSERT(IsLoaded());return pSection10;}

	//Section 11
	inline u16 GetDoorCount(){ASSERT(IsLoaded());return DoorCount;}
	inline DOOR &Door(u16 i)
	{
		ASSERT(IsLoaded());
		ASSERT(i<DoorCount);
		return pDoor[i];
	}

	//Section12
	S12H_SCRIPT S12HScript;
	inline u16 GetGraScriptCount(){ASSERT(IsLoaded());return GraScriptCount;}
	inline GRA_SCRIPT &GraScript(u16 i)
	{
		ASSERT(IsLoaded());
		ASSERT(i<GraScriptCount);
		return pGraScript[i];
	}

	//Section9
	u32 s9exl;
	u8* s9exp;

private:
	//Section 0
	u8 w,h;

	//Section 1~2
	u16 BlockMappingCountA,BlockMappingCountB;
	BLOCK_MAPPING *pBlockMappingA,*pBlockMappingB;

	//Section 3~8~9
	GridData *pGrid;


	//Section 10
	u8 *pSection10;

	//Section 11
	u16 DoorCount;
	DOOR* pDoor;

	//Section 12
	u16 GraScriptCount;
	GRA_SCRIPT* pGraScript;

	
};