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
void SqMx::Create(u16 StepCount)
{
	ASSERT(StepCount);
	Unload();
	this->StepCount=StepCount;
	pStep=new StepInfo[StepCount];
}
void SqMx::Make(u8 **ppData,u32 *pLen)
{
	CFile file;
	file.Open(TMP_FILE,CFile::modeCreate|CFile::modeReadWrite,0);
	Header head;
	head.Magic=MAGIC;
	head.HeaderSize=head.DataOffset=12;
	head.x06=0;
	head.EntryLevel_ZB=EntryLevel-1;
	head.EntryStage_ZB=EntryStage-1;
	head.StepCount=StepCount;
	file.Write(&head,sizeof(head));
	const char 
		*mMA="MA",
		*mFB="FB",
		*mBB="BB",
		*mDE="DE",
		*mPL="PL",
		*m3D="3D",
		*mTM="TM\0\0";
	bool DoubleNull;
	u16 StrLen;
	u8 Zero=0;
	for(u16 i=0;i<StepCount;++i)
	{
		StrLen=(u16)pStep[i].Ma.GetLength();
		DoubleNull=(StrLen&1)?false:true;
		StrLen+=DoubleNull?2:1;
		file.Write(mMA,2);
		file.Write(&StrLen,2);
		file.Write((const char*)pStep[i].Ma,pStep[i].Ma.GetLength());
		file.Write(&Zero,1);if(DoubleNull)file.Write(&Zero,1);

		StrLen=(u16)pStep[i].Fb.GetLength();
		DoubleNull=(StrLen&1)?false:true;
		StrLen+=DoubleNull?2:1;
		file.Write(mFB,2);
		file.Write(&StrLen,2);
		file.Write((const char*)pStep[i].Fb,pStep[i].Fb.GetLength());
		file.Write(&Zero,1);if(DoubleNull)file.Write(&Zero,1);

		StrLen=(u16)pStep[i].Bb.GetLength();
		DoubleNull=(StrLen&1)?false:true;
		StrLen+=DoubleNull?2:1;
		file.Write(mBB,2);
		file.Write(&StrLen,2);
		file.Write((const char*)pStep[i].Bb,pStep[i].Bb.GetLength());
		file.Write(&Zero,1);if(DoubleNull)file.Write(&Zero,1);

		StrLen=(u16)pStep[i].De.GetLength();
		DoubleNull=(StrLen&1)?false:true;
		StrLen+=DoubleNull?2:1;
		file.Write(mDE,2);
		file.Write(&StrLen,2);
		file.Write((const char*)pStep[i].De,pStep[i].De.GetLength());
		file.Write(&Zero,1);if(DoubleNull)file.Write(&Zero,1);

		StrLen=(u16)pStep[i].Pl.GetLength();
		DoubleNull=(StrLen&1)?false:true;
		StrLen+=DoubleNull?2:1;
		file.Write(mPL,2);
		file.Write(&StrLen,2);
		if(pStep[i].Pl.GetLength())file.Write((const char*)pStep[i].Pl,pStep[i].Pl.GetLength());
		file.Write(&Zero,1);if(DoubleNull)file.Write(&Zero,1);

		StrLen=(u16)pStep[i].Bg.GetLength();
		DoubleNull=(StrLen&1)?false:true;
		StrLen+=DoubleNull?2:1;
		file.Write(m3D,2);
		file.Write(&StrLen,2);
		file.Write((const char*)pStep[i].Bg,pStep[i].Bg.GetLength());
		file.Write(&Zero,1);if(DoubleNull)file.Write(&Zero,1);

		file.Write(mTM,4);
	}


	*pLen=(u32)file.GetLength();
	*ppData=new u8[*pLen];
	file.Seek(0,CFile::begin);
	file.Read(*ppData,*pLen);
	file.Close();
	CFile::Remove(TMP_FILE);
}
bool SqMx::Load(const u8* psrc)
{
	Header *head=(Header*)psrc;
	if(head->Magic!=MAGIC)return false;
	Unload();
	EntryLevel=head->EntryLevel_ZB+1;
	EntryStage=head->EntryStage_ZB+1;
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