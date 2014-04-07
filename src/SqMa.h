#pragma once
#include "Nitro.h"
#include "Evc.h"
#include "MaDeComm.h"

#define TEXM_F 0
#define TEXM_B 1

#define MAX_DOOR_COUNT 32

class SqMa
{
private:
	
	enum{
		MAGIC=0x614D,//"Ma"
	};
public:
	SQROM_STRUCT SqCell
	{
		u16 gra[3];
		u32 det[3];
		u8 guide;
	};
	SQROM_STRUCT SqGuide
	{
		u16 index;//?
		u16 x;
		u16 y;
		u16 U;
	};
	SQROM_STRUCT SqTexMapping
	{
		Nitro::CharData mapping[4];
	};
	SQROM_STRUCT SqDoor
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
	SQROM_STRUCT SqGraScript
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
		//SqGraScript GraScript[GraScriptCount];
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

	MaDeComm *pMaDeComm;
	void Notify_Door(MadeCommNOTIFY ncode,u16 index);
	void Notify_Mctrl(MadeCommNOTIFY ncode,u8 index);
	void Notify_Foe(MadeCommNOTIFY ncode,u16 index);
	void Notify_Sup(MadeCommNOTIFY ncode,u16 index);

private:u16 S10MakeLen();
public:
	//Section -1
	tMapAttribute MapAttribute;

	//Section 3~8~9
	inline SqCell& Cell(u8 x,u8 y)
	{
		ASSERT(IsLoaded());
		ASSERT(x<w);
		ASSERT(y<h);
		return pCell[x+y*w];
	}
	void ZeroCell();

	//Section 0
	inline u8 GetW(){ASSERT(IsLoaded());return w;}
	inline u8 GetH(){ASSERT(IsLoaded());return h;}
	void ResizeMap(u8 W,u8 H,int ox=0,int oy=0);

	//Section 1~2
	inline SqTexMapping& TexMapping(u8 TexMPlane,u16 i,bool ani=false)
	{
		ASSERT(IsLoaded());
		if(i>=TexMappingCount[TexMPlane])
		{
			return TexMappingNull;
		}
		if(ani)
		{
			for(u16 j=0;j<GraScriptCount;++j)
			{
				if(pGraScript[j].TexMappingIndex==i &&
					pGraScript[j].TexMappingPlane==TexMPlane)
					return pTexMapping[TexMPlane][i+pGraScriptCurrent[j].CurrentFrame+1];
			}
		}
		return pTexMapping[TexMPlane][i];
	}
	inline u16 GetTexMappingCount(u8 TexMPlane){
		ASSERT(IsLoaded());return TexMappingCount[TexMPlane];
	}
	void CopyTexMapping(SqMa& src,u8 TexMPlane);
	void ResizeTexMapping(u8 TexMPlane,u16 size);

	//Section 9 ex
	inline u16 GetGuideCount(){ASSERT(IsLoaded());return GuideCount;}
	inline SqGuide& Guide(u16 i){
		ASSERT(IsLoaded());ASSERT(i<GuideCount);
		return pGuide[i];
	}
	inline u8& GuideMatrix(u16 i){
		ASSERT(IsLoaded());ASSERT(i<GuideCount*GuideCount);
		return pGuideMatrix[i];
	}
	void RepairGuideIndex();
	void ResizeGuide(u16 count);

	//Section 10
	inline u8 GetMctrlGroupCount(){ASSERT(IsLoaded());return MctrlGroupCount;}
	inline u8 GetMctrlGroupId(u8 i){
		ASSERT(IsLoaded());ASSERT(i<MctrlGroupCount);
		return pMctrlGroup[i].Header.class_id;
	}
	SqMctrlPack MctrlPack;
	void ParseMctrl();
	void SerializeMctrl();
	
	

	//Section 11
	inline u16 GetDoorCount(){ASSERT(IsLoaded());return DoorCount;}
	inline SqDoor &Door(u16 i)
	{
		ASSERT(IsLoaded());
		ASSERT(i<DoorCount);
		return pDoor[i];
	}
	void RepairDoorIndex();
	void RemoveDoor(u16 i);
	u16 NewDoor();

	//Section12
	S12H_SCRIPT S12HScript;
	inline u16 GetGraScriptCount(){ASSERT(IsLoaded());return GraScriptCount;}
	inline SqGraScript &GraScript(u16 i)
	{
		ASSERT(IsLoaded());
		ASSERT(i<GraScriptCount);
		return pGraScript[i];
	}
	void RemoveGraScript(u16 i);
	u16 NewGraScript();

	void TicketClear();
	bool TicketIn();

	

private:
	//Section 0
	u8 w,h;

	//Section 1~2
	u16 TexMappingCount[2];
	SqTexMapping *pTexMapping[2];
	SqTexMapping TexMappingNull;

	//Section 3~8~9
	SqCell *pCell;

	//Section 9 
	u16 GuideCount;
	SqGuide *pGuide;
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
	SqDoor* pDoor;

	//Section 12
	u16 GraScriptCount;
	SqGraScript* pGraScript;
	
	//Section 12 ani
	struct GRA_SCRIPT_CURRENT
	{
		u8 CurrentFrame;
		u8 CurrentTime;
	} *pGraScriptCurrent;
	
};