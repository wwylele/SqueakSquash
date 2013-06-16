#include "stdafx.h"
#include "SqMapSet.h"
#include "NitroRom.h"
#include "NitroCompress.h"
#include "SqFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



SqMapSet::SqMapSet(void):m_Loaded(false)
{
}

SqMapSet::~SqMapSet(void)
{
	Unload();
}
bool SqMapSet::Load(CFile &file)
{
	
	file.Seek(0,CFile::begin);
	SqmsFileHeader header;
	file.Read(&header,sizeof(header));

	//Verify the File Magic
	if(memcmp(header.Magic,"SQSQMAPS",8) || header.Version!=1)return false;


	SqmsSectionHeader sheader;
	SqmsSecitemHeader siheader;
	u32 fpos,fpos2;

	//Verify the Sections Magic
	file.Seek(header.SectionBgOffset,CFile::begin);
	file.Read(&sheader,sizeof(sheader));
	if(memcmp(sheader.Magic,"SQBG",4))return false;
	file.Seek(header.SectionGlOffset,CFile::begin);
	file.Read(&sheader,sizeof(sheader));
	if(memcmp(sheader.Magic,"SQGL",4))return false;
	file.Seek(header.SectionPlOffset,CFile::begin);
	file.Read(&sheader,sizeof(sheader));
	if(memcmp(sheader.Magic,"SQPL",4))return false;

	Unload();

	//Read Section Bg
	file.Seek(header.SectionBgOffset,CFile::begin);
	file.Read(&sheader,sizeof(sheader));
	m_BgCount=sheader.Count;
	m_BgList=new SecitemData[m_BgCount];
	fpos=(u32)file.GetPosition();
	for(u32 i=0;i<m_BgCount;++i)
	{
		file.Seek(fpos,CFile::begin);
		file.Read(&siheader,sizeof(siheader));
		memcpy(m_BgList[i].Name,siheader.Name,16);
		m_BgList[i].DataLen=siheader.DataLen;
		file.Seek(siheader.DataOffset,CFile::begin);
		m_BgList[i].pData=new u8[m_BgList[i].DataLen];
		file.Read(m_BgList[i].pData,m_BgList[i].DataLen);

		fpos+=sizeof(SqmsSecitemHeader);
	}

	//Read Section Gl
	file.Seek(header.SectionGlOffset,CFile::begin);
	file.Read(&sheader,sizeof(sheader));
	m_GlCount=sheader.Count;
	m_GlList=new SecitemData[m_GlCount];
	fpos=(u32)file.GetPosition();
	for(u32 i=0;i<m_GlCount;++i)
	{
		file.Seek(fpos,CFile::begin);
		file.Read(&siheader,sizeof(siheader));
		memcpy(m_GlList[i].Name,siheader.Name,16);
		m_GlList[i].DataLen=siheader.DataLen;
		file.Seek(siheader.DataOffset,CFile::begin);
		m_GlList[i].pData=new u8[m_GlList[i].DataLen];
		file.Read(m_GlList[i].pData,m_GlList[i].DataLen);

		fpos+=sizeof(SqmsSecitemHeader);
	}

	//Read Section Pl
	file.Seek(header.SectionPlOffset,CFile::begin);
	file.Read(&sheader,sizeof(sheader));
	m_PlCount=sheader.Count;
	m_PlList=new SecitemData[m_PlCount];
	fpos=(u32)file.GetPosition();
	for(u32 i=0;i<m_PlCount;++i)
	{
		file.Seek(fpos,CFile::begin);
		file.Read(&siheader,sizeof(siheader));
		memcpy(m_PlList[i].Name,siheader.Name,16);
		m_PlList[i].DataLen=siheader.DataLen;
		file.Seek(siheader.DataOffset,CFile::begin);
		m_PlList[i].pData=new u8[m_PlList[i].DataLen];
		file.Read(m_PlList[i].pData,m_PlList[i].DataLen);

		fpos+=sizeof(SqmsSecitemHeader);
	}

	//Read Stage-Step-Map tree
	m_StageCount=header.StageCount;
	m_StageList=new StageData[m_StageCount];
	SqmsStageHeader stheader;
	SqmsStepHeader spheader;
	fpos=sizeof(header);
	for(u32 i=0;i<m_StageCount;++i)
	{
		file.Seek(fpos,CFile::begin);
		file.Read(&stheader,sizeof(stheader));
		m_StageList[i].LevelIdx=stheader.LevelIdx;
		m_StageList[i].StageIdx=stheader.StageIdx;
		m_StageList[i].StepCount=stheader.StepCount;
		m_StageList[i].StepList=new StageData::StepData[m_StageList[i].StepCount];
		fpos2=stheader.StepTableOffset;
		for(u32 j=0;j<m_StageList[i].StepCount;++j)
		{
			file.Seek(fpos2,CFile::begin);
			file.Read(&spheader,sizeof(spheader));
			m_StageList[i].StepList[j].MxpLen=spheader.MxpLen;
			m_StageList[i].StepList[j].DoeLen=spheader.DoeLen;
			m_StageList[i].StepList[j].BgId=spheader.BgId;
			m_StageList[i].StepList[j].BGlId=spheader.BGlId;
			m_StageList[i].StepList[j].FGlId=spheader.FGlId;
			m_StageList[i].StepList[j].PlId=spheader.PlId;
			m_StageList[i].StepList[j].pMxp=new u8[m_StageList[i].StepList[j].MxpLen];
			file.Seek(spheader.MxpOffset,CFile::begin);
			file.Read(m_StageList[i].StepList[j].pMxp,m_StageList[i].StepList[j].MxpLen);
			m_StageList[i].StepList[j].pDoe=new u8[m_StageList[i].StepList[j].DoeLen];
			file.Seek(spheader.DoeOffset,CFile::begin);
			file.Read(m_StageList[i].StepList[j].pDoe,m_StageList[i].StepList[j].DoeLen);
			fpos2+=sizeof(SqmsStepHeader);
		}

		fpos+=sizeof(SqmsStageHeader);
	}
	
	//Read RomInfo
	file.Seek(header.RomInfoOffset,CFile::begin);
	file.Read(&m_RomInfo,sizeof(m_RomInfo));

	m_Loaded=true;
	return true;

}
void SqMapSet::Unload()
{
	if(!m_Loaded)return;
	
	for(u32 i=0;i<m_BgCount;++i)delete[]m_BgList[i].pData;
	delete[]m_BgList;
	m_BgCount=0;
	m_BgList=0;
	
	for(u32 i=0;i<m_GlCount;++i)delete[]m_GlList[i].pData;
	delete[]m_GlList;
	m_GlCount=0;
	m_GlList=0;

	for(u32 i=0;i<m_PlCount;++i)delete[]m_PlList[i].pData;
	delete[]m_PlList;
	m_PlCount=0;
	m_PlList=0;

	for(u32 i=0;i<m_StageCount;++i)
	{
		for(u32 j=0;j<m_StageList[i].StepCount;++j)
		{
			delete[]m_StageList[i].StepList[j].pMxp;
			delete[]m_StageList[i].StepList[j].pDoe;
		}
		delete[]m_StageList[i].StepList;
	}
	delete[]m_StageList;
	m_StageList=0;
	m_StageCount=0;
	m_Loaded=false;
}
bool SqMapSet::Save(CFile &file)
{
	if(!m_Loaded)return false;

	SqmsFileHeader header;
	memcpy(header.Magic,"SQSQMAPS",8);
	header.Version=1;
	header.StageCount=m_StageCount;

	//Calculate the begin position of Stage-Step-Map Tree Data
	u32 stagetablepos=sizeof(header);
	u32 steptablepos=stagetablepos+sizeof(SqmsStageHeader)*m_StageCount;
	u32 stepdatapos=steptablepos;
	for(u32 i=0;i<m_StageCount;i++)
	{
		stepdatapos+=sizeof(SqmsStepHeader)*m_StageList[i].StepCount;
	}

	//Write Stage-Step-Map Tree Data
	SqmsStageHeader stageheader;
	SqmsStepHeader stepheader;
	for(u32 i=0;i<m_StageCount;++i)
	{
		stageheader.LevelIdx=m_StageList[i].LevelIdx;
		stageheader.StageIdx=m_StageList[i].StageIdx;
		stageheader.StepCount=m_StageList[i].StepCount;
		stageheader.StepTableOffset=steptablepos;
		
		for(u32 j=0;j<m_StageList[i].StepCount;++j)
		{
			stepheader.MxpLen=m_StageList[i].StepList[j].MxpLen;
			stepheader.MxpOffset=stepdatapos;
			file.Seek(stepdatapos,CFile::begin);
			file.Write(m_StageList[i].StepList[j].pMxp,m_StageList[i].StepList[j].MxpLen);
			stepdatapos+=m_StageList[i].StepList[j].MxpLen;

			stepheader.DoeLen=m_StageList[i].StepList[j].DoeLen;
			stepheader.DoeOffset=stepdatapos;
			file.Seek(stepdatapos,CFile::begin);
			file.Write(m_StageList[i].StepList[j].pDoe,m_StageList[i].StepList[j].DoeLen);
			stepdatapos+=m_StageList[i].StepList[j].DoeLen;

			stepheader.BgId=m_StageList[i].StepList[j].BgId;
			stepheader.BGlId=m_StageList[i].StepList[j].BGlId;
			stepheader.FGlId=m_StageList[i].StepList[j].FGlId;
			stepheader.PlId=m_StageList[i].StepList[j].PlId;

			file.Seek(steptablepos,CFile::begin);
			file.Write(&stepheader,sizeof(SqmsStepHeader));
			steptablepos+=sizeof(SqmsStepHeader);

		}

		file.Seek(stagetablepos,CFile::begin);
		file.Write(&stageheader,sizeof(SqmsStageHeader));
		stagetablepos+=sizeof(SqmsStageHeader);
	}

	SqmsSectionHeader sheader;
	SqmsSecitemHeader siheader;
	u32 stablepos,sdatapos;

	//Write Section Bg
	header.SectionBgOffset=stepdatapos;
	file.Seek(header.SectionBgOffset,CFile::begin);
	memcpy(sheader.Magic,"SQBG",4);
	sheader.Count=m_BgCount;
	file.Write(&sheader,sizeof(SqmsSectionHeader));
	stablepos=header.SectionBgOffset+sizeof(SqmsSectionHeader);
	sdatapos=stablepos+m_BgCount*sizeof(SqmsSecitemHeader);
	for(u32 i=0;i<m_BgCount;++i)
	{
		memcpy(siheader.Name,m_BgList[i].Name,16);
		siheader.DataLen=m_BgList[i].DataLen;
		siheader.DataOffset=sdatapos;
		file.Seek(stablepos,CFile::begin);
		file.Write(&siheader,sizeof(SqmsSecitemHeader));
		stablepos+=sizeof(SqmsSecitemHeader);
		file.Seek(sdatapos,CFile::begin);
		file.Write(m_BgList[i].pData,m_BgList[i].DataLen);
		sdatapos+=m_BgList[i].DataLen;
	}

	//Write Section Gl
	header.SectionGlOffset=sdatapos;
	file.Seek(header.SectionGlOffset,CFile::begin);
	memcpy(sheader.Magic,"SQGL",4);
	sheader.Count=m_GlCount;
	file.Write(&sheader,sizeof(SqmsSectionHeader));
	stablepos=header.SectionGlOffset+sizeof(SqmsSectionHeader);
	sdatapos=stablepos+m_GlCount*sizeof(SqmsSecitemHeader);
	for(u32 i=0;i<m_GlCount;++i)
	{
		memcpy(siheader.Name,m_GlList[i].Name,16);
		siheader.DataLen=m_GlList[i].DataLen;
		siheader.DataOffset=sdatapos;
		file.Seek(stablepos,CFile::begin);
		file.Write(&siheader,sizeof(SqmsSecitemHeader));
		stablepos+=sizeof(SqmsSecitemHeader);
		file.Seek(sdatapos,CFile::begin);
		file.Write(m_GlList[i].pData,m_GlList[i].DataLen);
		sdatapos+=m_GlList[i].DataLen;
	}

	//Write Section Pl
	header.SectionPlOffset=sdatapos;
	file.Seek(header.SectionPlOffset,CFile::begin);
	memcpy(sheader.Magic,"SQPL",4);
	sheader.Count=m_PlCount;
	file.Write(&sheader,sizeof(SqmsSectionHeader));
	stablepos=header.SectionPlOffset+sizeof(SqmsSectionHeader);
	sdatapos=stablepos+m_PlCount*sizeof(SqmsSecitemHeader);
	for(u32 i=0;i<m_PlCount;++i)
	{
		memcpy(siheader.Name,m_PlList[i].Name,16);
		siheader.DataLen=m_PlList[i].DataLen;
		siheader.DataOffset=sdatapos;
		file.Seek(stablepos,CFile::begin);
		file.Write(&siheader,sizeof(SqmsSecitemHeader));
		stablepos+=sizeof(SqmsSecitemHeader);
		file.Seek(sdatapos,CFile::begin);
		file.Write(m_PlList[i].pData,m_PlList[i].DataLen);
		sdatapos+=m_PlList[i].DataLen;
	}

	//Write RomInfo
	header.RomInfoOffset=sdatapos;
	file.Seek(header.RomInfoOffset,CFile::begin);
	file.Write(&m_RomInfo,sizeof(m_RomInfo));

	//Write File Header
	file.Seek(0,CFile::begin);
	file.Write(&header,sizeof(header));
	
	return true;
}
bool SqMapSet::LoadFromRom(CFile &file)
{
	//Read the ROM header and verify the ROM
	Nitro::ROM_HEADER rom_header;
	file.Seek(0,CFile::begin);
	file.Read(&rom_header,sizeof(rom_header));
	if(strcmp("KIRBY DRO",(char*)rom_header.name))return false;

	Unload();


	//Read RomInfo
	Nitro::ROM_TITLE rom_title;
	ZeroMemory(&m_RomInfo,sizeof(m_RomInfo));
	memcpy(&m_RomInfo,&rom_header,22);
	file.Seek(rom_header.title_offset,CFile::begin);
	file.Read(&rom_title,sizeof(rom_title));
	memcpy(&m_RomInfo.Title_icon_pixel,&rom_title.icon_pixel,512+32+1536);

	//Search and read .mxi file in the ROM
	CStringA strMxiName;
	u32 mxioffset,mxilen;
	u8 *mxitmp;
	SqMx mxi;
	CList<StageData> sdlist;
	CList<CStringA> bglist,gllist,pllist;
	BgDef(bglist);GlDef(gllist);PlDef(pllist);
	POSITION lpos;
	StageData sdtmp;
	u32 offt,lent;
	u8* buff;
	char cbuff[21];
	for(u8 a=1;a<10;++a)for(u8 s=1;s<12;++s)
	{
		strMxiName.Format("map/a%ds%d.mxi",a,s);
		if(mxioffset=Nitro::GetSubFileOffset(file,Nitro::GetSubFileId(file,strMxiName),&mxilen))
		{
			mxitmp=new u8[mxilen];
			file.Seek(mxioffset,CFile::begin);
			file.Read(mxitmp,mxilen);
			mxi.Load(mxitmp);
			sdtmp.LevelIdx=a;
			sdtmp.StageIdx=s;
			sdtmp.StepCount=mxi.GetStepCount();
			sdtmp.StepList=new StageData::StepData[sdtmp.StepCount];
			//Read step data.
			if(sdtmp.StepCount>100)throw;
			for(u16 step=0;step<sdtmp.StepCount;++step)
			{
				//Read mxp;
				offt=Nitro::GetSubFileOffset(file,Nitro::GetSubFileId(file,mxi.Step(step).Ma),&lent);
				if(offt){
					buff=new u8[lent];
					file.Seek(offt,CFile::begin);
					file.Read(buff,lent);
					if(*buff==0x30)//Need to uncompress
					{
						sdtmp.StepList[step].MxpLen=*(u32*)buff>>8;
						sdtmp.StepList[step].pMxp=new u8[sdtmp.StepList[step].MxpLen+10];
						Nitro::UncompressRL(buff,sdtmp.StepList[step].pMxp);
						delete[]buff;
					}
					else{
						sdtmp.StepList[step].MxpLen=lent;
						sdtmp.StepList[step].pMxp=buff;
					}
				}
				else
				{
					sdtmp.StepList[step].MxpLen=0;
					sdtmp.StepList[step].pMxp=new u8[1];
				}

				//Read doe;
				offt=Nitro::GetSubFileOffset(file,Nitro::GetSubFileId(file,mxi.Step(step).De),&lent);
				if(offt){
					buff=new u8[lent];
					file.Seek(offt,CFile::begin);
					file.Read(buff,lent);
					sdtmp.StepList[step].DoeLen=lent;
					sdtmp.StepList[step].pDoe=buff;
				}
				else{
					sdtmp.StepList[step].DoeLen=0;
					sdtmp.StepList[step].pDoe=new u8[1];
				}

				strcpy_s(cbuff,20,mxi.Step(step).Bg);
				_strlwr(cbuff);
				lpos=bglist.Find(cbuff);//Find Bg in name list
				if(!lpos){//If not finded,Add new
					bglist.AddTail(mxi.Step(step).Bg);lpos=bglist.GetTailPosition();
				}
				sdtmp.StepList[step].BgId=0;
				//Get the Bg ID
				while(lpos){
					bglist.GetPrev(lpos);++sdtmp.StepList[step].BgId;
				}--sdtmp.StepList[step].BgId;

				strcpy_s(cbuff,20,mxi.Step(step).Fb);
				_strlwr(cbuff);
				lpos=gllist.Find(cbuff);//Find FGl in name list
				if(!lpos){//If not finded,Add new
					gllist.AddTail(mxi.Step(step).Fb);lpos=gllist.GetTailPosition();
				}
				sdtmp.StepList[step].FGlId=0;
				//Get the FGl ID
				while(lpos){
					gllist.GetPrev(lpos);++sdtmp.StepList[step].FGlId;
				}--sdtmp.StepList[step].FGlId;

				strcpy_s(cbuff,20,mxi.Step(step).Bb);
				_strlwr(cbuff);
				lpos=gllist.Find(cbuff);//Find BGl in name list
				if(!lpos){//If not finded,Add new
					gllist.AddTail(mxi.Step(step).Bb);lpos=gllist.GetTailPosition();
				}
				sdtmp.StepList[step].BGlId=0;
				//Get the BGl ID
				while(lpos){
					gllist.GetPrev(lpos);++sdtmp.StepList[step].BGlId;
				}--sdtmp.StepList[step].BGlId;

				if(mxi.Step(step).Pl!="")
				{
					strcpy_s(cbuff,20,mxi.Step(step).Pl);
					_strlwr(cbuff);
					lpos=pllist.Find(cbuff);//Find Pl in name list
					if(!lpos){//If not finded,Add new
						pllist.AddTail(mxi.Step(step).Pl);lpos=pllist.GetTailPosition();
					}
					sdtmp.StepList[step].PlId=0;
					//Get the Pl ID
					while(lpos){
						pllist.GetPrev(lpos);++sdtmp.StepList[step].PlId;
					}--sdtmp.StepList[step].PlId;
				}
				else sdtmp.StepList[step].PlId=0xFF;
			}

			sdlist.AddTail(sdtmp);

			delete[]mxitmp;
		}
	}

	//Copy stage list from sdlist to m_StageList
	m_StageCount=sdlist.GetCount();
	m_StageList=new StageData[m_StageCount];
	lpos=sdlist.GetHeadPosition();
	for(u32 i=0;i<m_StageCount;++i)
	{
		m_StageList[i]=sdlist.GetNext(lpos);
	}

	//Read bg,gl,pl
	CStringA sfname;
	
	m_BgCount=bglist.GetCount();
	m_BgList=new SecitemData[m_BgCount];
	lpos=bglist.GetHeadPosition();
	for(u32 i=0;i<m_BgCount;++i)
	{
		sfname=bglist.GetNext(lpos);
		ZeroMemory(m_BgList[i].Name,16);
		strcpy_s((char*)m_BgList[i].Name,16,(const char*)sfname+4);
		offt=Nitro::GetSubFileOffset(file,Nitro::GetSubFileId(file,sfname),&m_BgList[i].DataLen);
		if(!offt)throw;
		m_BgList[i].pData=new u8[m_BgList[i].DataLen];
		file.Seek(offt,CFile::begin);
		file.Read(m_BgList[i].pData,m_BgList[i].DataLen);
	}

	m_GlCount=gllist.GetCount();
	m_GlList=new SecitemData[m_GlCount];
	lpos=gllist.GetHeadPosition();
	for(u32 i=0;i<m_GlCount;++i)
	{
		sfname=gllist.GetNext(lpos);
		ZeroMemory(m_GlList[i].Name,16);
		strcpy_s((char*)m_GlList[i].Name,16,(const char*)sfname+4);
		offt=Nitro::GetSubFileOffset(file,Nitro::GetSubFileId(file,sfname),&m_GlList[i].DataLen);
		if(!offt)throw;
		m_GlList[i].pData=new u8[m_GlList[i].DataLen];
		file.Seek(offt,CFile::begin);
		file.Read(m_GlList[i].pData,m_GlList[i].DataLen);
	}

	m_PlCount=pllist.GetCount();
	m_PlList=new SecitemData[m_PlCount];
	lpos=pllist.GetHeadPosition();
	for(u32 i=0;i<m_PlCount;++i)
	{
		sfname=pllist.GetNext(lpos);
		ZeroMemory(m_PlList[i].Name,16);
		strcpy_s((char*)m_PlList[i].Name,16,(const char*)sfname+4);
		offt=Nitro::GetSubFileOffset(file,Nitro::GetSubFileId(file,sfname),&m_PlList[i].DataLen);
		if(!offt)throw;
		m_PlList[i].pData=new u8[m_PlList[i].DataLen];
		file.Seek(offt,CFile::begin);
		file.Read(m_PlList[i].pData,m_PlList[i].DataLen);
	}


	m_Loaded=true;
	return true;
}
void SqMapSet::Dump(FILE* pf)
{
	if(!m_Loaded)return;
	fprintf(pf,"====================================\n");
	fprintf(pf,"Stage-Step tree\n");
	for(u32 i=0;i<m_StageCount;++i)
	{
		fprintf(pf,"[%02d]Level%dStage%02d\n",i,m_StageList[i].LevelIdx,m_StageList[i].StageIdx);
		for(u16 j=0;j<m_StageList[i].StepCount;++j)
		{
			fprintf(pf,"   [step%02d]bg=x%02X,fgl=x%02X,bgl=x%02X,pl=x%02X\n",j,
				m_StageList[i].StepList[j].BgId,m_StageList[i].StepList[j].FGlId,
				m_StageList[i].StepList[j].BGlId,m_StageList[i].StepList[j].PlId);
		}
	}
	char nameb[17]={0};

	fprintf(pf,"====================================\n");
	fprintf(pf,"Bg\n");
	for(u32 i=0;i<m_BgCount;++i)
	{
		memcpy(nameb,m_BgList[i].Name,16);
		fprintf(pf,"[x%02X]%s\n",i,nameb);
	}
	fprintf(pf,"====================================\n");
	fprintf(pf,"Gl\n");
	for(u32 i=0;i<m_GlCount;++i)
	{
		memcpy(nameb,m_GlList[i].Name,16);
		fprintf(pf,"[x%02X]%s\n",i,nameb);
	}
	fprintf(pf,"====================================\n");
	fprintf(pf,"Pl\n");
	for(u32 i=0;i<m_PlCount;++i)
	{
		memcpy(nameb,m_PlList[i].Name,16);
		fprintf(pf,"[x%02X]%s\n",i,nameb);
	}
}

