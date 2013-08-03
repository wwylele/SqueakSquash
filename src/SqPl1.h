#pragma once
#include "Nitro.h"

class SqPl1
{
public:
	SqPl1(void);
	~SqPl1(void);
	bool Load(const u8* psrc);
	inline bool IsLoaded(){return loaded;}
	void Unload();
	u32 MakeLen();
	void Make(u8* pdst);
	struct PlFrame
	{
		u8 Time;
		Nitro::Color15 Pl[16];
	};
	inline u8 GetFrameCount(u8 Line)
	{
		ASSERT(loaded);
		ASSERT(Line<16);
		return PlLineList[Line].FrameCount;
	}
	inline PlFrame& GetFrame(u8 Line,u8 FrameIdx)
	{
		ASSERT(loaded);
		ASSERT(Line<16);
		ASSERT(FrameIdx<PlLineList[Line].FrameCount);
		return PlLineList[Line].FrameList[FrameIdx];
	}
	
private:
	bool loaded;
	struct PlLine
	{
		u8 FrameCount;
		PlFrame* FrameList;//PlFrame FrameList[FrameCount]
	} PlLineList[16];//for each pl line.FrameCount=0 means no pl

};