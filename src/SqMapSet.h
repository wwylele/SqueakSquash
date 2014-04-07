#pragma once
#include "Nitro.h"

#define ROM_LOCK_FILE "wwylele_LOCK"
#define TEXTURE__CHAINBOMB "chainbomb.bin"
class SqMapSet
{
private:
	//
	//SQMS File Data Structure
	//
	struct SqmsFileHeader/*V1*/
	{
		u8 Magic[8];//="SQSQMAPS"
		u32 Version;
		u32 SectionBgOffset;//ptr to SqmsSectionHeader<Bg>
		u32 SectionGlOffset;//ptr to SqmsSectionHeader<Gl>
		u32 SectionPlOffset;//ptr to SqmsSectionHeader<Pl>
		u32 RomInfoOffset;//ptr to SqmsRomInfo
		u32 StageCount;
		//SqmsStageHeader StageHeader[StageCount];
	};
	struct SqmsStageHeader
	{
		u8 EntryLevel;
		u8 EntryStage;
		u16 StepCount;
		u32 StepTableOffset;//ptr to SqmsStepHeader[StepCount]
	};
	struct SqmsStepHeader
	{
		u32 MxpLen;
		u32 MxpOffset;//ptr to .mxp Data
		u32 DoeLen;
		u32 DoeOffset;//ptr to .doe Data
		u8 BgId;
		u8 BGlId;
		u8 FGlId;
		u8 PlId;
	};
	
	struct SqmsSectionHeader
	{
		u8 Magic[4];//="SQBG","SQGL","SQPL"
		u32 Count;
		//SqmsSecitemHeader SecitemHeader[Count];
	};
	struct SqmsSecitemHeader
	{
		u8 Name[16];
		u32 DataLen;
		u32 DataOffset;//ptr to .nsbtx,.bin,.pal Data
	};
	

public:
	struct SqmsRomInfo
	{
		u8 Rom_name[12];
		u8 Rom_id[4];
		u8 Rom_maker_code[2];
		u8 Rom_product_code;
		u8 Rom_device_type;
		u8 Rom_device_caps;
		u8 Rom_version;

		u8 Title_icon_pixel
			[/*block_y*/4]
			[/*block_x*/4]
			[/*pixel_y*/8]
			[/*pixel_x/2*/4];//4 bit per pixel
		Nitro::Color15 Title_icon_palette[16];
		u16 Title_text[6][128];//Unicode
	};

private:

	//
	//SQMS Data
	//
	bool m_Loaded;
	struct SecitemData
	{
		u8 Name[16];
		u32 DataLen;
		u8* pData;
	};
	/*u32 m_BgCount;
	SecitemData* m_BgList;
	u32 m_GlCount;
	SecitemData* m_GlList;
	u32 m_PlCount;
	SecitemData* m_PlList;*/
	u32 m_SecitemCount[3];
	#define m_BgCount m_SecitemCount[0]
	#define m_GlCount m_SecitemCount[1]
	#define m_PlCount m_SecitemCount[2]
	SecitemData* m_SecitemList[3];
	#define m_BgList m_SecitemList[0]
	#define m_GlList m_SecitemList[1]
	#define m_PlList m_SecitemList[2]
	
	u32 m_StageCount;
	struct StageData
	{
		u8 EntryLevel;
		u8 EntryStage;
		u16 StepCount;
		struct StepData
		{
			u32 MxpLen;
			u8* pMxp;
			u32 DoeLen;
			u8* pDoe;
			u8 BgId;
			u8 BGlId;
			u8 FGlId;
			u8 PlId;
		}*StepList;
	}* m_StageList;
public:
	SqmsRomInfo m_RomInfo;

private:
	//
	//
	//
	static void BgDef(CList<CStringA> &bglist);
	static void GlDef(CList<CStringA> &gllist);
	static void PlDef(CList<CStringA> &pllist);

public:
	SqMapSet(void);
	~SqMapSet(void);

	void Unload();
	bool Load(CFile &file);
	bool Save(CFile &file);
	bool LoadFromRom(CFile &file);
	bool MakeRom(CFile &file);
	void Dump(FILE* pf);
	inline bool IsLoaded(){return m_Loaded;}

public:

	u32 FindSecitem(u8 *GetSiSwitch,const char* pname);//return 0xFFFFFFFF means not found

