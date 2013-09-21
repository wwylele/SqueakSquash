#include "stdafx.h"
#include "SqPl1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SqPl1::SqPl1():loaded(false)
{
	ZeroMemory(PlLineList,sizeof(PlLineList));
}

SqPl1::~SqPl1()
{
	Unload();
}
void SqPl1::Unload()
{
	if(!loaded)return;
	for(u8 i=0;i<16;++i)
	{
		if(PlLineList[i].FrameCount)
		{
			delete[] PlLineList[i].FrameList;
		}
	}
	ZeroMemory(PlLineList,sizeof(PlLineList));
}

bool SqPl1::Load(const u8* psrc)
{
	if(*(u16*)psrc!=1)return false;
	psrc+=2;
	Unload();
	u16 LineCount=*(u16*)psrc;psrc+=2;
	ASSERT(LineCount<17);
	u8 line,framecount;
	for(u16 l=0;l<LineCount;++l)
	{
		line=*(psrc++);
		framecount=*(psrc++);
		ASSERT(line<16||!PlLineList[line].FrameCount);
		PlLineList[line].FrameCount=framecount;
		//ASSERT(framecount<17);
		PlLineList[line].FrameList=new PlFrame[framecount];
		for(u8 i=0;i<framecount;++i)PlLineList[line].FrameList[i].Time=*(psrc++);
		if(framecount&1)++psrc;
		for(u8 i=0;i<framecount;++i){memcpy(PlLineList[line].FrameList[i].Pl,psrc,32);psrc+=32;}
	}
	loaded=true;
	TickedClear();
	return true;
}

void SqPl1::TickedClear()
{
	for(u8 i=0;i<16;++i)
	{
		PlLineCurrentList[i].CurrentTime=0;
		PlLineCurrentList[i].CurrentFrame=0;
	}
}
bool SqPl1::TickedIn()
{
	bool r=false;
	for(u8 i=0;i<16;++i)if(PlLineList[i].FrameCount)
	{
		++PlLineCurrentList[i].CurrentTime;
		if(PlLineCurrentList[i].CurrentTime==
			PlLineList[i].FrameList[PlLineCurrentList[i].CurrentFrame].Time)
		{
			PlLineCurrentList[i].CurrentTime=0;
			++PlLineCurrentList[i].CurrentFrame;
			if(PlLineCurrentList[i].CurrentFrame==PlLineList[i].FrameCount)
				PlLineCurrentList[i].CurrentFrame=0;
			r=true;
		}
	}
	return r;
}