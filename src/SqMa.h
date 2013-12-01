#pragma once
#include "Nitro.h"

class SqMa
{
private:
	
	enum{
		MAGIC=0x614D,//"Ma"
	};
public:
	SQROM_STRUCT GridData
	{
		u16 gra[3];
		u32 det[3];
		u8 guide_id;
	};
	SQROM_STRUCT GUIDE_DATA
	{
		u16 id;
		u16 x;
		u16 y;
		u16 U;
	};
	SQROM_STRUCT BLOCK_MAPPING
	{
		Nitro::CharData mapping[4];
	};
	SQROM_STRUCT DOOR
	{

		u16 index;
		u16 class_id;
		u16 x;
		u16 y;
		u8 dst_step;
		u8 dst_door;
	};

	SQROM_STRUCT S12H_SCRIPT
	{
		u8 BackgroundScript[6];
		u8 TileTimeDelta[4];
	};
	SQROM_STRUCT GRA_SCRIPT
	{
		u16 BlockMappingIndex;
		u8 FrameCount;
		u8 BlockMappingPlane;
		u8 TimeDelta[8];

	};
private:
	SQROM_STRUCT S12HEADER
	{
		S12H_SCRIPT HScript;
		u16 GraScriptCount;
		//GRA_SCRIPT GraScript[GraScriptCount];
	};
	
	SQROM_STRUCT tMapAttribute
	{

		u8 LevelD;
		u8 StageD;
		u16 Version;//=0x0101
		u8 Boss;
		u8 x09;//?
		u8 x0A;//?
		u8 Bgm;
	};
	SQROM_STRUCT Header
	{
		u16 Magic;
		u16 HeaderSize;//=0x44
		tMapAttribute Attribute;
		u32 SectionOff[13];
		u32 x40;//=0
	};

	//struct Section10
	//{
		//u16 SectionLen;
		
		SQROM_STRUCT S10HEADER
		{
			u16 ComponentListOff;//=8
			u16 ComponentGroupListOff;
			u16 x04;//=0
			u8 ComponentCount;
			u8 ComponentGroupCount;
		}/*Header*/;
		
		SQROM_STRUCT ComponentData
		{
			u8 class_id;
			u8 id;
			u16 x;
			u16 y;
			u16 param;
		}/*ComponentList[ComponentCount]*/;

		//u16 ComponentGroupEntry[ComponentGroupCount];

		SQROM_STRUCT ComponentGroupHeader
		{
			u8 class_id;
			u8 count;
			u16 data_len;
			//u8[count][data_len] data;
		}/*ComponentGroup[ComponentGroupCount]*/;
	//};

	struct ComponentGroupData
	{
		ComponentGroupHeader Header;
		u8 *pData;
	};


	
public:
	SqMa(void);
	~SqMa(void);
	
	bool Load(const u8* psrc);
	inline bool IsLoaded(){return pGrid!=0;}
	void Unload();
	u32 MakeLen();
	void Make(u8* pdst);

private:u16 S10MakeLen();
public:
	//Section -1
	tMapAttribute MapAttribute;

	//Section 3~8~9
	inline GridData & Cell(u8 x,u8 y)
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
	inline BLOCK_MAPPING& BlockMappingA(u16 i,bool ani=false)
	{
		ASSERT(IsLoaded());
		ASSERT(i<BlockMappingCountA);
		if(ani)
		{
			for(u16 j=0;j<GraScriptCount;++j)
			{
				if(pGraScript[j].BlockMappingIndex==i &&
					pGraScript[j].BlockMappingPlane==0)
					return pBlockMappingA[i+pGraScriptCurrent[j].CurrentFrame+1];
			}
		}
		return pBlockMappingA[i];
	}
	inline BLOCK_MAPPING& BlockMappingB(u16 i,bool ani=false)
	{
		ASSERT(IsLoaded());
		ASSERT(i<BlockMappingCountB);
		if(ani)
		{
			for(u16 j=0;j<GraScriptCount;++j)
			{
				if(pGraScript[j].BlockMappingIndex==i &&
					pGraScript[j].BlockMappingPlane==1)
					return pBlockMappingB[i+pGraScriptCurrent[j].CurrentFrame+1];
			}
		}
		return pBlockMappingB[i];
	}
	inline u16 GetBlockMappingACount(){ASSERT(IsLoaded());return BlockMappingCountA;};
	inline u16 GetBlockMappingBCount(){ASSERT(IsLoaded());return BlockMappingCountB;};

	//Section 9 ex
	inline u16 GetGuideCount(){ASSERT(IsLoaded());return GuideCount;}
	inline GUIDE_DATA& Guide(u16 i){
		ASSERT(IsLoaded());ASSERT(i<GuideCount);
		return pGuide[i];
	}
	inline u8& GuideMatrix(u16 i){
		ASSERT(IsLoaded());ASSERT(i<GuideCount*GuideCount);
		return pGuideMatrix[i];
	}

	//Section 10
	//inline u8* Section10(){ASSERT(IsLoaded());return pSection10;}
	//inline u8 GetSObjCount(){ASSERT(IsLoaded());return pSection10[8];}
	//inline u8* GetSObj(u8 i){ASSERT(IsLoaded());return pSection10+10+i*8;}
	inline u8 GetComponentGroupCount(){ASSERT(IsLoaded());return ComponentGroupCount;}
	inline u8 GetComponentGroupId(u8 i){
		ASSERT(IsLoaded());ASSERT(i<ComponentGroupCount);
		return pComponentGroup[i].Header.class_id;
	}
	

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
	void TicketClear();
	bool TicketIn();

	

private:
	//Section 0
	u8 w,h;

	//Section 1~2
	u16 BlockMappingCountA,BlockMappingCountB;
	BLOCK_MAPPING *pBlockMappingA,*pBlockMappingB;

	//Section 3~8~9
	GridData *pGrid;

	//Section 9 
	u16 GuideCount;
	GUIDE_DATA *pGuide;
	u8* pGuideMatrix;


	//Section 10
	u8 ComponentCount;
	ComponentData* pComponent;
	u8 ComponentGroupCount;
	ComponentGroupData* pComponentGroup;
	friend class SqComponentPack;

	//Section10 New
	//EvpPack* pEvpPack;


	//Section 11
	u16 DoorCount;
	DOOR* pDoor;

	//Section 12
	u16 GraScriptCount;
	GRA_SCRIPT* pGraScript;
	
	//Section 12 ani
	struct GRA_SCRIPT_CURRENT
	{
		u8 CurrentFrame;
		u8 CurrentTime;
	} *pGraScriptCurrent;
	
};