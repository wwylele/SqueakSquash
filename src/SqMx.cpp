#include "StdAfx.h"
#include "SqMx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SqMx::SqMx():pStep(0)
{
}
SqMx::~SqMx()
{
	Unload();
}
void SqMx::Unload()
{
	if(pStep)delete[]pStep;
}
bool SqMx::Load(const u8* psrc)
{
	Header *head=(Header*)psrc;
	if(head->Magic!=MAGIC)return false;
	Unload();
	StepCount=head->StepCount;
	pStep=new StepInfo[StepCount];
	psrc+=12;
	u16 strlen;
	for(u16 i=0;i<StepCount;++i)
	{
		psrc+=2;//"MA"
		strlen=*(u16*)psrc;psrc+=2;
		pStep[i].Ma=psrc;psrc+=strlen;
		psrc+=2;//"FB"
		strlen=*(u16*)psrc;psrc+=2;
		pStep[i].Fb=psrc;psrc+=strlen;
		psrc+=2;//"BB"
		strlen=*(u16*)psrc;psrc+=2;
		pStep[i].Bb=psrc;psrc+=strlen;
		psrc+=2;//"DE"
		strlen=*(u16*)psrc;psrc+=2;
		pStep[i].De=psrc;psrc+=strlen;
		psrc+=2;//"PL"
		strlen=*(u16*)psrc;psrc+=2;
		pStep[i].Pl=psrc;psrc+=strlen;
		psrc+=2;//"3D"
		strlen=*(u16*)psrc;psrc+=2;
		pStep[i].Bg=psrc;psrc+=strlen;
		psrc+=4;//"TM\0\0"
		
	}

	return true;
}
u16 SqMx::GetStepCount(){return StepCount;}
bool SqMx::IsLoaded(){return pStep!=0;}
SqMx::StepInfo& SqMx::Step(u16 i){ASSERT(i<StepCount);return pStep[i];}