#pragma once
#include "Nitro.h"

class SqMa
{
private:
	
	enum{
		MAGIC=0x614D,//"Ma"
	};
public:
	SQROM_STRUCT CellData
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
	SQROM_STRUCT TEX_MAPPING
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
		u16 TexMappingIndex;
		u8 FrameCount;
		u8 TexMappingPlane;
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

		u8 EntryLevel_ZB;
		u8 EntryStage_ZB;
		u16 Version;//=0x0101
		u8 Boss;
		u8 x09;//?
		u8 x0A;//?
		u8 Bgm;
	};
public:
	SQROM_STRUCT Header
	{
		u16 Magic;
		u16 HeaderSize;//=0x44
		tMapAttribute Attribute;
		u32 SectionOff[13];
		u32 x40;//=0
	};
private:
	//struct Section10
	//{
		//u16 SectionLen;
		
		SQROM_STRUCT S10HEADER
		{
			u16 MctrlListOff;//=8
			u16 MctrlGroupListOff;
			u16 x04;//=0
			u8 MctrlCount;
			u8 MctrlGroupCount;
		}/*Header*/;
		
		SQROM_STRUCT MctrlData
		{
			u8 class_id;
			u8 id;
			u16 x;
			u16 y;
			u16 param;
		}/*MctrlList[MctrlCount]*/;

		//u16 MctrlGroupEntry[MctrlGroupCount];

		SQROM_STRUCT MctrlGroupHeader
		{
			u8 class_id;
			u8 count;
			u16 data_len;
			//u8[count][data_len] data;
		}/*MctrlGroup[MctrlGroupCount]*/;
	//};

	struct MctrlGroupData
	{
		MctrlGroupHeader Header;
		u8 *pData;
	};


	
public:
	SqMa(void);
	~SqMa(void);
	
	bool Load(const u8* psrc);
	void LoadDefault();
	inline bool IsLoaded(){return pCell!=0;}
	void Unload();
	u32 MakeLen();
	void Make(u8* pdst);

private:u16 S10MakeLen();
public:
	//Section -1
	tMapAttribute MapAttribute;

	//Section 3~8~9
	inline CellData& Cell(u8 x,u8 y)
	{
		ASSERT(IsLoaded());
		ASSERT(x<w);
		ASSERT(y<h);
		return pCell[x+y*w];
	}

	//Section 0
	inline u8 GetW(){ASSERT(IsLoaded());return w;}
	inline u8 GetH(){ASSERT(IsLoaded());return h;}

	//Section1~2
	inline TEX_MAPPING& TexMappingA(u16 i,bool ani=false)
	{
		ASSERT(IsLoaded());
		ASSERT(i<TexMappingCountA);
		if(ani)
		{
			for(u16 j=0;j<GraScriptCount;++j)
			{
				if(pGraScript[j].TexMappingIndex==i &&
					pGraScript[j].TexMappingPlane==0)
					return pTexMappingA[i+pGraScriptCurrent[j].CurrentFrame+1];
			}
		}
		return pTexMappingA[i];
	}
	inline TEX_MAPPING& TexMappingB(u16 i,bool ani=false)
	{
		ASSERT(IsLoaded());
		ASSERT(i<TexMappingCountB);
		if(ani)
		{
			for(u16 j=0;j<GraScriptCount;++j)
			{
				if(pGraScript[j].TexMappingIndex==i &&
					pGraScript[j].TexMappingPlane==1)
					return pTexMappingB[i+pGraScriptCurrent[j].CurrentFrame+1];
			}
		}
		return pTexMappingB[i];
	}
	inline u16 GetTexMappingACount(){ASSERT(IsLoaded());return TexMappingCountA;};
	inline u16 GetTexMappingBCount(){ASSERT(IsLoaded());return TexMappingCountB;};

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
	inline u8 GetMctrlGroupCount(){ASSERT(IsLoaded());return MctrlGroupCount;}
	inline u8 GetMctrlGroupId(u8 i){
		ASSERT(IsLoaded());ASSERT(i<MctrlGroupCount);
		return pMctrlGroup[i].Header.class_id;
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
	u16 TexMappingCountA,TexMappingCountB;
	TEX_MAPPING *pTexMappingA,*pTexMappingB;

	//Section 3~8~9
	CellData *pCell;

	//Section 9 
	u16 GuideCount;
	GUIDE_DATA *pGuide;
	u8* pGuideMatrix;


	//Section 10
	u8 MctrlCount;
	MctrlData* pMctrl;
	u8 MctrlGroupCount;
	MctrlGroupData* pMctrlGroup;
	friend class SqMctrlPack;

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