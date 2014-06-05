#include "stdafx.h"
#include "Main.h"
#include "LogoIn.h"
#include "Nitro.h"

const u8 LogoFileHeader[24]=
{
	0x18,0x00,0x00,0x00,
	0x20,0x00,0x00,0x00,
	0x20,0x0B,0x00,0x00,
	0x00,0x06,0x00,0x00,
	0x00,0x01,0x00,0x00,
	0xC0,0x00,0x00,0x00
};
u8* CreateLogoFile()
{
	u8 *pData=new u8[LOGO_FILE_LENGTH];
	memset(pData,0,LOGO_FILE_LENGTH);
	u8 *p=pData;
	memcpy(p,LogoFileHeader,24);p+=24;

	//Pal
	*(p+4)=0xFF;
	*(p+5)=0xFF;
	*(p+6)=0x0;
	*(p+7)=0xFF;
	p+=0x20;

	//Tile
	Nitro::Tile tile;
	for(u8 y=0;y<8;++y)for(u8 x=0;x<8;++x)
	{
		tile.Set(x,y,2);
	}
	memcpy( p,&tile,32);
	CImage BmpLogo;
	BmpLogo.LoadFromResource(AfxGetApp()->m_hInstance,IDB_LOGO);
	for(int by=0;by<5;++by)for(int bx=0;bx<16;++bx)
	{
		for(u8 y=0;y<8;++y)for(u8 x=0;x<8;++x)
		{
			if(rand()%20<2)tile.Set(x,y,3);
			else tile.Set(x,y,BmpLogo.GetPixel(bx*8+x,by*8+y)?2:1);
		}
		memcpy( p+32*(bx+by*16 +1),&tile,32);
	}
	BmpLogo.Destroy();
	p+=0xB20;

	//Mapping
	Nitro::CharData cd;
	cd.flipx=cd.flipy=0;
	cd.plt=0;
	int tx,ty;
	for(int y=0;y<24;++y)for(int x=0;x<32;++x)
	{
		tx=x-8;
		ty=y-8;
		if(ty>-1 && tx>-1 && ty<5 && tx<16)cd.tile=tx+ty*16+1;
		else cd.tile=0;
		memcpy( p+2*(x+y*32),&cd,2);
	}


	return pData;
}