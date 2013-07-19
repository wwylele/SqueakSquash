#pragma once

class SqMa
{
private:
	struct GridData
	{
		u16 gra[3];
		u32 det[3];
	};
	enum{
		MAGIC=0x614D,//"Ma"
	};
public:
	struct BLOCK_MAPPING
	{
		u16 mapping[4];
	};
	struct DOOR
	{
		u8 dt[4];
		u16 x;
		u16 y;
		u8 dst_step;
		u8 unk;
	};
	
	struct Header
	{
		u16 Magic;
		u16 HeaderSize;//=0x44
		u8 LevelD;
		u8 StageD;
		u16 x06;//=0x0101
		u8 Boss;
		u8 x09;//?
		u8 x0A;//?
		u8 Bgm;
		u32 SectionOff[13];
		u32 x40;//=0
	};
	SqMa(void);
	~SqMa(void);
	
	bool Load(const u8* psrc);
	inline bool IsLoaded(){return pGrid!=0;}
	void Unload();

	inline GridData & Grid(u8 x,u8 y)
	{
		ASSERT(IsLoaded());
		ASSERT(x<w);
		ASSERT(y<h);
		return pGrid[x+y*w];
	}
	inline u8 GetW(){ASSERT(IsLoaded());return w;}
	inline u8 GetH(){ASSERT(IsLoaded());return h;}
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
	inline u8* Section10(){ASSERT(IsLoaded());return pSection10;}
	inline u16 GetDoorCount(){ASSERT(IsLoaded());return DoorCount;}
	inline DOOR &Door(u16 i)
	{
		ASSERT(IsLoaded());
		ASSERT(i<DoorCount);
		return pDoor[i];
	}

private:
	u8 w,h;
	GridData *pGrid;
	u16 BlockMappingCountA,BlockMappingCountB;
	BLOCK_MAPPING *pBlockMappingA,*pBlockMappingB;
	u8 *pSection10;
	u16 DoorCount;
	DOOR* pDoor;
	
};