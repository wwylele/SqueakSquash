#pragma once
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
		u16 pa;
		u16 pb;
		u16 pc;
		u16 pd;
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
private:
	bool m_Loaded;
	u16 FoeCount;
	SqItem *pFoe;
	u16 SupCount;
	SqItem *pSup;
};