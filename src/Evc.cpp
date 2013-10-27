
//Unicode


#include "stdafx.h"
#include "Evc.h"
#include <list>




const EVC_DESC EvcDesc[]={

	//0
	{4,		_T("按钮")},
	{6,		_T("绳索顶?")},
	{2,		_T("按钮控制机关")},
	{2,		_T("悬挂平台")},
	{8,		_T("活动平台")},
	{12,	_T("云平台")},
	{2,		_T("滚石")},
	{4,		_T("火柱")},
	{2,		_T("落石")},
	{2,		_T("冰锥")},

	//10
	{2,		_T("蜡烛")},
	{2,		_T("大炮")},
	{2,		_T("点火大炮")},
	{64,	_T("传送星")},
	{2,		_T("草丛")},
	{2,		_T("木桶")},
	{2,		_T("雷管")},
	{2,		_T("FOE/SUP出现?")},
	{8,		_T("背景变色触发器")},
	{8,		_T("触发器？")},

	//20
	{14,	_T("？")},
	{6,		_T("触发器？")},
	{16,	_T("")},
	{12,	_T("")},
	{4,		_T("画面特效？")},
	{2,		_T("")},
	{4,		_T("")},
};

u16 GetEvcDataLen(u8 class_id)
{
	return EvcDesc[class_id].DataLen;
}

SqEvpPack::SqEvpPack():pEvp(0),EvpCount(0)
{
}
SqEvpPack::~SqEvpPack()
{
	Unload();
}
void SqEvpPack::ToMa(SqMa& sqma)
{
	ASSERT(sqma.IsLoaded());

	//Clear old data
	if(sqma.pEvp){delete[] sqma.pEvp;sqma.pEvp=0;}
	if(sqma.pEvc){
		for(u8 i=0;i<sqma.EvcCount;++i)delete[] sqma.pEvc[i].pData;
		delete[] sqma.pEvc;sqma.pEvc=0;
	}
	sqma.EvcCount=0;

	struct EVCTB
	{
		u8 class_id;
		std::list<u8*> ext_data_pl;
	};
	std::list<EVCTB> evctbl;
	EVCTB evctbt;

	bool needp;
	if(sqma.EvpCount=EvpCount)
	{

		//EvpIn;
		sqma.pEvp=new SqMa::EvpData[EvpCount];
		for(u8 i=0;i<EvpCount;++i)
		{
			sqma.pEvp[i].class_id=pEvp[i].class_id;
			sqma.pEvp[i].x=pEvp[i].x;
			sqma.pEvp[i].y=pEvp[i].y;
			sqma.pEvp[i].param=pEvp[i].param;

			//apply for id
			needp=true;
			for(std::list<EVCTB>::iterator iter=evctbl.begin();
				iter!=evctbl.end();iter++)
			{
				if(iter->class_id==pEvp[i].class_id)
				{
					sqma.pEvp[i].id=(u8)iter->ext_data_pl.size();
					iter->ext_data_pl.push_back(pEvp[i].pExtData);
					needp=false;
					break;
				}
			}
			if(needp)
			{
				evctbt.class_id=pEvp[i].class_id;
				evctbl.push_back(evctbt);
				evctbl.back().ext_data_pl.push_back(pEvp[i].pExtData);
				sqma.pEvp[i].id=0;
			}

		}

		//EvcIn
		sqma.EvcCount=(u8)evctbl.size();
		sqma.pEvc=new SqMa::EvcData[sqma.EvcCount];
		u8 i=0,j;
		for(std::list<EVCTB>::iterator iter=evctbl.begin();
				iter!=evctbl.end();iter++,++i)
		{
			sqma.pEvc[i].Header.class_id=iter->class_id;
			sqma.pEvc[i].Header.data_len=GetEvcDataLen(iter->class_id);
			sqma.pEvc[i].Header.count=(u8)iter->ext_data_pl.size();
			sqma.pEvc[i].pData=new u8[sqma.pEvc[i].Header.data_len*
				sqma.pEvc[i].Header.count];
			j=0;
			for(std::list<u8*>::iterator jter=iter->ext_data_pl.begin();
				jter!=iter->ext_data_pl.end();jter++,++j)
				memcpy(sqma.pEvc[i].pData+sqma.pEvc[i].Header.data_len*j,*jter,
					sqma.pEvc[i].Header.data_len);
		}	
	}
}
void SqEvpPack::FromMa(SqMa& sqma)
{
	ASSERT(sqma.IsLoaded());
	Unload();

	EvpCount=sqma.EvpCount;
	if(EvpCount)
	{
		pEvp=new EvpNote[EvpCount];
		for(u8 i=0;i<EvpCount;++i)
		{
			pEvp[i].class_id=sqma.pEvp[i].class_id;
			pEvp[i].x=sqma.pEvp[i].x;
			pEvp[i].y=sqma.pEvp[i].y;
			pEvp[i].param=sqma.pEvp[i].param;
			pEvp[i].pExtData=new u8[GetEvcDataLen(pEvp[i].class_id)];
			for(u8 j=0;;++j)
			{
				ASSERT(j<sqma.EvcCount);
				if(sqma.pEvc[j].Header.class_id==pEvp[i].class_id)
				{
					ASSERT(GetEvcDataLen(pEvp[i].class_id)==sqma.pEvc[j].Header.data_len);
					ASSERT(sqma.pEvp[i].id<sqma.pEvc[j].Header.count);
					memcpy(pEvp[i].pExtData,
						sqma.pEvc[j].pData+sqma.pEvp[i].id*sqma.pEvc[j].Header.data_len,
						sqma.pEvc[j].Header.data_len);
					break;
				}
			}
			
		}
	}
}
void SqEvpPack::Unload()
{
	if(pEvp)
	{
		for(u8 i=0;i<EvpCount;++i)delete[] pEvp[i].pExtData;
		delete[] pEvp;
		pEvp=0;
	}
	EvpCount=0;
}