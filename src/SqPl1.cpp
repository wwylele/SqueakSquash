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
u32 SqPl1::MakeLen()
{
	ASSERT(loaded);
	u32 len=4;
	for(u8 line=0;line<16;++line)if(PlLineList[line].FrameCount)
	{
		len+=2+PlLineList[line].FrameCount*33+(PlLineList[line].FrameCount&1);
	}
	return len;

}
void SqPl1::Make(u8* pdst)
{
	ASSERT(loaded);
	*(u16*)pdst=1;pdst+=2;
	u16& LineCount=*(u16*)pdst;pdst+=2;LineCount=0;
	for(u8 line=0;line<16;++line)if(PlLineList[line].FrameCount)
	{
		++LineCount;
		*(pdst++)=line;
		*(pdst++)=PlLineList[line].FrameCount;
		for(u8 i=0;i<PlLineList[line].FrameCount;++i)
			*(pdst++)=PlLineList[line].FrameList[i].Time;
		if(PlLineList[line].FrameCount&1)*(++pdst)=0;
		for(u8 i=0;i<PlLineList[line].FrameCount;++i)
		{
			memcpy(pdst,PlLineList[line].FrameList[i].Pl,32);pdst+=32;
		}
	}
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

void SqPl1::InsertFrame(u8 Line,u8 FrameIdx)
{
	ASSERT(loaded);
	if(PlLineList[Line].FrameCount==0)
	{
		PlLineList[Line].FrameCount=1;
		PlLineList[Line].FrameList=new PlFrame[1];
		memset(PlLineList[Line].FrameList,0,sizeof(PlFrame));
		PlLineList[Line].FrameList[0].Time=1;
	}
	else
	{
		ASSERT(FrameIdx<PlLineList[Line].FrameCount);
		PlFrame* newFrameList=new PlFrame[++PlLineList[Line].FrameCount];
		memcpy(newFrameList,PlLineList[Line].FrameList,(FrameIdx+1)*sizeof(PlFrame));
		memcpy(newFrameList+FrameIdx+1,PlLineList[Line].FrameList+FrameIdx,
			(PlLineList[Line].FrameCount-FrameIdx-1)*sizeof(PlFrame));
		delete[] PlLineList[Line].FrameList;
		PlLineList[Line].FrameList=newFrameList;
	}
	
}
void SqPl1::DeleteFrame(u8 Line,u8 FrameIdx)
{
	ASSERT(loaded);
	ASSERT(FrameIdx<PlLineList[Line].FrameCount);
	if(PlLineList[Line].FrameCount==1)
	{
		PlLineList[Line].FrameCount=0;
		delete[] PlLineList[Line].FrameList;
		PlLineList[Line].FrameList=0;
	}
	else
	{
		PlFrame* newFrameList=new PlFrame[--PlLineList[Line].FrameCount];
		memcpy(newFrameList,PlLineList[Line].FrameList,FrameIdx*sizeof(PlFrame));
		memcpy(newFrameList+FrameIdx,PlLineList[Line].FrameList+FrameIdx+1,
			(PlLineList[Line].FrameCount-FrameIdx)*sizeof(PlFrame));
		delete[] PlLineList[Line].FrameList;
		PlLineList[Line].FrameList=newFrameList;
	}
}