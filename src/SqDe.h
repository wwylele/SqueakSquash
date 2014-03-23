#pragma once
#include "MaDeComm.h"
class SqDe
{
private:
	
	enum{
		MAGIC=0x6544,//"De"
	};
	SQROM_STRUCT Header
	{
		u16 Magic;
		u16 HeaderSize;
		u16 FoeDataOffset;
		u16 SupDataOffset;
		u16 Zero;
		u16 Const1;
	};
public:
	SQROM_STRUCT SqItem
	{
		u16 index;
		u16 class_id;
		u16 x;
		u16 y;
		u8 pa;
		u8 pb;
		u8 pc;
		u8 pd;
		u8 pe;
		u8 pf;
		u8 pg;
		u8 ph;
	};
	bool Load(const u8* psrc);
	void LoadDefault();
	inline bool IsLoaded(){return m_Loaded;}
	void Unload();
	u32 MakeLen();
	void Make(u8* pdst);
	SqDe(void);
	~SqDe(void);
	inline SqItem& Foe(u16 i){
		ASSERT(IsLoaded());ASSERT(i<FoeCount);
		return pFoe[i];
	}
	inline SqItem& Sup(u16 i){
		ASSERT(IsLoaded());ASSERT(i<SupCount);
		return pSup[i];
	}
	inline u16 GetFoeCount(){ASSERT(IsLoaded());return FoeCount;}
	inline u16 GetSupCount(){ASSERT(IsLoaded());return SupCount;}
	void RepairIndex();
	void RemoveFoe(u16 i);
	void RemoveSup(u16 i);
	u16 NewFoe();
	u16 NewSup();

	MaDeComm *pMaDeComm;
	void Notify_Door(MadeCommNOTIFY ncode,u16 index);
	void Notify_Mctrl(MadeCommNOTIFY ncode,u8 index);
	void Notify_Foe(MadeCommNOTIFY ncode,u16 index);
	void Notify_Sup(MadeCommNOTIFY ncode,u16 index);
private:
	bool m_Loaded;
	u16 FoeCount;
	SqItem *pFoe;
	u16 SupCount;
	SqItem *pSup;
};