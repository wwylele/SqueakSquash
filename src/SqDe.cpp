#include "stdafx.h"
#include "SqDe.h"


SqDe::SqDe():pFoe(0),pSup(0),m_Loaded(false)
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
	m_Loaded=false;
}
void SqDe::LoadDefault()
{
	Unload();
	FoeCount=SupCount=0;
	m_Loaded=true;
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

	m_Loaded=true;
	return true;
}

u32 SqDe::MakeLen()
{
	ASSERT(m_Loaded);
	return sizeof(Header)+2+FoeCount*sizeof(SqItem)+2+SupCount*sizeof(SqItem);
}
void SqDe::Make(u8* pdst)
{
	ASSERT(m_Loaded);
	Header &head=*(Header*)pdst;
	head.Magic=MAGIC;
	head.HeaderSize=0x0C;
	head.FoeDataOffset=0x0C;
	head.SupDataOffset=0x0E+FoeCount*sizeof(SqItem);
	head.Zero=0;
	head.Const1=1;
	pdst+=sizeof(head);

	*(u16*)pdst=FoeCount;pdst+=2;
	memcpy(pdst,pFoe,sizeof(SqItem)*FoeCount);pdst+=sizeof(SqItem)*FoeCount;
	*(u16*)pdst=SupCount;pdst+=2;
	memcpy(pdst,pSup,sizeof(SqItem)*SupCount);

}