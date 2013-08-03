#pragma once
class SqDe
{
private:
	
	enum{
		MAGIC=0x6544,//"De"
	};
	struct Header
	{
		u16 Magic;
		u16 HeaderSize;
		u16 DataOffset;
		u16 FDE;
		u16 Zero;
		u16 Const1;
	};
public:
	struct SqItem
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
	inline bool IsLoaded(){return pFoe!=0;}
	void Unload();
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
	u16 FoeCount;
	SqItem *pFoe;
	u16 SupCount;
	SqItem *pSup;
};