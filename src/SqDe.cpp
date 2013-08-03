#include "stdafx.h"
#include "SqDe.h"


SqDe::SqDe():pFoe(0),pSup(0)
{
}
SqDe::~SqDe()
{
	Unload();
}
void SqDe::Unload()
{
	if(pFoe)
	{
		delete[] pFoe;
		pFoe=0;
	}
	if(pSup)
	{
		delete[] pSup;
		pSup=0;
	}
}
bool SqDe::Load(const u8* psrc)
{
	Header &head=*(Header*)psrc;
	ASSERT(head.Magic==MAGIC && head.HeaderSize==0x0C);
	Unload();
	psrc+=sizeof(Header);
	FoeCount=*(u16*)psrc;psrc+=2;
	pFoe=new SqItem[FoeCount];
	memcpy(pFoe,psrc,sizeof(SqItem)*FoeCount);psrc+=sizeof(SqItem)*FoeCount;
	SupCount=*(u16*)psrc;psrc+=2;
	pSup=new SqItem[SupCount];
	memcpy(pSup,psrc,sizeof(SqItem)*SupCount);

	return true;
}