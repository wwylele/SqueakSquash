#pragma once
#include "NitroCompress.h"
#include "NitroRom.h"
namespace Nitro{
	struct Color15
	{
		u16 r:5;
		u16 g:5;
		u16 b:5;
		u16 x:1;
	};
	inline COLORREF Color15to24(Color15 value)
	{
		return RGB(value.r*255/31,
			value.g*255/31,
			value.b*255/31);
	}
	struct Tile
	{
		u8 dt[32];
		inline u8 Tile::Get(u8 x,u8 y)
		{
			u8& k=dt[(y<<2)|(x>>1)];
			return (x&1) ? k>>4:k&0xF;
		}
		inline void Tile::Set(u8 x,u8 y,u8 value)
		{
			u8& k=dt[(y<<2)|(x>>1)];
			if(x&1)
			{
				k=(k&0xF)|(value<<4);
				
			}
			else
			{
				k=(k&0xF0)|(value&0xF);
			}
		}
		inline bool Tile::operator ==(Tile& value)
		{
			for(int i=0;i<32;++i)
			{
				if(dt[i]!=value.dt[i])return false;
			}
			return true;
		}
	};
}