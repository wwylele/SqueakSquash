#pragma once


class SqMapSet
{
private:
	
	//
	//SQMS File Data Structure
	//
	struct SqmsFileHeader
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
		u8 LevelIdx;
		u8 StageIdx;
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
		u16 Title_icon_palette[16];
		u16 Title_text/*[6]*/[128];//Unicode
	};

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
	u32 m_BgCount;
	SecitemData* m_BgList;
	u32 m_GlCount;
	SecitemData* m_GlList;
	u32 m_PlCount;
	SecitemData* m_PlList;
	u32 m_StageCount;
	struct StageData
	{
		u8 LevelIdx;
		u8 StageIdx;
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
	SqmsRomInfo m_RomInfo;

public:
	SqMapSet(void);
	~SqMapSet(void);

	void Unload();
	bool Load(CFile &file);
	bool Save(CFile &file);
	bool LoadFromRom(CFile &file);
	bool MakeRom(CFile &file);
	void Dump(FILE* pf);
};
