#pragma once
#include "MaDeComm.h"

#define MAX_FOE_COUNT 250
#define MAX_SUP_COUNT 250

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

		union{
			u8 pa;
			u8 foe_a;
		};
		union{
			u8 pb;
			u8 foe_b;
		};
		union{
			u8 pc;
			u8 foe_c;
		};
		union{
			u8 pd;
			u8 sup_flag;
			u8 foe_d;
		};
		union{
			u8 pe;
			u8 sup_ff;
			u8 foe_e;
		};
		union{
			u8 pf;
			u8 sup_treasure_id;
		};
		union{
			u8 pg;
			u8 sup_treasure_valid;
		};
		u8 ph;

	};
#define SQ_ITEM_SUP_FLAG_INVISIBLE     1
#define SQ_ITEM_SUP_FLAG_BUBBLE        2
#define SQ_ITEM_SUP_FLAG_SQNOW         4

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
	void DownFoe(u16 i);
	void DownSup(u16 i);


	MaDeComm *pMaDeComm;
	void Notify_Door(MaDeCommNOTIFY ncode,u16 index);
	void Notify_Mctrl(MaDeCommNOTIFY ncode,u8 index);
	void Notify_Foe(MaDeCommNOTIFY ncode,u16 index);
	void Notify_Sup(MaDeCommNOTIFY ncode,u16 index);
private:
	bool m_Loaded;
	u16 FoeCount;
	SqItem *pFoe;
	u16 SupCount;
	SqItem *pSup;
};