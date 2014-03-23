#include "stdafx.h"
#include "Main.h"
#include "SqMapSet.h"
#include "SqMx.h"
#include "SqMa.h"
#include "SqDe.h"
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
	PrintLog("SqMapSet::Load>\n");
	
	file.Seek(0,CFile::begin);
	SqmsFileHeader header;
	file.Read(&header,sizeof(header));

	//Verify the File Magic
	if(memcmp(header.Magic,"SQSQMAPS",8) || header.Version!=1){
		PrintLog("Fail to load\nMagic=%c%c%c%c%c%c%c%c\nFile Version=%d\n",
			header.Magic[0],header.Magic[1],header.Magic[2],header.Magic[3],
			header.Magic[4],header.Magic[5],header.Magic[6],header.Magic[7],
			header.Version);
		return false;
	}


	SqmsSectionHeader sheader;
	SqmsSecitemHeader siheader;
	u32 fpos,fpos2;

	//Verify the Sections Magic
	PrintLog("Verify the Sections Magic\n");
	file.Seek(header.SectionBgOffset,CFile::begin);
	file.Read(&sheader,sizeof(sheader));
	if(memcmp(sheader.Magic,"SQBG",4))
	{
		PrintLog("Wrong SectionBg Magic\n");
		return false;
	}
	file.Seek(header.SectionGlOffset,CFile::begin);
	file.Read(&sheader,sizeof(sheader));
	if(memcmp(sheader.Magic,"SQGL",4))
	{
		PrintLog("Wrong SectionGl Magic\n");
		return false;
	}
	file.Seek(header.SectionPlOffset,CFile::begin);
	file.Read(&sheader,sizeof(sheader));
	if(memcmp(sheader.Magic,"SQPL",4))
	{
		PrintLog("Wrong SectionPl Magic\n");
		return false;
	}

	Unload();

	//Read Section Bg
	PrintLog("Read Section Bg\n");
	file.Seek(header.SectionBgOffset,CFile::begin);
	file.Read(&sheader,sizeof(sheader));
	m_BgCount=sheader.Count;
	PrintLog("Bg Count=%u\n",m_BgCount);
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
	PrintLog("Read Section Gl\n");
	file.Seek(header.SectionGlOffset,CFile::begin);
	file.Read(&sheader,sizeof(sheader));
	m_GlCount=sheader.Count;
	PrintLog("Gl Count=%u\n",m_GlCount);
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
	PrintLog("Read Section Pl\n");
	file.Seek(header.SectionPlOffset,CFile::begin);
	file.Read(&sheader,sizeof(sheader));
	m_PlCount=sheader.Count;
	PrintLog("Pl Count=%u\n",m_PlCount);
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
	PrintLog("Read Stage-Step-Map tree\n");
	m_StageCount=header.StageCount;
	PrintLog("Stage Count=%u\n",m_StageCount);
	m_StageList=new StageData[m_StageCount];
	SqmsStageHeader stheader;
	SqmsStepHeader spheader;
	fpos=sizeof(header);
	for(u32 i=0;i<m_StageCount;++i)
	{
		file.Seek(fpos,CFile::begin);
		file.Read(&stheader,sizeof(stheader));
		m_StageList[i].EntryLevel=stheader.EntryLevel;
		m_StageList[i].EntryStage=stheader.EntryStage;
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
	PrintLog("Read RomInfo\n");
	file.Seek(header.RomInfoOffset,CFile::begin);
	file.Read(&m_RomInfo,sizeof(m_RomInfo));

	m_Loaded=true;
	PrintLog("SqMapSet::Load done\n");
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
		stageheader.EntryLevel=m_StageList[i].EntryLevel;
		stageheader.EntryStage=m_StageList[i].EntryStage;
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
	PrintLog("SqMapSet::LoadFromRom>\n");
	//Read the ROM header and verify the ROM
	Nitro::ROM_HEADER rom_header;
	file.Seek(0,CFile::begin);
	file.Read(&rom_header,sizeof(rom_header));
	if(strcmp("KIRBY DRO",(char*)rom_header.name))
	{
		PrintLog("Wrong ROM Magic\n");
		return false;
	}
	if(Nitro::Crc16(&rom_header,0x15E)!=rom_header.headerCRC16)
	{
		PrintLog("Wrong CRC\n");
		return false;
	}
	if(Nitro::GetSubFileId(file,"map/"ROM_LOCK_FILE)!=0xFFFF)
	{
		PrintLog("Detected ROM_LOCK_FILE\n");
		return false;
	}

	Unload();


	//Read RomInfo
	PrintLog("Read RomInfo\n");
	Nitro::ROM_TITLE rom_title;
	ZeroMemory(&m_RomInfo,sizeof(m_RomInfo));
	memcpy(&m_RomInfo,&rom_header,22);
	file.Seek(rom_header.title_offset,CFile::begin);
	file.Read(&rom_title,sizeof(rom_title));
	if(rom_title.crc16!=Nitro::Crc16(((u8*)&rom_title)+0x20,0x820))ASSERT(FALSE);
	memcpy(&m_RomInfo.Title_icon_pixel,&rom_title.icon_pixel,512+32+1536);

	//Search and read .mxi file in the ROM
	PrintLog("Search and read .mxi file in the ROM\n");
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
			PrintLog("Read %s",(const char*)strMxiName);
			mxitmp=new u8[mxilen];
			file.Seek(mxioffset,CFile::begin);
			file.Read(mxitmp,mxilen);
			mxi.Load(mxitmp);
			sdtmp.EntryLevel=a;
			sdtmp.EntryStage=s;
			sdtmp.StepCount=mxi.GetStepCount();
			sdtmp.StepList=new StageData::StepData[sdtmp.StepCount];
			//Read step data.
			ASSERT(sdtmp.StepCount<100);
			for(u16 step=0;step<sdtmp.StepCount;++step)
			{
				PrintLog("|");
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
				else//Not find the mxp file,so create one.
				{
					SqMa sqma;
					sqma.LoadDefault();
					sdtmp.StepList[step].MxpLen=sqma.MakeLen();
					sdtmp.StepList[step].pMxp=new u8[sdtmp.StepList[step].MxpLen];
					sqma.Make(sdtmp.StepList[step].pMxp);
					PrintLog("\n>Fail to Load RomSubFile\"%s\"\n",(const char*)mxi.Step(step).Ma);
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
				else{//Not find the doe file,so create one.
					SqDe sqde;
					sqde.LoadDefault();
					sdtmp.StepList[step].DoeLen=sqde.MakeLen();
					sdtmp.StepList[step].pDoe=new u8[sdtmp.StepList[step].DoeLen];
					sqde.Make(sdtmp.StepList[step].pDoe);
					PrintLog("\n>Fail to Load RomSubFile\"%s\"\n",(const char*)mxi.Step(step).De);
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
			PrintLog("\n");

			sdlist.AddTail(sdtmp);

			delete[]mxitmp;
		}
	}

	//Copy stage list from sdlist to m_StageList
	PrintLog("Copy stage list");
	m_StageCount=sdlist.GetCount();
	m_StageList=new StageData[m_StageCount];
	lpos=sdlist.GetHeadPosition();
	for(u32 i=0;i<m_StageCount;++i)
	{
		PrintLog("|");
		m_StageList[i]=sdlist.GetNext(lpos);
	}
	PrintLog("\n");

	//Read bg,gl,pl
	CStringA sfname;
	
	PrintLog("Read Bg");
	m_BgCount=bglist.GetCount();
	m_BgList=new SecitemData[m_BgCount];
	lpos=bglist.GetHeadPosition();
	for(u32 i=0;i<m_BgCount;++i)
	{
		PrintLog("|");
		sfname=bglist.GetNext(lpos);
		ZeroMemory(m_BgList[i].Name,16);
		strcpy_s((char*)m_BgList[i].Name,16,(const char*)sfname+4);
		offt=Nitro::GetSubFileOffset(file,Nitro::GetSubFileId(file,sfname),&m_BgList[i].DataLen);
		ASSERT(offt);
		m_BgList[i].pData=new u8[m_BgList[i].DataLen];
		file.Seek(offt,CFile::begin);
		file.Read(m_BgList[i].pData,m_BgList[i].DataLen);
	}
	PrintLog("\n");

	PrintLog("Read Gl");
	m_GlCount=gllist.GetCount();
	m_GlList=new SecitemData[m_GlCount];
	lpos=gllist.GetHeadPosition();
	for(u32 i=0;i<m_GlCount;++i)
	{
		PrintLog("|");
		sfname=gllist.GetNext(lpos);
		ZeroMemory(m_GlList[i].Name,16);
		strcpy_s((char*)m_GlList[i].Name,16,(const char*)sfname+4);
		offt=Nitro::GetSubFileOffset(file,Nitro::GetSubFileId(file,sfname),&m_GlList[i].DataLen);
		if(offt)
		{
			m_GlList[i].pData=new u8[m_GlList[i].DataLen];
			file.Seek(offt,CFile::begin);
			file.Read(m_GlList[i].pData,m_GlList[i].DataLen);
		}
		else
		{
			m_GlList[i].pData=CreateGl(&m_GlList[i].DataLen);
			PrintLog("\n>Fail to Load RomSubFile\"%s\"\n",(const char*)sfname);

		}
	}
	PrintLog("\n");

	PrintLog("Read Pl");
	m_PlCount=pllist.GetCount();
	m_PlList=new SecitemData[m_PlCount];
	lpos=pllist.GetHeadPosition();
	for(u32 i=0;i<m_PlCount;++i)
	{
		PrintLog("|");
		sfname=pllist.GetNext(lpos);
		ZeroMemory(m_PlList[i].Name,16);
		strcpy_s((char*)m_PlList[i].Name,16,(const char*)sfname+4);
		offt=Nitro::GetSubFileOffset(file,Nitro::GetSubFileId(file,sfname),&m_PlList[i].DataLen);
		ASSERT(offt);
		m_PlList[i].pData=new u8[m_PlList[i].DataLen];
		file.Seek(offt,CFile::begin);
		file.Read(m_PlList[i].pData,m_PlList[i].DataLen);
	}
	PrintLog("\n");

	PrintLog("Repair Mxp Header\n");
	RepairMxpHeader();


	m_Loaded=true;
	PrintLog("SqMapSet::ReadFromRom done\n");
	return true;
}
void SqMapSet::Dump(FILE* pf)
{
	if(!m_Loaded)return;
	fprintf(pf,"====================================\n");
	fprintf(pf,"Stage-Step tree\n");
	for(u32 i=0;i<m_StageCount;++i)
	{
		fprintf(pf,"[%02d]Level%dStage%02d\n",i,m_StageList[i].EntryLevel,m_StageList[i].EntryStage);
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
	for(int i=0;i<0x3E/*0x40*/;++i)//0x40 is the range of USA ROM,larger than JAP ROM
	{
		if(i!=0x1F){str.Format("map/%02x_b.bin",i);list.AddTail(str);}
		if(i!=0x1F&&i!=3){str.Format("map/%02x_f.bin",i);list.AddTail(str);}
		
	}
	list.AddTail("map/chainbomb.bin");
	//the files below are not existed in the USA ROM, so delete them
	//Undo
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
u32 SqMapSet::FindSecitem(u8 *GetSiSwitch,const char* pname)
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

	//Adjust the step info
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
			if(m_StageList[i].StepList[j].PlId==index
				|| m_StageList[i].StepList[j].PlId==0xFF)m_StageList[i].StepList[j].PlId=0xFF;
			else if(m_StageList[i].StepList[j].PlId>index)--m_StageList[i].StepList[j].PlId;
				
		}
	}
}


