#pragma once


class SqBase
{
public:
	virtual bool Load(const u8* psrc)=0;
	virtual bool IsValid()=0;
	bool LoadFromFile(LPCTSTR fname);
	virtual void Unload()=0;

};


class SqMx:public SqBase
{
public:
	SqMx(void);
	~SqMx(void);

	virtual bool Load(const u8* psrc);
	virtual bool IsValid();
	virtual void Unload();
	struct StepInfo
	{
		CStringA Ma;
		CStringA Fb;
		CStringA Bb;
		CStringA De;
		CStringA Pl;
		CStringA Bg;
	};
	StepInfo &Step(u16 i);
	u16 GetStepCount();
private:
	u16 StepCount;
	StepInfo *pStep;
	struct Header
	{
		u16 Magic;
		u16 HeaderSize;//=12
		u16 DataOffset;//=12
		u16 x06;//=0
		u8 LevelDec;
		u8 StageDec;
		u16 StepCount;
	};
	enum{
		MAGIC=0x584D,//"MX"
	};

};
