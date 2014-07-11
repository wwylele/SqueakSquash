#pragma once
#include "MaDeComm.h"

#define MAX_EVC_COUNT 27
#define MAX_MCTRL_COUNT 250
struct EVC_DESC
{
	u16 DataLen;
	TCHAR *Name;

	u8 RelMctrlIndex_Offset;
	u8 RelMctrlIndex_Count;
};
extern const EVC_DESC EvcDesc[];
extern const u8 MctrlDefault[64][27];
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
	void DownMctrl(u8 i);

	MaDeComm **ppMaDeComm;
private:
	MctrlNote *pMctrl;
	u8 MctrlCount;

};

void DecodeMctrlParam(u8 class_id,u8 *param,CString &str);

struct MCTRL_PARAM_ENUM
{
	u16 value;
#define MCTRL_PARAM_ENUM_END 0xFFFF
	TCHAR *name;
};
int MpeV2I(u16 value,const MCTRL_PARAM_ENUM* mpe);

SQROM_STRUCT MCTRL_EXT_0{
	u8 Const_0:4;
	u8 RelMctrlCount:4;
	u8 RelMctrlIndex[3];
};//OK

SQROM_STRUCT MCTRL_EXT_1{
	u8 Style:4;
	u8 RelMctrlCount:4;
	u8 Const_0:4;
	u8 RopeLength:4;
	u8 RelMctrlIndex[2];
	u8 Const_FF;
	u8 Const_00;
};//OK
extern const MCTRL_PARAM_ENUM MCTRL_EXT_1_STYLE[];

SQROM_STRUCT MCTRL_EXT_2{
	u8 MainStyle:3;
	u8 Reverse:1;
	u8 ExStyle:4;
	u8 HoldTime:8;
};//OK
extern const MCTRL_PARAM_ENUM MCTRL_EXT_2_MAIN_STYLE[];
extern const MCTRL_PARAM_ENUM MCTRL_EXT_2_EX_STYLE[];

SQROM_STRUCT MCTRL_EXT_3{
	u8 Flag_Unk:4;
	u8 Flag_Fall:4;
	u8 Const_00:8;
};//OK

SQROM_STRUCT MCTRL_EXT_4{
	u8 Style;//?
	u8 NodeCount_LSL4_OR_SubStyle;
	struct MCTRL_EXT_4_NODE{
		s8 dx;
		s8 dy;
	}Node[3];
};
SQROM_STRUCT MCTRL_EXT_5{
	u8 Style:8;
	u8 SubStyle:4;
	u8 NodeCount:4;

	u8 Const[4];//1E 00 1E 00
	struct MCTRL_EXT_5_NODE{
		s8 dx;
		s8 dy;
	}Node[3];
};//OK
extern const MCTRL_PARAM_ENUM MCTRL_EXT_5_STYLE[];
extern const MCTRL_PARAM_ENUM MCTRL_EXT_5_SUB_STYLE[];

SQROM_STRUCT MCTRL_EXT_6{
	u16 Style;
};//OK
extern const MCTRL_PARAM_ENUM MCTRL_EXT_6_STYLE[];

SQROM_STRUCT MCTRL_EXT_7{//???
	u8 StateA;
	u8 TimeA;
	u8 StateB;
	u8 TimeB;
};
SQROM_STRUCT MCTRL_EXT_9{u16 Dummy/*=0*/;};
SQROM_STRUCT MCTRL_EXT_10{
	u8 Style;//0x00,0x80,0xC0
	u8 Time;
};
extern const MCTRL_PARAM_ENUM MCTRL_EXT_10_STYLE[];
SQROM_STRUCT MCTRL_EXT_11{
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
};//OK
SQROM_STRUCT MCTRL_EXT_12{
	u8 DestStep;
	u8 DestDoor;
};//OK
SQROM_STRUCT MCTRL_EXT_13{
	u8 DestStep;//? 0xFE=Finish?
	u8 Unk;
	u16 Sound_OR_NodeCount;//? NodeCount exclude "0x20"Node
	/* Sound ?
	0x0008 - ?
	0x0028 - ?
	0x0048 - ?
	0x0068 - ?
	0x00A8 - ?
	*/
	struct MCTRL_EXT_13_NODE{
		u16 Type;
		/*
		0x00 - None
		0x01
		0x11
		0x20
		0x21
		0x31
		*/
		u16 TimeDelta;
		u16 ToX,ToY;
		u16 CtrlX,CtrlY;
	}Node[5];
};
SQROM_STRUCT MCTRL_EXT_14{
	u8 Style:4;
	u8 IsSup:4;
	u8 RelFoeSupIndex:8;
};//OK
extern const MCTRL_PARAM_ENUM MCTRL_EXT_14_STYLE[];

SQROM_STRUCT MCTRL_EXT_15{
	u8 IsSup_LSL4;
#define MCTRL_EXT_15_ISSUP 0x10
	u8 RelFoeSupIndex;
};//OK
SQROM_STRUCT MCTRL_EXT_16{u16 Dummy/*=0xCCCC*/;};//OK
SQROM_STRUCT MCTRL_EXT_17{
	u8 IsSup:4;
	u8 Dust:4;
	u8 RelFoeSupIndex;
};//OK
SQROM_STRUCT MCTRL_EXT_19{
	u8 a,b,c;
	u8 RelMctrlCount_LSL6_OR_Unk1;
	u8 RelMctrlIndex[3];
	u8 Dummy;//=0

};
SQROM_STRUCT MCTRL_EXT_21{
	u8 Unk0;
	u8 RelMctrlCount_LSL6_OR_Unk1;
	u8 RelMctrlIndex[3];
	u8 Dummy;//=0
};
SQROM_STRUCT MCTRL_EXT_22{
	u8 RelMctrlCount:4;
	u8 Const_0:3;
	u8 WaitBoss:1;
	u8 RelMctrlIndex[5];
	u16 TimeDelta[5];
};//OK
SQROM_STRUCT MCTRL_EXT_23{
	u32 Const;//=0x001E0000
	u8 RelMctrlIndex[8];
};//OK

SQROM_STRUCT MCTRL_EXT_24{
	u16 Style;
	u8 Width;
	u8 Height;
};//OK
extern const MCTRL_PARAM_ENUM MCTRL_EXT_24_STYLE[];

SQROM_STRUCT MCTRL_EXT_25{
	u16 Style;/*
			  0 fire
			  1 bubble
			  */
};//OK