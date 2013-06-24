#include "stdafx.h"
#include "SqMa.h"


SqMa::SqMa(void):pGrid(0),pBlockMappingA(0),pBlockMappingB(0){}

SqMa::~SqMa(void){Unload();}
bool SqMa::IsLoaded(){return pGrid!=0;}
bool SqMa::Load(const u8* psrc)
{

	Header& head=*(Header*)psrc;
	ASSERT(head.Magic==MAGIC && head.HeaderSize==0x44 && head.SectionOff[0]);
	Unload();
	w=*(psrc+head.SectionOff[0]);
	h=*(psrc+head.SectionOff[0]+1);
	pGrid=new GridData[w*h];
	for(u16 i=0;i<w*h;++i)
	{
		pGrid[i].gra[0]=*((u16*)(psrc+head.SectionOff[3])+i);
		pGrid[i].gra[1]=*((u16*)(psrc+head.SectionOff[4])+i);
		pGrid[i].gra[2]=*((u16*)(psrc+head.SectionOff[5])+i);
		pGrid[i].det[0]=*((u32*)(psrc+head.SectionOff[6])+i);
		pGrid[i].det[1]=*((u32*)(psrc+head.SectionOff[7])+i);
		pGrid[i].det[2]=*((u32*)(psrc+head.SectionOff[8])+i);
	}
	BlockMappingCountA=*(u16*)(psrc+head.SectionOff[1]);
	BlockMappingCountB=*(u16*)(psrc+head.SectionOff[2]);
	pBlockMappingA=new BLOCK_MAPPING[BlockMappingCountA];
	pBlockMappingB=new BLOCK_MAPPING[BlockMappingCountB];
	memcpy(pBlockMappingA,psrc+head.SectionOff[1]+2,sizeof(BLOCK_MAPPING)*BlockMappingCountA);
	memcpy(pBlockMappingB,psrc+head.SectionOff[2]+2,sizeof(BLOCK_MAPPING)*BlockMappingCountB);
	return true;
	
}
void SqMa::Unload()
{
	if(pGrid)delete[] pGrid;
	if(pBlockMappingA)delete[] pBlockMappingA;
	if(pBlockMappingB)delete[] pBlockMappingB;
}
SqMa::GridData& SqMa::Grid(u8 x,u8 y)
{
	ASSERT(IsLoaded());
	ASSERT(x<w);
	ASSERT(y<h);
	return pGrid[x+y*w];
}
u8 SqMa::GetW(){ASSERT(IsLoaded());return w;}
u8 SqMa::GetH(){ASSERT(IsLoaded());return h;}
SqMa::BLOCK_MAPPING& SqMa::BlockMappingA(u16 i)
{
	ASSERT(IsLoaded());
	ASSERT(i<BlockMappingCountA);
	return pBlockMappingA[i];
}
SqMa::BLOCK_MAPPING& SqMa::BlockMappingB(u16 i)
{
	ASSERT(IsLoaded());
	ASSERT(i<BlockMappingCountB);
	return pBlockMappingB[i];
}
