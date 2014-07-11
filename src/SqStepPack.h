#pragma once
#include "Main.h"
class SqStepPack
{
public:
	SqStepPack(void);
	~SqStepPack(void);

	void WriteMxp(u8* src,u32 len);
	void WriteDoe(u8* src,u32 len);
	u8* GetMxp(u32 *getlen);
	u8* GetDoe(u32 *getlen);
	void Unload();

	bool Load(CFile &file);
	void Save(CFile &file);
	struct
	{
		u8 Level;
		u8 Stage;
		u16 Step;
	}Header;
private:
	u8 *pMxp,*pDoe;
	u32 MxpLen,DoeLen;
};
