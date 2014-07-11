#include "StdAfx.h"
#include "SqStepPack.h"

SqStepPack::SqStepPack(void):pMxp(0),pDoe(0)
{
}

SqStepPack::~SqStepPack(void)
{
	Unload();
}
void SqStepPack::WriteMxp(u8* src,u32 len)
{
	if(pMxp)delete[]pMxp;
	MxpLen=len;
	if(len)
	{
		pMxp=new u8[len];
		memcpy(pMxp,src,len);
	}
	else
	{
		pMxp=0;
	}
}
void SqStepPack::WriteDoe(u8* src,u32 len)
{
	if(pDoe)delete[]pDoe;
	DoeLen=len;
	if(len)
	{
		pDoe=new u8[len];
		memcpy(pDoe,src,len);
	}
	else
	{
		pDoe=0;
	}
}
u8* SqStepPack::GetMxp(u32 *getlen)
{
	if(getlen)*getlen=MxpLen;
	return pMxp;
}
u8* SqStepPack::GetDoe(u32 *getlen)
{
	if(getlen)*getlen=DoeLen;
	return pDoe;
}
void SqStepPack::Unload()
{

	if(pMxp){delete[]pMxp;pMxp=0;}
	if(pDoe){delete[]pDoe;pDoe=0;}
}

const char* SqStepPackMagic="SQSQSTEP";
bool SqStepPack::Load(CFile &file)
{
	Unload();
	file.Seek(0,CFile::begin);
	char MagicBuff[8];
	file.Read(MagicBuff,8);
	if(memcmp(MagicBuff,SqStepPackMagic,8)!=0)return false;
	u32 version;
	file.Read(&version,4);
	if(version!=1)return false;

	file.Read(&Header,sizeof(Header));

	file.Read(&MxpLen,4);
	pMxp=new u8[MxpLen];
	file.Read(pMxp,MxpLen);
	file.Read(&DoeLen,4);
	pDoe=new u8[DoeLen];
	file.Read(pDoe,DoeLen);

	return true;
}
void SqStepPack::Save(CFile &file)
{
	ASSERT(pMxp && pDoe);
	file.Seek(0,CFile::begin);
	file.Write(SqStepPackMagic,8);
	u32 version;
	version=1;
	file.Write(&version,4);

	file.Write(&Header,sizeof(Header));

	file.Write(&MxpLen,4);
	file.Write(pMxp,MxpLen);
	file.Write(&DoeLen,4);
	file.Write(pDoe,DoeLen);
}