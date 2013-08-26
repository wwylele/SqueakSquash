#include "stdafx.h"
#include "SqMa.h"


SqMa::SqMa(void):
	pGrid(0),
	pBlockMappingA(0),
	pBlockMappingB(0),
	pSection10(0),
	pDoor(0),
	pGraScript(0),
	s9exp(0)
{}

SqMa::~SqMa(void){Unload();}

bool SqMa::Load(const u8* psrc)
{

	Header& head=*(Header*)psrc;
	ASSERT(head.Magic==MAGIC && head.HeaderSize==0x44 && head.SectionOff[0]);
	Unload();
	MapAttribute=head.Attribute;

	//Section 0
	w=*(psrc+head.SectionOff[0]);
	h=*(psrc+head.SectionOff[0]+1);

	//Section 1~2
	BlockMappingCountA=*(u16*)(psrc+head.SectionOff[1]);
	BlockMappingCountB=*(u16*)(psrc+head.SectionOff[2]);
	pBlockMappingA=new BLOCK_MAPPING[BlockMappingCountA];
	pBlockMappingB=new BLOCK_MAPPING[BlockMappingCountB];
	memcpy(pBlockMappingA,psrc+head.SectionOff[1]+2,sizeof(BLOCK_MAPPING)*BlockMappingCountA);
	memcpy(pBlockMappingB,psrc+head.SectionOff[2]+2,sizeof(BLOCK_MAPPING)*BlockMappingCountB);

	//Section 3~9
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

	//Section9 ex
	s9exl=head.SectionOff[10]-head.SectionOff[9]-w*h;
	s9exp=new u8[s9exl];
	memcpy(s9exp,psrc+head.SectionOff[9]+w*h,s9exl);
	

	//Section 10
	u16 Section10Len=*(u16*)(psrc+head.SectionOff[10]);
	pSection10=new u8[Section10Len];
	memcpy(pSection10,psrc+head.SectionOff[10],Section10Len);

	//Section 11
	DoorCount=*(u16*)(psrc+head.SectionOff[11]);
	pDoor=new DOOR[DoorCount];
	memcpy(pDoor,psrc+head.SectionOff[11]+2,DoorCount*sizeof(DOOR));

	//Section 12
	S12HEADER S12Header;
	memcpy(&S12Header,psrc+head.SectionOff[12],sizeof(S12Header));
	GraScriptCount=S12Header.GraScriptCount;
	pGraScript=new GRA_SCRIPT[GraScriptCount];
	memcpy(pGraScript,psrc+head.SectionOff[12]+sizeof(S12Header),
		sizeof(GRA_SCRIPT)*GraScriptCount);
	S12HScript=S12Header.HScript;

	return true;
	
}
u32 SqMa::MakeLen()
{
	return sizeof(Header)
		+2//s0
		+2+sizeof(BLOCK_MAPPING)*BlockMappingCountA//s1
		+2+sizeof(BLOCK_MAPPING)*BlockMappingCountB//s2
		+w*h*(2+2+2+4+4+4+1)//s3~s9
		+s9exl//s9ex
		+*(u16*)pSection10//s10
		+2+sizeof(DOOR)*DoorCount//s11
		+sizeof(S12HEADER)+sizeof(GRA_SCRIPT)*GraScriptCount;//s12
}
void SqMa::Make(u8* pdst)
{
	u8 *p=pdst;
	Header head;
	head.Magic=MAGIC;
	head.HeaderSize=0x44;
	head.Attribute=MapAttribute;
	head.x40=0;

	p+=sizeof(Header);

	//Section 0
	head.SectionOff[0]=p-pdst;
	*(p++)=w;
	*(p++)=h;

	//Section 1~2
	head.SectionOff[1]=p-pdst;
	*(u16*)p=BlockMappingCountA;p+=2;
	memcpy(p,pBlockMappingA,sizeof(BLOCK_MAPPING)*BlockMappingCountA);
	p+=sizeof(BLOCK_MAPPING)*BlockMappingCountA;
	head.SectionOff[2]=p-pdst;
	*(u16*)p=BlockMappingCountB;p+=2;
	memcpy(p,pBlockMappingB,sizeof(BLOCK_MAPPING)*BlockMappingCountB);
	p+=sizeof(BLOCK_MAPPING)*BlockMappingCountB;

	//Section 3~8
	head.SectionOff[3]=p-pdst;
	for(u16 i=0;i<w*h;++i){*(u16*)p=pGrid[i].gra[0];p+=2;}
	head.SectionOff[4]=p-pdst;
	for(u16 i=0;i<w*h;++i){*(u16*)p=pGrid[i].gra[1];p+=2;}
	head.SectionOff[5]=p-pdst;
	for(u16 i=0;i<w*h;++i){*(u16*)p=pGrid[i].gra[2];p+=2;}
	head.SectionOff[6]=p-pdst;
	for(u16 i=0;i<w*h;++i){*(u32*)p=pGrid[i].det[0];p+=4;}
	head.SectionOff[7]=p-pdst;
	for(u16 i=0;i<w*h;++i){*(u32*)p=pGrid[i].det[1];p+=4;}
	head.SectionOff[8]=p-pdst;
	for(u16 i=0;i<w*h;++i){*(u32*)p=pGrid[i].det[2];p+=4;}

	//Section 9
	head.SectionOff[9]=p-pdst;
	for(u16 i=0;i<w*h;++i)*p++=pGrid[i].t;
	memcpy(p,s9exp,s9exl);p+=s9exl;

	//Section 10
	head.SectionOff[10]=p-pdst;
	memcpy(p,pSection10,*(u16*)pSection10);p+=*(u16*)pSection10;

	//Section 11
	head.SectionOff[11]=p-pdst;
	*(u16*)p=DoorCount;p+=2;
	memcpy(p,pDoor,sizeof(DOOR)*DoorCount);p+=sizeof(DOOR)*DoorCount;

	//Secttion 12
	head.SectionOff[12]=p-pdst;
	S12HEADER S12Header;
	S12Header.HScript=S12HScript;
	S12Header.GraScriptCount=GraScriptCount;
	memcpy(p,&S12Header,sizeof(S12Header));p+=sizeof(S12Header);
	memcpy(p,pGraScript,sizeof(GRA_SCRIPT)*GraScriptCount);

	memcpy(pdst,&head,sizeof(head));

}
void SqMa::Unload()
{
	if(pGrid){delete[] pGrid;pGrid=0;}
	if(pBlockMappingA){delete[] pBlockMappingA;pBlockMappingA=0;}
	if(pBlockMappingB){delete[] pBlockMappingB;pBlockMappingB=0;}
	if(pSection10){delete[] pSection10;pSection10=0;}
	if(pDoor){delete[] pDoor;pDoor=0;}
	if(pGraScript){delete[] pGraScript;pGraScript=0;}
	if(s9exp){delete[] s9exp;s9exp=0;}
}