void SqMapSet::BgDef(CList<CStringA> &list)
{
	CStringA str;
	for(int i=0;i<0x28;++i)
	{
		str.Format("map/d_viw_%02x%s.nsbtx",i,i==0x20?"a":"");
		list.AddTail(str);
	}
}
void SqMapSet::GlDef(CList<CStringA> &list)
{
	CStringA str;
	for(int i=0;i<0x3E;++i)
	{
		if(i!=0x1F){str.Format("map/%02x_b.bin",i);list.AddTail(str);}
		if(i!=0x1F&&i!=3){str.Format("map/%02x_f.bin",i);list.AddTail(str);}
		
	}
	list.AddTail("map/chainbomb.bin");
	list.AddTail("map/front.bin");
	list.AddTail("map/nudemap.bin");
	list.AddTail("map/nudemap2.bin");
	list.AddTail("map/nudemap3.bin");
	list.AddTail("map/nudemap_b.bin");
	list.AddTail("map/nudemap_f.bin");
}
void SqMapSet::PlDef(CList<CStringA> &list)
{
	list.AddTail("map/p_0f.pal");
	list.AddTail("map/p_1a.pal");
	list.AddTail("map/p_21.pal");
	list.AddTail("map/p_22.pal");
	list.AddTail("map/p_23.pal");
	list.AddTail("map/p_24.pal");
	list.AddTail("map/p_26.pal");
	list.AddTail("map/p_2b.pal");
	list.AddTail("map/p_2d.pal");
	list.AddTail("map/pa_08.pal");
	list.AddTail("map/pa_09.pal");
	list.AddTail("map/pa_11.pal");
	list.AddTail("map/pa_12.pal");
	list.AddTail("map/pa_15.pal");
	list.AddTail("map/pa_19.pal");
	list.AddTail("map/pa_1c.pal");
	list.AddTail("map/pa_22.pal");
	list.AddTail("map/pa_27.pal");
	list.AddTail("map/pa_29.pal");
	list.AddTail("map/pa_2d.pal");
	list.AddTail("map/pa_30.pal");


}
u32 SqMapSet::FindSecitem(u8 *GetSiSwitch,char* pname)
{
	ASSERT(m_Loaded);
	for(u8 ss=0;ss<3;++ss)
	{
		for(u32 i=0;i<m_SecitemCount[ss];++i)
		{
			if(strcmp((char*)m_SecitemList[ss][i].Name,pname)==0)
			{
				if(GetSiSwitch)*GetSiSwitch=ss;
				return i;
			}
		}
	}
	return 0xFFFFFFFF;

}
u32 SqMapSet::GetSecitemCount(u8 SiSwitch)
{
	ASSERT(m_Loaded);
	ASSERT(SiSwitch<3);
	return m_SecitemCount[SiSwitch];
}
u8* SqMapSet::GetSecitemBuffer(u8 SiSwitch,u32 index,u32* pGetLen)
{
	ASSERT(m_Loaded);
	ASSERT(SiSwitch<3);
	ASSERT(index<m_SecitemCount[SiSwitch]);
	if(pGetLen)*pGetLen=m_SecitemList[SiSwitch][index].DataLen;
	return m_SecitemList[SiSwitch][index].pData;
}
u8* SqMapSet::ResizeSecitemBuffer(u8 SiSwitch,u32 index,u32 Len)
{
	ASSERT(m_Loaded);
	ASSERT(SiSwitch<3);
	ASSERT(index<m_SecitemCount[SiSwitch]);
	ASSERT(Len);
	delete[] m_SecitemList[SiSwitch][index].pData;
	m_SecitemList[SiSwitch][index].DataLen=Len;
	return m_SecitemList[SiSwitch][index].pData=new u8[Len];
}
void SqMapSet::GetSecitemName(u8 SiSwitch,u32 index,char *pname)
{
	ASSERT(m_Loaded);
	ASSERT(SiSwitch<3);
	ASSERT(index<m_SecitemCount[SiSwitch]);
	memcpy(pname,m_SecitemList[SiSwitch][index].Name,16);
}
void SqMapSet::SetSecitemName(u8 SiSwitch,u32 index,char *pname)
{
	ASSERT(m_Loaded);
	ASSERT(SiSwitch<3);
	ASSERT(index<m_SecitemCount[SiSwitch]);
	memcpy(m_SecitemList[SiSwitch][index].Name,pname,16);
}
u8 *SqMapSet::NewSecitem(u8 SiSwitch,u32 Len,char *pname)
{
	ASSERT(m_Loaded);
	ASSERT(SiSwitch<3);
	SecitemData *NewSecitemList=new SecitemData[++m_SecitemCount[SiSwitch]];
	memcpy(NewSecitemList,m_SecitemList[SiSwitch],(m_SecitemCount[SiSwitch]-1)*sizeof(SecitemData));
	delete[] m_SecitemList[SiSwitch];
	m_SecitemList[SiSwitch]=NewSecitemList;
	memcpy(m_SecitemList[SiSwitch][m_SecitemCount[SiSwitch]-1].Name,pname,16);
	m_SecitemList[SiSwitch][m_SecitemCount[SiSwitch]-1].DataLen=Len;
	return m_SecitemList[SiSwitch][m_SecitemCount[SiSwitch]-1].pData=new u8[Len];
}
void SqMapSet::DeleteSecitem(u8 SiSwitch,u32 index)
{
	ASSERT(m_Loaded);
	ASSERT(SiSwitch<3);
	ASSERT(m_SecitemCount[SiSwitch]>1);//At least two Secitem before delete one
	ASSERT(index<m_SecitemCount[SiSwitch]);
	//Delete the Secitem data
	delete[] m_SecitemList[SiSwitch][index].pData;

	//Create new Secitem List
	SecitemData* NewSecitemList=new SecitemData[--m_SecitemCount[SiSwitch]];
	//Copy the old list to the new one
	if(index)memcpy(NewSecitemList,
		m_SecitemList[SiSwitch],
		index*sizeof(SecitemData));
	if(index<m_SecitemCount[SiSwitch])memcpy(NewSecitemList+index,
		m_SecitemList[SiSwitch]+index+1,
		(m_SecitemCount[SiSwitch]-index)*sizeof(SecitemData));
	//Delete the old list
	delete[] m_SecitemList[SiSwitch];
	m_SecitemList[SiSwitch]=NewSecitemList;

	//Adapt the step info
	for(u32 i=0;i<m_StageCount;++i)for(u16 j=0;j<m_StageList[i].StepCount;++j)
	{
		if(SiSwitch==0)
		{
			if(m_StageList[i].StepList[j].BgId==index)m_StageList[i].StepList[j].BgId=0;
			if(m_StageList[i].StepList[j].BgId>index)--m_StageList[i].StepList[j].BgId;
		}
		else if(SiSwitch==1)
		{
			if(m_StageList[i].StepList[j].BGlId==index)m_StageList[i].StepList[j].BGlId=0;
			if(m_StageList[i].StepList[j].BGlId>index)--m_StageList[i].StepList[j].BGlId;
			if(m_StageList[i].StepList[j].FGlId==index)m_StageList[i].StepList[j].FGlId=0;
			if(m_StageList[i].StepList[j].FGlId>index)--m_StageList[i].StepList[j].FGlId;
		}
		else if(SiSwitch==2)
		{
			if(m_StageList[i].StepList[j].PlId==index)m_StageList[i].StepList[j].PlId=0;
			if(m_StageList[i].StepList[j].PlId>index
				&& m_StageList[i].StepList[j].PlId!=0xFF)--m_StageList[i].StepList[j].PlId;
		}
	}
}
u32 SqMapSet::GetStageIndex(u8 levelidx,u8 substageidx)
{
	ASSERT(m_Loaded);
	for(u32 i=0;i<m_StageCount;++i)
	{
		if(m_StageList[i].LevelIdx==levelidx && m_StageList[i].StageIdx==substageidx)
			return i;
	}
	return 0xFFFFFFFF;

}
u16 SqMapSet::GetStepCount(u32 StageIdx)
{
	ASSERT(m_Loaded);
	ASSERT(StageIdx<m_StageCount);
	return m_StageList[StageIdx].StepCount;

}

