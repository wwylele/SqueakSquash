#pragma once
#include "MaDeComm.h"

#define MAX_EVC_COUNT 27
#define MAX_MCTRL_COUNT 250
struct EVC_DESC
{
	u16 DataLen;
	TCHAR *Name;
};
extern const EVC_DESC EvcDesc[];
u16 GetEvcDataLen(u8 class_id);

class SqMa;

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
	void RemoveMctrl(u8 i);
	u8 NewMctrl();

	MaDeComm **ppMaDeComm;
private:
	MctrlNote *pMctrl;
	u8 MctrlCount;

};

struct MCTRL_EXT_0{
	u8 RelMctrlCount_LSL4;
	u8 RelMctrlIndex[3];
};//OK
struct MCTRL_EXT_1{
	u8 RelMctrlCount_LSL4_OR_Unk0;
	u8 Unk1;
	u8 RelMctrlIndex[2];
	u16 Dummy;//=0x00FF
};
struct MCTRL_EXT_3{
	u16 Style;/*=
			  0x0000
			  0x0010 - fall
			  0x0011 - fall?
			  */
};//OK
#define MCTRL_EXT_3_FLAG_UNK  0x0001
#define MCTRL_EXT_3_FLAG_FALL 0x0010
struct MCTRL_EXT_6{
	u16 Style;/*=
			  0x0000
			  0x0010
			  0x0050
			  0x0080
			  0x0090
			  */
};
struct MCTRL_EXT_9{u16 Dummy/*=0*/;};
struct MCTRL_EXT_11{
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
struct MCTRL_EXT_16{u16 Dummy/*=0xCCCC*/;};
struct MCTRL_EXT_17{
	u8 IsSup_OR_IsUnk_LSL4;
	u8 RelFoeSupIndex;//?
};
struct MCTRL_EXT_22{
	u8 RelMctrlCount;
	u8 RelMctrlIndex[5];
	u16 TimeDelta[5];
};
struct MCTRL_EXT_23{
	u32 Const;//=0x001E0000
	u8 RelMctrlIndex[8];
};
struct MCTRL_EXT_25{
	u16 Style;/*
			  0 fire
			  1 bubble
			  */
};