u32 SqMapSet::GetStageIndex(u8 levelidx,u8 substageidx)
{
	ASSERT(m_Loaded);
	for(u32 i=0;i<m_StageCount;++i)
	{
		if(m_StageList[i].EntryLevel==levelidx && m_StageList[i].EntryStage==substageidx)
			return i;
	}
	return 0xFFFFFFFF;

}




void SqMapSet::GetStepInfo(u32 StageIdx,u16 StepIndex,u8* Bg,u8* BGl,u8* FGl,u8* Pl)
{
	ASSERT(m_Loaded);
	ASSERT(StageIdx<m_StageCount);
	ASSERT(StepIndex<m_StageList[StageIdx].StepCount);
	if(Bg)*Bg=m_StageList[StageIdx].StepList[StepIndex].BgId;
	if(BGl)*BGl=m_StageList[StageIdx].StepList[StepIndex].BGlId;
	if(FGl)*FGl=m_StageList[StageIdx].StepList[StepIndex].FGlId;
	if(Pl)*Pl=m_StageList[StageIdx].StepList[StepIndex].PlId;
}
void SqMapSet::SetStepInfo(u32 StageIdx,u16 StepIndex,u8  Bg,u8  BGl,u8  FGl,u8  Pl)
{
	ASSERT(m_Loaded);
	ASSERT(StageIdx<m_StageCount);
	ASSERT(StepIndex<m_StageList[StageIdx].StepCount);
	if(Bg!=0xFE)m_StageList[StageIdx].StepList[StepIndex].BgId=Bg;
	if(BGl!=0xFE)m_StageList[StageIdx].StepList[StepIndex].BGlId=BGl;
	if(FGl!=0xFE)m_StageList[StageIdx].StepList[StepIndex].FGlId=FGl;
	if(Pl!=0xFE)m_StageList[StageIdx].StepList[StepIndex].PlId=Pl;
}
u8 *SqMapSet::ResizeMxpBuffer(u32 StageIdx,u16 StepIndex,u32 Len)
{
	ASSERT(m_Loaded);
	ASSERT(StageIdx<m_StageCount);
	ASSERT(StepIndex<m_StageList[StageIdx].StepCount);
	ASSERT(Len);
	delete[] m_StageList[StageIdx].StepList[StepIndex].pMxp;
	m_StageList[StageIdx].StepList[StepIndex].MxpLen=Len;
	return m_StageList[StageIdx].StepList[StepIndex].pMxp=new u8[Len];
}
u8 *SqMapSet::ResizeDoeBuffer(u32 StageIdx,u16 StepIndex,u32 Len)
{
	ASSERT(m_Loaded);
	ASSERT(StageIdx<m_StageCount);
	ASSERT(StepIndex<m_StageList[StageIdx].StepCount);
	ASSERT(Len);
	delete[] m_StageList[StageIdx].StepList[StepIndex].pDoe;
	m_StageList[StageIdx].StepList[StepIndex].DoeLen=Len;
	return m_StageList[StageIdx].StepList[StepIndex].pDoe=new u8[Len];
}
void SqMapSet::DeleteStep(u32 StageIdx,u16 StepIndex)
{
	ASSERT(m_Loaded);
	ASSERT(StageIdx<m_StageCount);
	ASSERT(m_StageList[StageIdx].StepCount>1);//At least two Step before delete one
	ASSERT(StepIndex<m_StageList[StageIdx].StepCount);

	//Delete the Step data
	delete[] m_StageList[StageIdx].StepList[StepIndex].pMxp;
	delete[] m_StageList[StageIdx].StepList[StepIndex].pDoe;

	//Create new Step List
	StageData::StepData* NewStepList=new StageData::StepData[--m_StageList[StageIdx].StepCount];
	//Copy the old list to the new one
	if(StepIndex)memcpy(NewStepList,
		m_StageList[StageIdx].StepList,
		StepIndex*sizeof(StageData::StepData));
	if(StepIndex<m_StageList[StageIdx].StepCount)memcpy(NewStepList+StepIndex,
		m_StageList[StageIdx].StepList+StepIndex+1,
		(m_StageList[StageIdx].StepCount-StepIndex)*sizeof(StageData::StepData));
	//Delete the old list
	delete[] m_StageList[StageIdx].StepList;
	m_StageList[StageIdx].StepList=NewStepList;
}

