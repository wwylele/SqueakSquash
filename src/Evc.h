#pragma once
#include "SqMa.h"

#define MAX_EVC_COUNT 27
struct EVC_DESC
{
	u16 DataLen;
	TCHAR *Name;
};
extern const EVC_DESC EvcDesc[];
u16 GetEvcDataLen(u8 class_id);


class SqMctrlPack
{
public:
	struct MctrlNote
	{
		u8 class_id;
		u16 x;
		u16 y;
		u16 param;
		u8* pExtData;
	};
	SqMctrlPack();
	~SqMctrlPack();
	void FromMa(SqMa& sqma);
	void Unload();
	void ToMa(SqMa& sqma);
	inline u8 GetMctrlCount(){return MctrlCount;}
	inline MctrlNote& operator [](u8 i)
	{
		ASSERT(i<MctrlCount);
		return pMctrl[i];
	}
private:
	MctrlNote *pMctrl;
	u8 MctrlCount;

};

struct EVC_EXT_0{
	u8 RelEvpCount_LSL8;
	u8 RelEvpIndex[3];
};
struct EVC_EXT_1{
	u8 RelEvpCount_LSL8_OR_Unk0;
	u8 Unk1;
	u8 RelEvpIndex[2];
	u16 Dummy;//=0x00FF
};
struct EVC_EXT_3{
	u16 Style;/*=
			  0x0000
			  0x0010
			  0x0011
			  */
};
struct EVC_EXT_6{
	u16 Style;/*=
			  0x0000
			  0x0010
			  0x0050
			  0x0080
			  0x0090
			  */
};
struct EVC_EXT_9{u16 Dummy/*=0*/;};
struct EVC_EXT_11{
	u16 Style;/*
			  0
			  1
			  36*1
			  36*2
			  36*3
			  36*4
			  36*5
			  36*6
			  36*7
			  */
};
struct EVC_EXT_16{u16 Dummy/*=0xCCCC*/;};
struct EVC_EXT_17{
	u8 IsSup_OR_IsUnk_LSL8;
	u8 RelFoeSupIndex;//?
};
struct EVC_EXT_21{
	u8 Unk0;
	u8 RelEvpCount_LSL10_OR_Unk;
	u8 RelEvpIndex[3];
	u8 Dummy;//0
};
struct EVC_EXT_22{
	u8 RelEvpCount;
	u8 RelEvpIndex[5];
	u16 TimeDelta[5];
};
struct EVC_EXT_23{
	u32 Const;//=0x001E0000
	u8 RelEvpIndex[8];
};
struct EVC_EXT_25{u16 Unk/*=0,1*/;};