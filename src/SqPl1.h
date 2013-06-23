#pragma once

class SqPl1
{
public:
	SqPl1(void);
	~SqPl1(void);
	bool Load(const u8* psrc);
	bool IsLoaded();
	void Unload();
	struct PlFrame
	{
		u8 Time;
		u16 Pl[16];
	};
	u8 GetFrameCount(u8 Line);
	PlFrame& GetFrame(u8 Line,u8 FrameIdx);
	
private:
	bool loaded;
	struct PlLine
	{
		u8 FrameCount;
		PlFrame* FrameList;//PlFrame FrameList[FrameCount]
	} PlLineList[16];//for each pl line.FrameCount=0 means no pl

};