u8* SqMapSet::CreateGl(u32* pGetLen)
{
	u8* pData=new u8[34320];
	ZeroMemory(pData,34320);
	((u32*)pData)[0]=0x10;
	((u32*)pData)[1]=0x200;
	((u32*)pData)[2]=0x8000;
	((u32*)pData)[3]=0x400;
	return pData;
}
u8 *SqMapSet::GetSecitemBuffer(u8 SiSwitch,u32 index,u32* pGetLen)
{
	ASSERT(m_Loaded);
	ASSERT(SiSwitch<3);
	ASSERT(index<m_SecitemCount[SiSwitch]);
	if(pGetLen)*pGetLen=m_SecitemList[SiSwitch][index].DataLen;
	return m_SecitemList[SiSwitch][index].pData;
}
void SqMapSet::GetSecitemName(u8 SiSwitch,u32 index,char *pname)
{
	ASSERT(m_Loaded);
	ASSERT(SiSwitch<3);
	ASSERT(index<m_SecitemCount[SiSwitch]);
	memcpy(pname,m_SecitemList[SiSwitch][index].Name,16);
}
void SqMapSet::SetSecitemName(u8 SiSwitch,u32 index,const char *pname)
{
	ASSERT(m_Loaded);
	ASSERT(SiSwitch<3);
	ASSERT(index<m_SecitemCount[SiSwitch]);
	memcpy(m_SecitemList[SiSwitch][index].Name,pname,16);
}
void SqMapSet::GetStageEntry(u32 StageIdx,u8 *plevelidx,u8 *psubstageidx)
{
	ASSERT(m_Loaded);
	ASSERT(StageIdx<m_StageCount);
	if(plevelidx)*plevelidx=m_StageList[StageIdx].EntryLevel;
	if(psubstageidx)*psubstageidx=m_StageList[StageIdx].EntryStage;
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
void SqMapSet::RepairMxpHeader(u32 StageIdx,u16 StepIdx)
{
	if(StageIdx==0xFFFFFFFF)
	{
		for(u32 i=0;i<m_StageCount;++i)
			RepairMxpHeader(i);
		return;
	}
	ASSERT(StageIdx<m_StageCount);
	if(StepIdx==0xFFFF)
	{
		for(u16 i=0;i<m_StageList[StageIdx].StepCount;++i)
			RepairMxpHeader(StageIdx,i);
		return;
	}
	ASSERT(StepIdx<m_StageList[StageIdx].StepCount);
	SqMa::Header &head=*(SqMa::Header*)m_StageList[StageIdx].StepList[StepIdx].pMxp;
	head.Attribute.EntryLevel_ZB=m_StageList[StageIdx].EntryLevel-1;
	head.Attribute.EntryStage_ZB=m_StageList[StageIdx].EntryStage-1;
}
u16 SqMapSet::NewStep(u32 StageIdx)
{
	ASSERT(m_Loaded);
	ASSERT(StageIdx<m_StageCount);
	StageData::StepData* newList=new StageData::StepData[m_StageList[StageIdx].StepCount+1];
	memcpy(newList,m_StageList[StageIdx].StepList,
		m_StageList[StageIdx].StepCount*sizeof(StageData::StepData));
	delete[] m_StageList[StageIdx].StepList;
	m_StageList[StageIdx].StepList=newList;
	m_StageList[StageIdx].StepList[m_StageList[StageIdx].StepCount].BgId=0;
	m_StageList[StageIdx].StepList[m_StageList[StageIdx].StepCount].BGlId=0;
	m_StageList[StageIdx].StepList[m_StageList[StageIdx].StepCount].FGlId=0;
	m_StageList[StageIdx].StepList[m_StageList[StageIdx].StepCount].PlId=0xFF;
	m_StageList[StageIdx].StepList[m_StageList[StageIdx].StepCount].MxpLen=16;
	m_StageList[StageIdx].StepList[m_StageList[StageIdx].StepCount].pMxp=new u8[16];
	m_StageList[StageIdx].StepList[m_StageList[StageIdx].StepCount].DoeLen=16;
	m_StageList[StageIdx].StepList[m_StageList[StageIdx].StepCount].pDoe=new u8[16];
	++m_StageList[StageIdx].StepCount;
	return m_StageList[StageIdx].StepCount-1;
}
u32 SqMapSet::GetSubFileCount()
{
	ASSERT(m_Loaded);
	u32 count;
	count=m_BgCount+m_GlCount+m_PlCount+m_StageCount;
	for(u32 i=0;i<m_StageCount;++i)count+=m_StageList[i].StepCount*2;
	return count;
}

bool SqMapSet::MakeRom(CFile &file)
{
	PrintLog("SqMapSet::MakeRom>\n");
	ASSERT(m_Loaded);

	//Verify the ROM
	file.Seek(0,CFile::begin);
	Nitro::ROM_HEADER rom_header;
	file.Seek(0,CFile::begin);
	file.Read(&rom_header,sizeof(rom_header));
	if(strcmp("KIRBY DRO",(char*)rom_header.name))
	{
		PrintLog("Wrong ROM Magic\n");
		return false;
	}
	if(Nitro::GetSubFileId(file,"map/"ROM_LOCK_FILE)!=0xFFFF)
	{
		PrintLog("Detected ROM_LOCK_FILE\n");
		return false;
	}

	struct NSFA
	{
		CStringA name;
		u8* pData;
		u32 DataLen;
	} nsfa;
	CList<NSFA> nsfal;

	POSITION pos;

	//ROM_LOCK_FILE
	PrintLog("List ROM_LOCK_FILE File\n");
	u8 dummydata_ROM_LOCK_FILE=0;
	nsfa.name=ROM_LOCK_FILE;
	nsfa.pData=&dummydata_ROM_LOCK_FILE;
	nsfa.DataLen=1;
	nsfal.AddTail(nsfa);

	//Bg File
	PrintLog("List Bg File\n");
	for(u32 i=0;i<m_BgCount;++i)
	{
		nsfa.name.Format("%Xzb",i);
		nsfa.pData=m_BgList[i].pData;
		nsfa.DataLen=m_BgList[i].DataLen;
		nsfal.AddTail(nsfa);
	}

	//Gl File
	PrintLog("List Gl File\n");
	for(u32 i=0;i<m_GlCount;++i)
	{
		if(!strcmp((char*)m_GlList[i].Name,"chainbomb.bin"))nsfa.name="chainbomb.bin";
		else nsfa.name.Format("%Xzg",i);
		nsfa.pData=m_GlList[i].pData;
		nsfa.DataLen=m_GlList[i].DataLen;
		nsfal.AddTail(nsfa);
	}

	//Pl File
	PrintLog("List Pl File\n");
	for(u32 i=0;i<m_PlCount;++i)
	{
		nsfa.name.Format("%Xzp",i);
		nsfa.pData=m_PlList[i].pData;
		nsfa.DataLen=m_PlList[i].DataLen;
		nsfal.AddTail(nsfa);
	}

	//Stage-Step File
	PrintLog("List .mxi.mxp.doe File\n");
	SqMx sqmx;
	u8** mxi=new u8*[m_StageCount];
	CList<u8*> mxpl;
	u8* mxp;
	u32 accum=0;
	for(u32 i=0;i<m_StageCount;++i)
	{
		//Create a mxi file
		sqmx.Create(m_StageList[i].StepCount);
		sqmx.EntryLevel=m_StageList[i].EntryLevel;
		sqmx.EntryStage=m_StageList[i].EntryStage;

		//Step File
		for(u16 j=0;j<m_StageList[i].StepCount;++j)
		{
			//sqmx.Step(j).Ma.Format("a%ds%ds%d.mxp",
			//	m_StageList[i].LevelIdx,m_StageList[i].StageIdx,j+1);
			sqmx.Step(j).Ma.Format("%Xzm",accum);
			nsfa.name=sqmx.Step(j).Ma;
			sqmx.Step(j).Ma="map/"+sqmx.Step(j).Ma;
			mxp=new u8[m_StageList[i].StepList[j].MxpLen];
			nsfa.pData=mxp;
			nsfa.DataLen=Nitro::CompressRL(m_StageList[i].StepList[j].pMxp,m_StageList[i].StepList[j].MxpLen,mxp);
			mxpl.AddTail(mxp);
			nsfal.AddTail(nsfa);

			//sqmx.Step(j).De.Format("a%ds%ds%d.doe",
			//	m_StageList[i].LevelIdx,m_StageList[i].StageIdx,j+1);
			sqmx.Step(j).De.Format("%Xzd",accum);
			nsfa.name=sqmx.Step(j).De;
			sqmx.Step(j).De="map/"+sqmx.Step(j).De;
			nsfa.pData=m_StageList[i].StepList[j].pDoe;
			nsfa.DataLen=m_StageList[i].StepList[j].DoeLen;
			nsfal.AddTail(nsfa);

			++accum;

			sqmx.Step(j).Bg.Format("map/%Xzb",m_StageList[i].StepList[j].BgId);
			sqmx.Step(j).Bb.Format("map/%Xzg",m_StageList[i].StepList[j].BGlId);
			sqmx.Step(j).Fb.Format("map/%Xzg",m_StageList[i].StepList[j].FGlId);
			if(m_StageList[i].StepList[j].PlId!=0xFF)
				sqmx.Step(j).Pl.Format("map/%Xzp",m_StageList[i].StepList[j].PlId);
			else
				sqmx.Step(j).Pl="";
		}

		nsfa.name.Format("a%ds%d.mxi",m_StageList[i].EntryLevel,m_StageList[i].EntryStage);
		sqmx.Make(&mxi[i],&nsfa.DataLen);
		nsfa.pData=mxi[i];
		nsfal.AddTail(nsfa);
	}

	//Import the file into the ROM

	PrintLog("Search for FNT,FAT entry\n");
	u32 fpos_fnt,fpos_fat,fpos_fim;
	Nitro::ROM_FNTDir fntdir;
	file.Seek(rom_header.fnt_offset,CFile::begin);//Seek to the Root folder
	file.Read(&fntdir,sizeof(fntdir));
	file.Seek(rom_header.fnt_offset+fntdir.entry_start,CFile::begin);//Seek to the Root Dir
	//search for the folder "map"
	u8 fh;
	u8 name_length;
	char namebuf[4]={0};
	u16 folderid;
	while(1)
	{
		file.Read(&fh,1);
		name_length=fh&0x7F;
		if(name_length==3)//Length"map"==3
		{
			file.Read(namebuf,3);
			if(!strcmp(_strlwr(namebuf),"map"))
			{
				file.Read(&folderid,2);
				break;
			}	
		}
		else
		{
			file.Seek(name_length,CFile::current);
		}
		if(fh&0x80)file.Seek(2,CFile::current);
	}
	//Seek to the map Dir
	file.Seek(rom_header.fnt_offset+(folderid-0xF000)*sizeof(fntdir),CFile::begin);
	file.Read(&fntdir,sizeof(fntdir));
	fpos_fnt=rom_header.fnt_offset+fntdir.entry_start;
	fpos_fat=rom_header.fat_offset+fntdir.entry_file_id*sizeof(Nitro::ROM_FAT);
	/*//Search for the data end
	PrintLog("Search for FIM entry\n");
	u32 flen=(u32)file.GetLength();
	u8* psrc,*psrce;
	{
		HANDLE hFileMapping=CreateFileMapping(file.m_hFile,0,PAGE_READONLY,0,0,0);
		psrc=(u8*)MapViewOfFile(hFileMapping,FILE_MAP_READ,0,0,0);
		psrce=psrc+flen-1;
		for(;*psrce==0xFF;--psrce);
		UnmapViewOfFile(psrc);
		CloseHandle(hFileMapping);
	}
	fpos_fim=(psrce-psrc+0x18)&0xFFFFFFF0;*/
	fpos_fim=(rom_header.ROMSize+32)&0xFFFFFFF0;
	//Write all file in
	PrintLog("Write all file in\n");
	Nitro::ROM_FAT fat;
	for(pos=nsfal.GetHeadPosition();pos;)
	{
		nsfa=nsfal.GetNext(pos);
		//FNT
		file.Seek(fpos_fnt,CFile::begin);
		fh=nsfa.name.GetLength();
		file.Write(&fh,1);
		file.Write((const char*)nsfa.name,fh);
		fpos_fnt=(u32)file.GetPosition();
		//FAT
		file.Seek(fpos_fat,CFile::begin);
		fat.top=fpos_fim;
		fat.bottom=fpos_fim+nsfa.DataLen;
		file.Write(&fat,sizeof(fat));
		fpos_fat=(u32)file.GetPosition();
		//FIM
		file.Seek(fpos_fim,CFile::begin);
		file.Write(nsfa.pData,nsfa.DataLen);
		fpos_fim=((u32)file.GetPosition()+0x10)&0xFFFFFFF0;
	}
	//FNT end
	file.Seek(fpos_fnt,CFile::begin);
	fh=0;
	file.Write(&fh,1);

	//ROM Header;
	memcpy(&rom_header.id,&m_RomInfo.Rom_id,10);
	++rom_header.device_caps;
	file.Seek(fpos_fim,CFile::begin);rom_header.ROMSize=(u32)file.GetPosition();
	rom_header.headerCRC16=Nitro::Crc16(&rom_header,0x15E);
	file.Seek(0,CFile::begin);
	file.Write(&rom_header,sizeof(rom_header));

	//ROM Title
	Nitro::ROM_TITLE rom_title;
	file.Seek(rom_header.title_offset,CFile::begin);
	file.Read(&rom_title,sizeof(rom_title));
	memcpy(rom_title.icon_pixel,&m_RomInfo.Title_icon_pixel,512+32+1536);
	rom_title.crc16=Nitro::Crc16((u8*)&rom_title+0x20,0x820);
	file.Seek(rom_header.title_offset,CFile::begin);
	file.Write(&rom_title,sizeof(rom_title));
	

	//Clear the mxi,mxp file
	PrintLog("Clear\n");
	for(u32 i=0;i<m_StageCount;++i)delete[] mxi[i];
	delete[] mxi;
	for(pos=mxpl.GetHeadPosition();pos;)delete[] mxpl.GetNext(pos);

	PrintLog("SqMapSet::MakeRom done\n");
	return true;
}
void SqMapSet::MoveStep(u32 StageIdx,u16 StepIndex,u16 NewStepIndex)
{
	ASSERT(m_Loaded);
	ASSERT(StageIdx<m_StageCount);
	ASSERT(StepIndex<m_StageList[StageIdx].StepCount);
	ASSERT(NewStepIndex<m_StageList[StageIdx].StepCount);
	if(StepIndex==NewStepIndex)return;
	StageData::StepData t=m_StageList[StageIdx].StepList[StepIndex];
	if(StepIndex<NewStepIndex)
	{
		memmove(m_StageList[StageIdx].StepList+StepIndex,
			m_StageList[StageIdx].StepList+StepIndex+1,
			(NewStepIndex-StepIndex)*sizeof(StageData::StepData));
	}
	else
	{
		memmove(m_StageList[StageIdx].StepList+NewStepIndex+1,
			m_StageList[StageIdx].StepList+NewStepIndex,
			(StepIndex-NewStepIndex)*sizeof(StageData::StepData));
	}
	m_StageList[StageIdx].StepList[NewStepIndex]=t;
}