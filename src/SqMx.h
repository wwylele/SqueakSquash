#pragma once




class SqMx
{
public:
	SqMx(void);
	~SqMx(void);

	bool Load(const u8* psrc);
	void Create(u16 StepCount);
	bool IsLoaded();
	void Unload();
	void Make(u8 **ppData,u32 *pLen);
	struct StepInfo
	{
		CStringA Ma;
		CStringA Fb;
		CStringA Bb;
		CStringA De;
		CStringA Pl;
		CStringA Bg;//"3D"
	};
	StepInfo &Step(u16 i);
	u16 GetStepCount();
	u8 EntryLevel;
	u8 EntryStage;
private:
	u16 StepCount;
	StepInfo *pStep;
	SQROM_STRUCT Header
	{
		u16 Magic;
		u16 HeaderSize;//=12
		u16 DataOffset;//=12
		u16 x06;//=0
		u8 EntryLevel_ZB;
		u8 EntryStage_ZB;
		u16 StepCount;
	};
	enum{
		MAGIC=0x584D,//"MX"
	};

};