u8 *SqMapSet::GetMxpBuffer(u32 StageIdx,u16 StepIndex,u32* pGetLen)
{
	ASSERT(m_Loaded);
	ASSERT(StageIdx<m_StageCount);
	ASSERT(StepIndex<m_StageList[StageIdx].StepCount);
	if(pGetLen)*pGetLen=m_StageList[StageIdx].StepList[StepIndex].MxpLen;
	return m_StageList[StageIdx].StepList[StepIndex].pMxp;
}
u8 *SqMapSet::GetDoeBuffer(u32 StageIdx,u16 StepIndex,u32* pGetLen)
{
	ASSERT(m_Loaded);
	ASSERT(StageIdx<m_StageCount);
	ASSERT(StepIndex<m_StageList[StageIdx].StepCount);
	if(pGetLen)*pGetLen=m_StageList[StageIdx].StepList[StepIndex].DoeLen;
	return m_StageList[StageIdx].StepList[StepIndex].pDoe;
}
void SqMapSet::GetStepInfo(u32 StageIdx,u16 StepIndex,u8* Bg,u8* BGl,u8* FGl,u8* Pl)
{
	ASSERT(m_Loaded);
	ASSERT(StageIdx<m_StageCount);
	ASSERT(StepIndex<m_StageList[StageIdx].StepCount);
	if(Bg)*Bg=m_StageList[StageIdx].StepList[StepIndex].BgId;
	if(BGl)*Bg=m_StageList[StageIdx].StepList[StepIndex].BGlId;
	if(FGl)*Bg=m_StageList[StageIdx].StepList[StepIndex].FGlId;
	if(Pl)*Bg=m_StageList[StageIdx].StepList[StepIndex].PlId;
}
void SqMapSet::SetStepInfo(u32 StageIdx,u16 StepIndex,u8  Bg,u8  BGl,u8  FGl,u8  Pl)
{
	ASSERT(m_Loaded);
	ASSERT(StageIdx<m_StageCount);
	ASSERT(StepIndex<m_StageList[StageIdx].StepCount);
	if(Bg!=0xFE)m_StageList[StageIdx].StepList[StepIndex].BgId=Bg;
	if(BGl!=0xFE)m_StageList[StageIdx].StepList[StepIndex].BgId=BGl;
	if(FGl!=0xFE)m_StageList[StageIdx].StepList[StepIndex].BgId=FGl;
	if(Pl!=0xFE)m_StageList[StageIdx].StepList[StepIndex].BgId=Pl;
}
