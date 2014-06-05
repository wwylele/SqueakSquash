#include "stdafx.h"
#include "SqMa.h"
#include "SqDe.h"
#include "Evc.h"

SqMa::SqMa(void):
	pCell(0),
	pGuide(0),
	pGuideMatrix(0),
	pMctrl(0),
	pMctrlGroup(0),
	pDoor(0),
	pGraScript(0),
	pGraScriptCurrent(0),
	pMaDeComm(0)
{
	pTexMapping[TEXM_F]=pTexMapping[TEXM_B]=0;
	for(int i=0;i<4;++i)
	{
		TexMappingNull.mapping[i].tile=0;
		TexMappingNull.mapping[i].flipx=i&1;
		TexMappingNull.mapping[i].flipy=i>>1;
		TexMappingNull.mapping[i].plt=0xF;//just a mark
	}
	MctrlPack.ppMaDeComm=&pMaDeComm;
}

SqMa::~SqMa(void){Unload();}
void SqMa::LoadDefault()
{
	Unload();
	tMapAttribute DefaultMapAttribute={0,0,0x0101,0,0,0,0};
	MapAttribute=DefaultMapAttribute;

	//Section 0
	w=50;
	h=50;

	//Section 1~2
	TexMappingCount[TEXM_F]=256;
	TexMappingCount[TEXM_B]=256;
	pTexMapping[TEXM_F]=new SqTexMapping[TexMappingCount[TEXM_F]];
	pTexMapping[TEXM_B]=new SqTexMapping[TexMappingCount[TEXM_B]];
	ZeroMemory(pTexMapping[TEXM_F],sizeof(SqTexMapping)*TexMappingCount[TEXM_F]);
	ZeroMemory(pTexMapping[TEXM_B],sizeof(SqTexMapping)*TexMappingCount[TEXM_B]);

	//Section 3~9
	pCell=new SqCell[w*h];
	ZeroMemory(pCell,sizeof(SqCell)*w*h);
	GuideCount=0;
	
	//Section 10
	MctrlCount=0;
	MctrlGroupCount=0;

	//Section 11
	DoorCount=0;

	//Section 12
	GraScriptCount=0;
	S12H_SCRIPT DefaultS12HScript={{0,0,0,0xFF,0xFF,0xFF},{5,5,5,5}};
	S12HScript=DefaultS12HScript;
	TicketClear();
}
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
	TexMappingCount[TEXM_F]=*(u16*)(psrc+head.SectionOff[1]);
	TexMappingCount[TEXM_B]=*(u16*)(psrc+head.SectionOff[2]);
	pTexMapping[TEXM_F]=new SqTexMapping[TexMappingCount[TEXM_F]];
	pTexMapping[TEXM_B]=new SqTexMapping[TexMappingCount[TEXM_B]];
	memcpy(pTexMapping[TEXM_F],psrc+head.SectionOff[1]+2,sizeof(SqTexMapping)*TexMappingCount[TEXM_F]);
	memcpy(pTexMapping[TEXM_B],psrc+head.SectionOff[2]+2,sizeof(SqTexMapping)*TexMappingCount[TEXM_B]);

	//Section 3~9
	pCell=new SqCell[w*h];
	for(u16 i=0;i<w*h;++i)
	{
		pCell[i].gra[0]=*((u16*)(psrc+head.SectionOff[3])+i);
		pCell[i].gra[1]=*((u16*)(psrc+head.SectionOff[4])+i);
		pCell[i].gra[2]=*((u16*)(psrc+head.SectionOff[5])+i);
		pCell[i].det[0]=*((u32*)(psrc+head.SectionOff[6])+i);
		pCell[i].det[1]=*((u32*)(psrc+head.SectionOff[7])+i);
		pCell[i].det[2]=*((u32*)(psrc+head.SectionOff[8])+i);
		pCell[i].guide=*(psrc+head.SectionOff[9]+i);
	}

	u8 pad=w*h%2;


	//Section9 ex new
	const u8* ps9ex=psrc+head.SectionOff[9]+w*h+pad;
	if(ps9ex==psrc+head.SectionOff[10] || *ps9ex==0)
	{
		GuideCount=0;
		pGuide=0;
		pGuideMatrix=0;
	}
	else
	{
		GuideCount=*(u16*)(ps9ex);ps9ex+=2;
		pGuide=new SqGuide[GuideCount];
		memcpy(pGuide,ps9ex,sizeof(SqGuide)*GuideCount);
		ps9ex+=sizeof(SqGuide)*GuideCount;
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
		pDoor=new SqDoor[DoorCount];
		memcpy(pDoor,psrc+head.SectionOff[11]+2,DoorCount*sizeof(SqDoor));
	}
	else pDoor=0;

	//Section 12
	S12HEADER S12Header;
	memcpy(&S12Header,psrc+head.SectionOff[12],sizeof(S12Header));
	GraScriptCount=S12Header.GraScriptCount;
	if(GraScriptCount)
	{
		pGraScript=new SqGraScript[GraScriptCount];
		memcpy(pGraScript,psrc+head.SectionOff[12]+sizeof(S12Header),
			sizeof(SqGraScript)*GraScriptCount);
		pGraScriptCurrent=new GRA_SCRIPT_CURRENT[GraScriptCount];
	}
	else{pGraScript=0;pGraScriptCurrent=0;}
	S12HScript=S12Header.HScript;
	TicketClear();

	return true;
	
}
u16 SqMa::S10MakeLen()
{
	ASSERT(IsLoaded());
	u16 S10Len=10+MctrlCount*sizeof(MctrlData)+MctrlGroupCount*2;
	for(u8 i=0;i<MctrlGroupCount;++i)
	{
		S10Len+=sizeof(MctrlGroupHeader)+pMctrlGroup[i].Header.count*pMctrlGroup[i].Header.data_len;
	}
	return S10Len;
}
u32 SqMa::MakeLen()
{
	ASSERT(IsLoaded());
	return sizeof(Header)
		+2//s0
		+2+sizeof(SqTexMapping)*TexMappingCount[TEXM_F]//s1
		+2+sizeof(SqTexMapping)*TexMappingCount[TEXM_B]//s2
		+w*h*(2+2+2+4+4+4+1)//s3~s9
		+w*h%2//s9pad
		+(GuideCount?(2
			+GuideCount*sizeof(SqGuide)
			+GuideCount*GuideCount+GuideCount%2)
			:0)//s9ex
		+S10MakeLen()//s10
		+2+sizeof(SqDoor)*DoorCount//s11
		+sizeof(S12HEADER)+sizeof(SqGraScript)*GraScriptCount;//s12
}
void SqMa::Make(u8* pdst)
{
	ASSERT(IsLoaded());

	RepairDoorIndex();
	//RepairGuideIndex();

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
	*(u16*)p=TexMappingCount[TEXM_F];p+=2;
	memcpy(p,pTexMapping[TEXM_F],sizeof(SqTexMapping)*TexMappingCount[TEXM_F]);
	p+=sizeof(SqTexMapping)*TexMappingCount[TEXM_F];
	head.SectionOff[2]=p-pdst;
	*(u16*)p=TexMappingCount[TEXM_B];p+=2;
	memcpy(p,pTexMapping[TEXM_B],sizeof(SqTexMapping)*TexMappingCount[TEXM_B]);
	p+=sizeof(SqTexMapping)*TexMappingCount[TEXM_B];

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
	for(u16 i=0;i<w*h;++i)*p++=pCell[i].guide;
	if(w*h%2)*(p++)=0;

	//section9 old
	//memcpy(p,s9exp,s9exl);p+=s9exl;

	//section 9 new
	if(GuideCount)
	{
		*(u16*)p=GuideCount;p+=2;
		memcpy(p,pGuide,GuideCount*sizeof(SqGuide));p+=GuideCount*sizeof(SqGuide);
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
	memcpy(p,pDoor,sizeof(SqDoor)*DoorCount);p+=sizeof(SqDoor)*DoorCount;

	//Secttion 12
	head.SectionOff[12]=p-pdst;
	S12HEADER S12Header;
	S12Header.HScript=S12HScript;
	S12Header.GraScriptCount=GraScriptCount;
	memcpy(p,&S12Header,sizeof(S12Header));p+=sizeof(S12Header);
	memcpy(p,pGraScript,sizeof(SqGraScript)*GraScriptCount);

	memcpy(pdst,&head,sizeof(head));

}
void SqMa::Unload()
{
	if(pCell){delete[] pCell;pCell=0;}
	if(pTexMapping[TEXM_F]){delete[] pTexMapping[TEXM_F];pTexMapping[TEXM_F]=0;}
	if(pTexMapping[TEXM_B]){delete[] pTexMapping[TEXM_B];pTexMapping[TEXM_B]=0;}
	if(pGuide){delete[] pGuide;pGuide=0;}
	if(pGuideMatrix){delete[] pGuideMatrix;pGuideMatrix=0;}
	if(pMctrl){delete[] pMctrl;pMctrl=0;}
	if(pMctrlGroup){
		for(u8 i=0;i<MctrlGroupCount;++i)delete[] pMctrlGroup[i].pData;
		delete[] pMctrlGroup;
		pMctrlGroup=0;
	}
	if(pDoor){delete[] pDoor;pDoor=0;}
	if(pGraScript){delete[] pGraScript;pGraScript=0;}
	if(pGraScriptCurrent){delete[]pGraScriptCurrent;pGraScriptCurrent=0;}
}
void SqMa::CopyTexMapping(SqMa& src,u8 TexMPlane)
{
	ASSERT(IsLoaded());
	ASSERT(src.IsLoaded());
	delete[] pTexMapping[TexMPlane];
	TexMappingCount[TexMPlane]=src.TexMappingCount[TexMPlane];
	pTexMapping[TexMPlane]=new SqTexMapping[TexMappingCount[TexMPlane]];
	memcpy(pTexMapping[TexMPlane],src.pTexMapping[TexMPlane],
		TexMappingCount[TexMPlane]*sizeof(SqTexMapping));

	//Ani
	SqGraScript* newDataTemp=new SqGraScript[GraScriptCount+src.GraScriptCount];
	u16 newCount=0;
	for(u16 i=0;i<GraScriptCount;++i)
	{
		if(pGraScript[i].TexMappingPlane!=TexMPlane)
		{
			newDataTemp[newCount]=pGraScript[i];
			++newCount;
		}
	}
	for(u16 i=0;i<src.GraScriptCount;++i)
	{
		if(src.pGraScript[i].TexMappingPlane==TexMPlane)
		{
			newDataTemp[newCount]=src.pGraScript[i];
			++newCount;
		}
	}
	if(GraScriptCount)
	{
		delete[] pGraScript;
		delete[] pGraScriptCurrent;
	}
	GraScriptCount=newCount;
	pGraScript=new SqGraScript[newCount];
	memcpy(pGraScript,newDataTemp,newCount*sizeof(SqGraScript));
	delete[] newDataTemp;
	pGraScriptCurrent=new GRA_SCRIPT_CURRENT[newCount];
	TicketClear();

}

void SqMa::TicketClear()
{
	ASSERT(IsLoaded());
	for(u16 i=0;i<GraScriptCount;++i)
	{
		pGraScriptCurrent[i].CurrentFrame=0;
		pGraScriptCurrent[i].CurrentTime=0;
	}
}
bool SqMa::TicketIn()
{
	ASSERT(IsLoaded());
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

void SqMa::ZeroCell()
{
	ASSERT(IsLoaded());
	ZeroMemory(pCell,sizeof(SqCell)*w*h);
}
void SqMa::RepairDoorIndex()
{
	for(u16 i=0;i<DoorCount;++i)pDoor[i].index=i;
}
void SqMa::RemoveDoor(u16 i)
{
	ASSERT(IsLoaded());
	ASSERT(i<DoorCount);
	if(DoorCount==1)
	{
		DoorCount=0;
		delete[] pDoor;
		pDoor=0;
	}
	else
	{
		SqDoor *newData=new SqDoor[--DoorCount];
		memcpy(newData,pDoor,i*sizeof(SqDoor));
		memcpy(newData+i,pDoor+i+1,(DoorCount-i)*sizeof(SqDoor));
		delete[] pDoor;
		pDoor=newData;
	}
	if(pMaDeComm)
	{
		pMaDeComm->pMa->Notify_Door(N_DELETE,i);
		pMaDeComm->pDe->Notify_Door(N_DELETE,i);
	}
}
void SqMa::DownDoor(u16 i)
{
	ASSERT(IsLoaded());
	ASSERT(i+1<DoorCount);

	SqDoor t;
	t=pDoor[i];
	pDoor[i]=pDoor[i+1];
	pDoor[i+1]=t;

	if(pMaDeComm)
	{
		pMaDeComm->pMa->Notify_Door(N_DOWN,i);
		pMaDeComm->pDe->Notify_Door(N_DOWN,i);
	}
}
void SqMa::RemoveGraScript(u16 i)
{
	ASSERT(IsLoaded());
	ASSERT(i<GraScriptCount);
	if(GraScriptCount==1)
	{
		GraScriptCount=0;
		delete[] pGraScript;
		pGraScript=0;
		delete[] pGraScriptCurrent;
		pGraScriptCurrent=0;
	}
	else
	{
		SqGraScript *newData=new SqGraScript[--GraScriptCount];
		memcpy(newData,pGraScript,i*sizeof(SqGraScript));
		memcpy(newData+i,pGraScript+i+1,(GraScriptCount-i)*sizeof(SqGraScript));
		delete[] pGraScript;
		pGraScript=newData;
		delete[] pGraScriptCurrent;
		pGraScriptCurrent=new GRA_SCRIPT_CURRENT[GraScriptCount];
	}
}
u16 SqMa::NewDoor()
{
	ASSERT(IsLoaded());
	if(DoorCount>=MAX_DOOR_COUNT)
	{
		return 0xFFFF;
	}
	SqDoor *newData=new SqDoor[++DoorCount];
	if(DoorCount!=1)
	{
		memcpy(newData,pDoor,(DoorCount-1)*sizeof(SqDoor));
		delete[]pDoor;
	}
	pDoor=newData;
	memset(&pDoor[DoorCount-1],0,sizeof(SqDoor));

	return DoorCount-1;
}
u16 SqMa::NewGraScript()
{
	ASSERT(IsLoaded());
	ASSERT(IsLoaded());
	SqGraScript *newData=new SqGraScript[++GraScriptCount];
	if(GraScriptCount!=1)
	{
		memcpy(newData,pGraScript,(GraScriptCount-1)*sizeof(SqGraScript));
		delete[]pGraScript;
	}
	pGraScript=newData;
	memset(&pGraScript[GraScriptCount-1],0,sizeof(SqGraScript));

	delete[] pGraScriptCurrent;
	pGraScriptCurrent=new GRA_SCRIPT_CURRENT[GraScriptCount];

	return GraScriptCount-1;
}
void SqMa::ResizeMap(u8 W,u8 H,int ox,int oy)
{
	ASSERT(IsLoaded());
	ASSERT(W && H);
	SqCell *newData=new SqCell[W*H];
	
	for(int y=0;y<(int)H;++y)for(int x=0;x<(int)W;++x)
	{
		if(x>=ox && x<ox+w && y>=oy && y<oy+h)
		{
			newData[x+y*W]=pCell[x-ox+(y-oy)*w];
		}
		else
		{
			memset(&newData[x+y*W],0,sizeof(SqCell));
		}
	}

	delete[] pCell;
	pCell=newData;
	w=W;
	h=H;
}
void SqMa::ParseMctrl()
{
	ASSERT(IsLoaded());
	MctrlPack.FromMa(*this);
}
void SqMa::SerializeMctrl()
{
	ASSERT(IsLoaded());
	MctrlPack.ToMa(*this);
	MctrlPack.Unload();
}
void SqMa::ResizeTexMapping(u8 TexMPlane,u16 size)
{
	ASSERT(IsLoaded());
	ASSERT(size);
	SqTexMapping *newData=new SqTexMapping[size];
	memset(newData,0,size*sizeof(SqTexMapping));
	memcpy(newData,pTexMapping[TexMPlane],
		min(size,TexMappingCount[TexMPlane])*sizeof(SqTexMapping));
	delete[] pTexMapping[TexMPlane];
	pTexMapping[TexMPlane]=newData;
	TexMappingCount[TexMPlane]=size;
}
void SqMa::RepairGuideIndex()
{
	ASSERT(IsLoaded());
	for(u16 i=0;i<GuideCount;++i)
	{
		pGuide[i].index=i;
	}
}
void SqMa::ResizeGuide(u16 count)
{
	ASSERT(IsLoaded());
	if(count==GuideCount)return;
	if(count==0)
	{
		GuideCount=0;
		delete[] pGuide;
		pGuide=0;
		delete[] pGuideMatrix;
		pGuideMatrix=0;
	}
	else
	{
		SqGuide *newGuide=new SqGuide[count];
		u8 *newGuideMatrix=new u8[count*count];
		memset(newGuide,0,sizeof(SqGuide)*count);
		memset(newGuideMatrix,0,count*count);
		for(u16 i=0;i<count;++i)
		{
			if(i<GuideCount)
			{
				newGuide[i]=pGuide[i];
				for(u16 j=0;j<min(count,GuideCount);++j)
				{
					newGuideMatrix[i*count+j]=pGuideMatrix[i*GuideCount+j];
				}
			}
			else
			{
				newGuide[i].index=i;
			}
		}



		if(GuideCount)
		{
			delete[] pGuide;
			delete[] pGuideMatrix;
		}
		GuideCount=count;
		pGuide=newGuide;
		pGuideMatrix=newGuideMatrix;
	}
}
void SqMa::Notify_Door(MaDeCommNOTIFY ncode,u16 index)
{
}
void SqMa::Notify_Mctrl(MaDeCommNOTIFY ncode,u8 index)
{
	for(u8 i=0;i<MctrlPack.GetMctrlCount();++i)
	{
		for(int j=0;j<EvcDesc[MctrlPack[i].class_id].RelMctrlIndex_Count;++j)
		{
			u8 *pRelIndex;
			pRelIndex=MctrlPack[i].pExtData+
				EvcDesc[MctrlPack[i].class_id].RelMctrlIndex_Offset+j;

			if(*pRelIndex!=0xFF)switch(ncode)
			{
			case N_DELETE:
				if(*pRelIndex==index)*pRelIndex=0xFF;
				else if(*pRelIndex>index)--*pRelIndex;
				break;
			case N_DOWN:
				if(*pRelIndex==index)++*pRelIndex;
				else if(*pRelIndex==index+1)--*pRelIndex;
				break;
			}
		}	
	}
	
}

void SqMa::Notify_Foe(MaDeCommNOTIFY ncode,u16 index)
{
	for(u8 i=0;i<MctrlPack.GetMctrlCount();++i)
	{
		//14 Cao / 15 Mutong / 17 Apear
		if(MctrlPack[i].class_id==14 && !(MctrlPack[i].pExtData[0]&MCTRL_EXT_14_ISSUP)||
			MctrlPack[i].class_id==15 && !(MctrlPack[i].pExtData[0]&MCTRL_EXT_15_ISSUP)||
			MctrlPack[i].class_id==17 && !(MctrlPack[i].pExtData[0]&MCTRL_EXT_17_ISSUP)
			)
		{
			if(MctrlPack[i].pExtData[1]!=0xFF)switch(ncode)
			{
			case N_DELETE:
				if(MctrlPack[i].pExtData[1]==index)MctrlPack[i].pExtData[1]=0xFF;
				else if(MctrlPack[i].pExtData[1]>index)--MctrlPack[i].pExtData[1];
				break;
			case N_DOWN:
				if(MctrlPack[i].pExtData[1]==index)++MctrlPack[i].pExtData[1];
				else if(MctrlPack[i].pExtData[1]==index+1)--MctrlPack[i].pExtData[1];
				break;
			}
		}
	}
}
void SqMa::Notify_Sup(MaDeCommNOTIFY ncode,u16 index)
{
	for(u8 i=0;i<MctrlPack.GetMctrlCount();++i)
	{
		//14 Cao / 15 Mutong / 17 Apear
		if((MctrlPack[i].class_id==14 && (MctrlPack[i].pExtData[0]&MCTRL_EXT_14_ISSUP))||
			(MctrlPack[i].class_id==15 && (MctrlPack[i].pExtData[0]&MCTRL_EXT_15_ISSUP))||
			(MctrlPack[i].class_id==17 && (MctrlPack[i].pExtData[0]&MCTRL_EXT_17_ISSUP))
			)
		{
			if(MctrlPack[i].pExtData[1]!=0xFF)switch(ncode)
			{
			case N_DELETE:
				if(MctrlPack[i].pExtData[1]==index)MctrlPack[i].pExtData[1]=0xFF;
				else if(MctrlPack[i].pExtData[1]>index)--MctrlPack[i].pExtData[1];
				break;
			case N_DOWN:
				if(MctrlPack[i].pExtData[1]==index)++MctrlPack[i].pExtData[1];
				else if(MctrlPack[i].pExtData[1]==index+1)--MctrlPack[i].pExtData[1];
				break;
			}
		}
	}
}