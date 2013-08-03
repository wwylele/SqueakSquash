#include "stdafx.h"
#include "SqMa.h"


SqMa::SqMa(void):
	pGrid(0),
	pBlockMappingA(0),
	pBlockMappingB(0),
	pSection10(0),
	pDoor(0),
	pGraScript(0)
{}

SqMa::~SqMa(void){Unload();}

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
		pGrid[i].t=*(psrc+head.SectionOff[9]+i);
	}

	BlockMappingCountA=*(u16*)(psrc+head.SectionOff[1]);
	BlockMappingCountB=*(u16*)(psrc+head.SectionOff[2]);
	pBlockMappingA=new BLOCK_MAPPING[BlockMappingCountA];
	pBlockMappingB=new BLOCK_MAPPING[BlockMappingCountB];
	memcpy(pBlockMappingA,psrc+head.SectionOff[1]+2,sizeof(BLOCK_MAPPING)*BlockMappingCountA);
	memcpy(pBlockMappingB,psrc+head.SectionOff[2]+2,sizeof(BLOCK_MAPPING)*BlockMappingCountB);

	u16 Section10Len=*(u16*)(psrc+head.SectionOff[10]);
	pSection10=new u8[Section10Len];
	memcpy(pSection10,psrc+head.SectionOff[10],Section10Len);

	DoorCount=*(u16*)(psrc+head.SectionOff[11]);
	pDoor=new DOOR[DoorCount];
	memcpy(pDoor,psrc+head.SectionOff[11]+2,DoorCount*sizeof(DOOR));

	//t_pSection12=psrc+head.SectionOff[12];
	memcpy(&S12Header,psrc+head.SectionOff[12],sizeof(S12Header));
	pGraScript=new GRA_SCRIPT[S12Header.GraScriptCount];
	memcpy(pGraScript,psrc+head.SectionOff[12]+sizeof(S12Header),
		sizeof(GRA_SCRIPT)*S12Header.GraScriptCount);

	//S9
	s9exl=head.SectionOff[10]-head.SectionOff[9]-w*h;
	s9exp=psrc+head.SectionOff[9]+w*h;

	return true;
	
}
void SqMa::Unload()
{
	if(pGrid){delete[] pGrid;pGrid=0;}
	if(pBlockMappingA){delete[] pBlockMappingA;pBlockMappingA=0;}
	if(pBlockMappingB){delete[] pBlockMappingB;pBlockMappingB=0;}
	if(pSection10){delete[] pSection10;pSection10=0;}
	if(pDoor){delete[] pDoor;pDoor=0;}
	if(pGraScript){delete[] pGraScript;pGraScript=0;}
}