	inline u32 GetSecitemCount(u8 SiSwitch)
	{
		ASSERT(m_Loaded);
		ASSERT(SiSwitch<3);
		return m_SecitemCount[SiSwitch];
	}
	u8 *GetSecitemBuffer(u8 SiSwitch,u32 index,u32* pGetLen);
	u8 *ResizeSecitemBuffer(u8 SiSwitch,u32 index,u32 Len);
	void GetSecitemName(u8 SiSwitch,u32 index,char *pname);
	void SetSecitemName(u8 SiSwitch,u32 index,const char *pname);
	u8 *NewSecitem(u8 SiSwitch,u32 Len,char *pname);
	void DeleteSecitem(u8 SiSwitch,u32 index);
	bool IsSecitemImportant(u8 SiSwitch,u32 index);

	#define GetBgCount() GetSecitemCount(0)
	#define GetGlCount() GetSecitemCount(1)
	#define GetPlCount() GetSecitemCount(2)
	#define GetBgBuffer(index,pGetLen) GetSecitemBuffer(0,index,pGetLen)
	#define GetGlBuffer(index,pGetLen) GetSecitemBuffer(1,index,pGetLen)
	#define GetPlBuffer(index,pGetLen) GetSecitemBuffer(2,index,pGetLen)
	#define ResizeBgBuffer(index,Len) ResizeSecitemBuffer(0,index,Len)
	#define ResizeGlBuffer(index,Len) ResizeSecitemBuffer(1,index,Len)
	#define ResizePlBuffer(index,Len) ResizeSecitemBuffer(2,index,Len)
	#define GetBgName(index,pname) GetSecitemName(0,index,pname)
	#define GetGlName(index,pname) GetSecitemName(1,index,pname)
	#define GetPlName(index,pname) GetSecitemName(2,index,pname)
	#define SetBgName(index,pname) SetSecitemName(0,index,pname)
	#define SetGlName(index,pname) SetSecitemName(1,index,pname)
	#define SetPlName(index,pname) SetSecitemName(2,index,pname)
	#define NewBg(Len,pname) NewSecitem(0,Len,pname)
	#define NewGl(Len,pname) NewSecitem(1,Len,pname)
	#define NewPl(Len,pname) NewSecitem(2,Len,pname)
	#define DeleteBg(index) DeleteSecitem(0,index)
	#define DeleteGl(index) DeleteSecitem(1,index)
	#define DeletePl(index) DeleteSecitem(2,index)


	inline u32 GetStageCount(){ASSERT(m_Loaded);return m_StageCount;}
	void GetStageEntry(u32 StageIdx,u8 *pEntryLevel,u8 *pEntryStage);

	//Return 0xFFFFFFFF means not find the stage
	u32 GetStageIndex(u8 EntryLevel,u8 EntryStage);
	
	inline u16 GetStepCount(u32 StageIdx)
	{
		ASSERT(m_Loaded);
		ASSERT(StageIdx<m_StageCount);
		return m_StageList[StageIdx].StepCount;
	}
	u8 *GetMxpBuffer(u32 StageIdx,u16 StepIndex,u32* pGetLen=0);
	u8 *GetDoeBuffer(u32 StageIdx,u16 StepIndex,u32* pGetLen=0);
	u8 *ResizeMxpBuffer(u32 StageIdx,u16 StepIndex,u32 Len);
	u8 *ResizeDoeBuffer(u32 StageIdx,u16 StepIndex,u32 Len);
	void DeleteStep(u32 StageIdx,u16 StepIndex);
	u16/*StepIndex*/ NewStep(u32 StageIdx);
	void RepairMxpHeader(u32 StageIdx=0xFFFFFFFF,u16 StepIdx=0xFFFF);
	void MoveStep(u32 StageIdx,u16 StepIndex,u16 NewStepIndex);

	//Bg/BGl/FGl/Pl =0    means not to get the value
	void GetStepInfo(u32 StageIdx,u16 StepIndex,u8* Bg,u8* BGl,u8* FGl,u8* Pl);
	//Bg/BGl/FGl/Pl =0xFE means not to set the value
	void SetStepInfo(u32 StageIdx,u16 StepIndex,u8  Bg,u8  BGl,u8  FGl,u8  Pl);


	u32 GetSubFileCount();

	static u8* CreateGl(u32* pGetLen);

};
