#include "stdafx.h"
#include "SqMa.h"
#include "Evc.h"
#include "Boss.h"

SqMa::SqMa(void):
	pCell(0),
	pTexMappingA(0),
	pTexMappingB(0),
	pGuide(0),
	pGuideMatrix(0),
	pMctrl(0),
	pMctrlGroup(0),
	//pEvpPack(0),
	pDoor(0),
	pGraScript(0),
	pGraScriptCurrent(0)
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
	TexMappingCountA=*(u16*)(psrc+head.SectionOff[1]);
	TexMappingCountB=*(u16*)(psrc+head.SectionOff[2]);
	pTexMappingA=new TEX_MAPPING[TexMappingCountA];
	pTexMappingB=new TEX_MAPPING[TexMappingCountB];
	memcpy(pTexMappingA,psrc+head.SectionOff[1]+2,sizeof(TEX_MAPPING)*TexMappingCountA);
	memcpy(pTexMappingB,psrc+head.SectionOff[2]+2,sizeof(TEX_MAPPING)*TexMappingCountB);

	//Section 3~9
	pCell=new CellData[w*h];
	for(u16 i=0;i<w*h;++i)
	{
		pCell[i].gra[0]=*((u16*)(psrc+head.SectionOff[3])+i);
		pCell[i].gra[1]=*((u16*)(psrc+head.SectionOff[4])+i);
		pCell[i].gra[2]=*((u16*)(psrc+head.SectionOff[5])+i);
		pCell[i].det[0]=*((u32*)(psrc+head.SectionOff[6])+i);
		pCell[i].det[1]=*((u32*)(psrc+head.SectionOff[7])+i);
		pCell[i].det[2]=*((u32*)(psrc+head.SectionOff[8])+i);
		pCell[i].guide_id=*(psrc+head.SectionOff[9]+i);
	}

	u8 pad=w*h%2;


	//Section9 ex new
	const u8* ps9ex=psrc+head.SectionOff[9]+w*h+pad;
	if(ps9ex==psrc+head.SectionOff[10] || *ps9ex==0)
	{
		GuideCount=0;
	}
	else
	{
		GuideCount=*(u16*)(ps9ex);ps9ex+=2;
		pGuide=new GUIDE_DATA[GuideCount];
		memcpy(pGuide,ps9ex,sizeof(GUIDE_DATA)*GuideCount);
		ps9ex+=sizeof(GUIDE_DATA)*GuideCount;
		pGuideMatrix=new u8[GuideCount*GuideCount];
		memcpy(pGuideMatrix,ps9ex,GuideCount*GuideCount);
	}
	

	//Section 10
	//u16 Section10Len=*(u16*)(psrc+head.SectionOff[10]);
	//pSection10=new u8[Section10Len];
	//memcpy(pSection10,psrc+head.SectionOff[10],Section10Len);

	//Section 10'
	S10HEADER S10Header;
	const u8* ps10=psrc+head.SectionOff[10]+2;
	memcpy(&S10Header,ps10,sizeof(S10Header));
	MctrlCount=S10Header.MctrlCount;
	MctrlGroupCount=S10Header.MctrlGroupCount;
	
	if(MctrlCount)
	{
		pMctrl=new MctrlData[MctrlCount];
		memcpy(pMctrl,ps10+S10Header.MctrlListOff,sizeof(MctrlData)*MctrlCount);
		pMctrlGroup=new MctrlGroupData[MctrlGroupCount];
		for(u8 i=0;i<MctrlGroupCount;++i)
		{
			const u8 *pEvcE;
			pEvcE=ps10+*(u16*)(ps10+S10Header.MctrlGroupListOff+i*2);
			memcpy(&pMctrlGroup[i].Header,pEvcE,sizeof(MctrlGroupHeader));
			
			pMctrlGroup[i].pData=new u8[pMctrlGroup[i].Header.count*pMctrlGroup[i].Header.data_len];
			memcpy(pMctrlGroup[i].pData,pEvcE+sizeof(MctrlGroupHeader),
				pMctrlGroup[i].Header.count*pMctrlGroup[i].Header.data_len);

		}

		/*//Section10 New
		pEvpPack=new EvpPack[MctrlCount];
		for(u8 i=0;i<MctrlCount;++i)
		{
			u8 evpid;
			pEvpPack[i].class_id=((MctrlData*)(ps10+S10Header.MctrlListOff))[i].class_id;
			pEvpPack[i].x=((MctrlData*)(ps10+S10Header.MctrlListOff))[i].x;
			pEvpPack[i].y=((MctrlData*)(ps10+S10Header.MctrlListOff))[i].y;
			pEvpPack[i].param=((MctrlData*)(ps10+S10Header.MctrlListOff))[i].param;
			evpid=((MctrlData*)(ps10+S10Header.MctrlListOff))[i].id;
			pEvpPack[i].pExtData=new u8[GetEvcDataLen(pEvpPack[i].class_id)];
			//Search for the extdata
			for(u8 j=0;;++j)
			{
				ASSERT(j<MctrlGroupCount);
				const u8 *pEvcE;
				pEvcE=ps10+*(u16*)(ps10+S10Header.MctrlGroupListOff+j*2);
				if(*pEvcE==pEvpPack[i].class_id)
				{
					ASSERT(*(pEvcE+1)>evpid);
					memcpy(pEvpPack[i].pExtData,
						pEvcE+4+ evpid*GetEvcDataLen(pEvpPack[i].class_id),
						GetEvcDataLen(pEvpPack[i].class_id));
					break;
				}
			}
		}*/
	}

	//Section 11
	DoorCount=*(u16*)(psrc+head.SectionOff[11]);
	if(DoorCount)
	{
		pDoor=new DOOR[DoorCount];
		memcpy(pDoor,psrc+head.SectionOff[11]+2,DoorCount*sizeof(DOOR));
	}

	//Section 12
	S12HEADER S12Header;
	memcpy(&S12Header,psrc+head.SectionOff[12],sizeof(S12Header));
	GraScriptCount=S12Header.GraScriptCount;
	if(GraScriptCount)
	{
		pGraScript=new GRA_SCRIPT[GraScriptCount];
		memcpy(pGraScript,psrc+head.SectionOff[12]+sizeof(S12Header),
			sizeof(GRA_SCRIPT)*GraScriptCount);
		pGraScriptCurrent=new GRA_SCRIPT_CURRENT[GraScriptCount];
	}
	S12HScript=S12Header.HScript;
	TicketClear();

	return true;
	
}
u16 SqMa::S10MakeLen()
{
	u16 S10Len=10+MctrlCount*sizeof(MctrlData)+MctrlGroupCount*2;
	for(u8 i=0;i<MctrlGroupCount;++i)
	{
		S10Len+=sizeof(MctrlGroupHeader)+pMctrlGroup[i].Header.count*pMctrlGroup[i].Header.data_len;
	}
	return S10Len;
}
u32 SqMa::MakeLen()
{
	return sizeof(Header)
		+2//s0
		+2+sizeof(TEX_MAPPING)*TexMappingCountA//s1
		+2+sizeof(TEX_MAPPING)*TexMappingCountB//s2
		+w*h*(2+2+2+4+4+4+1)//s3~s9
		+w*h%2//s9pad
		+(GuideCount?(2
			+GuideCount*sizeof(GUIDE_DATA)
			+GuideCount*GuideCount+GuideCount%2)
			:0)//s9ex
		+S10MakeLen()//s10
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
	*(u16*)p=TexMappingCountA;p+=2;
	memcpy(p,pTexMappingA,sizeof(TEX_MAPPING)*TexMappingCountA);
	p+=sizeof(TEX_MAPPING)*TexMappingCountA;
	head.SectionOff[2]=p-pdst;
	*(u16*)p=TexMappingCountB;p+=2;
	memcpy(p,pTexMappingB,sizeof(TEX_MAPPING)*TexMappingCountB);
	p+=sizeof(TEX_MAPPING)*TexMappingCountB;

	//Section 3~8
	head.SectionOff[3]=p-pdst;
	for(u16 i=0;i<w*h;++i){*(u16*)p=pCell[i].gra[0];p+=2;}
	head.SectionOff[4]=p-pdst;
	for(u16 i=0;i<w*h;++i){*(u16*)p=pCell[i].gra[1];p+=2;}
	head.SectionOff[5]=p-pdst;
	for(u16 i=0;i<w*h;++i){*(u16*)p=pCell[i].gra[2];p+=2;}
	head.SectionOff[6]=p-pdst;
	for(u16 i=0;i<w*h;++i){*(u32*)p=pCell[i].det[0];p+=4;}
	head.SectionOff[7]=p-pdst;
	for(u16 i=0;i<w*h;++i){*(u32*)p=pCell[i].det[1];p+=4;}
	head.SectionOff[8]=p-pdst;
	for(u16 i=0;i<w*h;++i){*(u32*)p=pCell[i].det[2];p+=4;}

	//Section 9
	head.SectionOff[9]=p-pdst;
	for(u16 i=0;i<w*h;++i)*p++=pCell[i].guide_id;
	if(w*h%2)*(p++)=0;

	//section9 old
	//memcpy(p,s9exp,s9exl);p+=s9exl;

	//section 9 new
	if(GuideCount)
	{
		*(u16*)p=GuideCount;p+=2;
		memcpy(p,pGuide,GuideCount*sizeof(GUIDE_DATA));p+=GuideCount*sizeof(GUIDE_DATA);
		memcpy(p,pGuideMatrix,GuideCount*GuideCount);p+=GuideCount*GuideCount;
		if(GuideCount%2)*(p++)=0;
	}

	//Section 10
	head.SectionOff[10]=p-pdst;
	//memcpy(p,pSection10,*(u16*)pSection10);p+=*(u16*)pSection10;
	*(u16*)p=S10MakeLen();p+=2;
	u8* ps10=p;

	//Section10
	S10HEADER S10Header;
	S10Header.MctrlCount=MctrlCount;
	S10Header.MctrlGroupCount=MctrlGroupCount;
	if(MctrlCount)
	{
		S10Header.MctrlListOff=8;
		S10Header.MctrlGroupListOff=8+sizeof(MctrlData)*MctrlCount;
	}else S10Header.MctrlListOff=S10Header.MctrlGroupListOff=0;
	S10Header.x04=0;
	memcpy(p,&S10Header,sizeof(S10HEADER));p+=sizeof(S10HEADER);
	memcpy(p,pMctrl,sizeof(MctrlData)*MctrlCount);p+=sizeof(MctrlData)*MctrlCount;
	u8* ps10e=p;
	p+=MctrlGroupCount*2;
	for(u8 i=0;i<MctrlGroupCount;++i)
	{
		*(u16*)ps10e=(u16)(p-ps10);ps10e+=2;
		memcpy(p,&pMctrlGroup[i].Header,sizeof(MctrlGroupHeader));p+=sizeof(MctrlGroupHeader);
		memcpy(p,pMctrlGroup[i].pData,pMctrlGroup[i].Header.count*pMctrlGroup[i].Header.data_len);
		p+=pMctrlGroup[i].Header.count*pMctrlGroup[i].Header.data_len;
	}

	/*//Section10 New
	S10HEADER S10Header;
	S10Header.MctrlCount=MctrlCount;
	u8 EvcExist[MAX_EVC_COUNT]={0},EvcA[MAX_EVC_COUNT]={0};
	for(u8 i=0;i<MctrlCount;++i)++EvcExist[pEvpPack[i].class_id];
	S10Header.MctrlGroupCount=0;
	for(u8 i=0;i<MAX_EVC_COUNT;++i)if(EvcExist[i])++S10Header.MctrlGroupCount;
	if(MctrlCount)
	{
		S10Header.MctrlListOff=8;
		S10Header.MctrlGroupListOff=8+sizeof(MctrlData)*MctrlCount;
	}else S10Header.MctrlListOff=S10Header.MctrlGroupListOff=0;
	S10Header.x04=0;
	memcpy(p,&S10Header,sizeof(S10HEADER));p+=sizeof(S10HEADER);
	MctrlData tEvp;
	for(u8 i=0;i<MctrlCount;++i)
	{
		tEvp.class_id=pEvpPack[i].class_id;
		tEvp.x=pEvpPack[i].x;
		tEvp.y=pEvpPack[i].y;
		tEvp.param=pEvpPack[i].param;
		tEvp.id=EvcA[tEvp.class_id]++;
		memcpy(p,&tEvp,sizeof(MctrlData));p+=sizeof(MctrlData);
	}
	u8* ps10e=p;
	p+=S10Header.MctrlGroupCount*2;
	u8 cur_evc=0;
	MctrlGroupHeader tEvc;
	for(u8 i=0;i<S10Header.MctrlGroupCount;++i)
	{
		while(EvcExist[cur_evc]==0)++cur_evc;
		*(u16*)ps10e=(u16)(p-ps10);ps10e+=2;

		tEvc.class_id=cur_evc;
		tEvc.data_len=GetEvcDataLen(cur_evc);
		tEvc.count=EvcExist[cur_evc];
		memcpy(p,&pMctrlGroup[i].Header,sizeof(MctrlGroupHeader));p+=sizeof(MctrlGroupHeader);
		
		for(u8 j=0;j<MctrlCount;++j)if(pEvpPack[j].class_id==cur_evc)
		{
			memcpy(p,pEvpPack[j].pExtData,GetEvcDataLen(cur_evc));
			p+=GetEvcDataLen(cur_evc);
		}
	}*/


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
	if(pCell){delete[] pCell;pCell=0;}
	if(pTexMappingA){delete[] pTexMappingA;pTexMappingA=0;}
	if(pTexMappingB){delete[] pTexMappingB;pTexMappingB=0;}
	if(pGuide){delete[] pGuide;pGuide=0;}
	if(pGuideMatrix){delete[] pGuideMatrix;pGuideMatrix=0;}
	//if(pSection10){delete[] pSection10;pSection10=0;}
	if(pMctrl){delete[] pMctrl;pMctrl=0;}
	if(pMctrlGroup){
		for(u8 i=0;i<MctrlGroupCount;++i)delete[] pMctrlGroup[i].pData;
		delete[] pMctrlGroup;
		pMctrlGroup=0;
	}
	/*if(pEvpPack){
		for(u8 i=0;i<MctrlCount;++i)delete[] pEvpPack[i].pExtData;
		delete[] pEvpPack;
		pEvpPack=0;
	}*/
	if(pDoor){delete[] pDoor;pDoor=0;}
	if(pGraScript){delete[] pGraScript;pGraScript=0;}
	if(pGraScriptCurrent){delete[]pGraScriptCurrent;pGraScriptCurrent=0;}
}


void SqMa::TicketClear()
{
	for(u16 i=0;i<GraScriptCount;++i)
	{
		pGraScriptCurrent[i].CurrentFrame=0;
		pGraScriptCurrent[i].CurrentTime=0;
	}
}
bool SqMa::TicketIn()
{
	bool r=false;
	for(u16 i=0;i<GraScriptCount;++i)
	{
		++pGraScriptCurrent[i].CurrentTime;
		if(pGraScriptCurrent[i].CurrentTime==
			pGraScript[i].TimeDelta[pGraScriptCurrent[i].CurrentFrame])
		{
			pGraScriptCurrent[i].CurrentTime=0;
			++pGraScriptCurrent[i].CurrentFrame;
			if(pGraScriptCurrent[i].CurrentFrame==pGraScript[i].FrameCount)
				pGraScriptCurrent[i].CurrentFrame=0;
			r=true;
		}
	}
	return r;
}