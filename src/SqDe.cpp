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
	pFoe=pSup=0;
	m_Loaded=true;
}
bool SqDe::Load(const u8* psrc)
{
	Header &head=*(Header*)psrc;
	ASSERT(head.Magic==MAGIC && head.HeaderSize==0x0C);
	Unload();
	psrc+=sizeof(Header);
	FoeCount=*(u16*)psrc;psrc+=2;
	if(FoeCount)
	{
		pFoe=new SqItem[FoeCount];
		memcpy(pFoe,psrc,sizeof(SqItem)*FoeCount);psrc+=sizeof(SqItem)*FoeCount;
	}
	else pFoe=0;
	SupCount=*(u16*)psrc;psrc+=2;
	if(SupCount)
	{
		pSup=new SqItem[SupCount];
		memcpy(pSup,psrc,sizeof(SqItem)*SupCount);
	}
	else pSup=0;


	m_Loaded=true;
	return true;
}

u32 SqDe::MakeLen()
{
	ASSERT(m_Loaded);
	return sizeof(Header)+2+FoeCount*sizeof(SqItem)+2+SupCount*sizeof(SqItem);
}
void SqDe::RepairIndex()
{
	for(u16 i=0;i<FoeCount;++i)pFoe[i].index=i;
	for(u16 i=0;i<SupCount;++i)pSup[i].index=i;
}
void SqDe::Make(u8* pdst)
{
	ASSERT(m_Loaded);

	RepairIndex();

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

void SqDe::RemoveFoe(u16 i)
{
	ASSERT(m_Loaded);
	ASSERT(i<FoeCount);
	if(FoeCount==1)
	{
		FoeCount=0;
		delete[] pFoe;
		pFoe=0;
	}
	else
	{
		SqItem *newData=new SqItem[--FoeCount];
		memcpy(newData,pFoe,i*sizeof(SqItem));
		memcpy(newData+i,pFoe+i+1,(FoeCount-i)*sizeof(SqItem));
		delete[] pFoe;
		pFoe=newData;
	}
}
void SqDe::RemoveSup(u16 i)
{
	ASSERT(m_Loaded);
	ASSERT(i<SupCount);
	if(SupCount==1)
	{
		SupCount=0;
		delete[] pSup;
		pSup=0;
	}
	else
	{
		SqItem *newData=new SqItem[--SupCount];
		memcpy(newData,pSup,i*sizeof(SqItem));
		memcpy(newData+i,pSup+i+1,(SupCount-i)*sizeof(SqItem));
		delete[] pSup;
		pSup=newData;
	}
}
u16 SqDe::NewFoe()
{
	ASSERT(m_Loaded);
	SqItem *newData=new SqItem[++FoeCount];
	if(FoeCount!=1)
	{
		memcpy(newData,pFoe,(FoeCount-1)*sizeof(SqItem));
		delete[]pFoe;
	}
	pFoe=newData;
	memset(&pFoe[FoeCount-1],0,sizeof(SqItem));
	return FoeCount-1;

}
u16 SqDe::NewSup()
{
	ASSERT(m_Loaded);
	SqItem *newData=new SqItem[++SupCount];
	if(SupCount!=1)
	{
		memcpy(newData,pSup,(SupCount-1)*sizeof(SqItem));
		delete[]pSup;
	}
	pSup=newData;
	memset(&pSup[SupCount-1],0,sizeof(SqItem));
	return SupCount-1;

}