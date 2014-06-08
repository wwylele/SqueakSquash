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

struct MCTRL_EXT_0{
	u8 RelMctrlCount_LSL4;
	u8 RelMctrlIndex[3];
};//OK
struct MCTRL_EXT_1{
	u8 RelMctrlCount_LSL4_OR_Style;
	/*Style:
		0 wo
		1 ?
		2 st
	*/
#define MCTRL_EXT_1_STYLE_MASK 0x0F
#define MCTRL_EXT_1_RELCOUNT_MASK 0xF0
	u8 RopeLength_LSL4;
	u8 RelMctrlIndex[2];
	u16 Dummy;//=0x00FF
};//OK
struct MCTRL_EXT_2{
	u8 Style;//?
#define MCTRL_EXT_2_STYLE_MAIN_MASK 0x07
#define MCTRL_EXT_2_STYLE_REVERSE 0x08
#define MCTRL_EXT_2_STYLE_EX_MASK 0xF0
	/*
	0x00 - ¡üClose->Open hold? 
	0x01 - ¡úClose->Open hold? 
	0x02 - ¡ûClose->Open hold? 
	0x80 - ¡üClose->Open
	0x81 - ¡úClose->Open
	0x82 - ¡ûClose->Open
	0x83 - ¡üc On->Off
	0x84 - ¡ýc On->Off
	0x85 - ¡úc On->Off
	0x86 - ¡ûc On->Off
	0x88 - ¡üOpen->Close
	0x89 - ¡úOpen->Close
	0x8A - ¡ûOpen->Close
	0x8B - ¡üc Off->On
	0x8C - ¡ýc Off->On
	0x8D - ¡úc Off->On
	0x8E - ¡ûc Off->On
	...
	*/
	u8 HoldTime;//?
};//OK
struct MCTRL_EXT_3{
	u16 Style;/*=
			  0x0000
			  0x0010 - fall
			  0x0011 - fall?
			  */
#define MCTRL_EXT_3_FLAG_UNK  0x0001
#define MCTRL_EXT_3_FLAG_FALL 0x0010
};//OK
struct MCTRL_EXT_4{
	u8 Style;//?
	u8 NodeCount_LSL4_OR_SubStyle;
	s8 dx0,dy0;
	s8 dx1,dy1;
	s8 dx2,dy2;
};
struct MCTRL_EXT_5{
	u8 Style;
	/*
	0x00 -Still
	0x10 -Step on
	0x90 -Break
	0xA0 -Auto
	*/
	u8 NodeCount_LSL4_OR_SubStyle;
	/*
	SubStyle:
	0x0 -normal?
	0x1 -loop
	0x5 -move,over 
	*/

	u8 Const[4];//1E 00 1E 00
	s8 dx0,dy0;
	s8 dx1,dy1;
	s8 dx2,dy2;
};//OK
#define MCTRL_EXT_5_SUBSTYLE_MASK 0x0F
#define MCTRL_EXT_5_CODECOUNT_MASK 0xF0
struct MCTRL_EXT_6{
	u16 Style;/*=
			  0x0000 waiting
			  0x0010 rolling
			  0x0050 second
			  0x0080 first
			  0x0090 ->_<-
			  */
};//OK
struct MCTRL_EXT_7{//???
	u8 StateA;
	u8 TimeA;
	u8 StateB;
	u8 TimeB;
};
struct MCTRL_EXT_9{u16 Dummy/*=0*/;};
struct MCTRL_EXT_10{
	u8 Unk;//0x00,0x80,0xC0
	u8 Time;
};
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
};//OK
struct MCTRL_EXT_12{
	u8 DestStep;
	u8 DestDoor;
};//OK
struct MCTRL_EXT_13{
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
struct MCTRL_EXT_14{
	u8 IsSup_LSL4_OR_Style;
#define MCTRL_EXT_14_ISSUP 0x10
#define MCTRL_EXT_14_STYLE_MASK 0x0F
	/* Style:
	0 - Grass
	1 - Grass G
	2 - Bush
	3 - Bush G
	4 - Leave
	5 - Leave G
	*/
	u8 RelFoeSupIndex;
};//OK
struct MCTRL_EXT_15{
	u8 IsSup_LSL4;
#define MCTRL_EXT_15_ISSUP 0x10
	u8 RelFoeSupIndex;
};//OK
struct MCTRL_EXT_16{u16 Dummy/*=0xCCCC*/;};
struct MCTRL_EXT_17{
	u8 IsChain_LSL4_OR_IsSup;
#define MCTRL_EXT_17_ISSUP 0x01
#define MCTRL_EXT_17_DUST 0x10
	u8 RelFoeSupIndex;
};//OK
struct MCTRL_EXT_19{
	u8 a,b,c;
	u8 RelMctrlCount_LSL6_OR_Unk1;
	u8 RelMctrlIndex[3];
	u8 Dummy;//=0

};
struct MCTRL_EXT_21{
	u8 Unk0;
	u8 RelMctrlCount_LSL6_OR_Unk1;
	u8 RelMctrlIndex[3];
	u8 Dummy;//=0
};
struct MCTRL_EXT_22{
	u8 RelMctrlCount_OR_WaitBoss;
#define MCTRL_EXT_22_WAITBOSS 0x80 
#define MCTRL_EXT_22_RELMCTRLCOUNT_MASK 0x0F
	u8 RelMctrlIndex[5];
	u16 TimeDelta[5];
};//OK
struct MCTRL_EXT_23{
	u32 Const;//=0x001E0000
	u8 RelMctrlIndex[8];
};//OK
struct MCTRL_EXT_24{
	u16 Style;/*
			  0x0001 - Snow
			  0x0000 - Cloud Wind Down
			  0x0010 - Cloud Wind Up
			  0x0020 - Cloud Wind Right
			  0x0030 - Cloud Wind Left
			  0x0100 - Wind Down
			  0x0110 - Wind Up
			  0x0120 - Wind Right
			  0x0130 - Wind Left
			  */
	u8 Width;
	u8 Height;
};//OK
struct MCTRL_EXT_25{
	u16 Style;/*
			  0 fire
			  1 bubble
			  */
};//OK