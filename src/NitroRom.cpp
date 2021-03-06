#include "stdafx.h"
#include "NitroRom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//The code below is copied from another project of mine,
//but I forget how it works...
u16 Nitro::GetSubFileId(CFile& file,const char* pathname)
{
	file.Seek(0,CFile::begin);
	ROM_HEADER rom_header;
	file.Read(&rom_header,sizeof(rom_header));

	u32 fntdir_offset=0;
	ROM_FNTDir fntdir;
	while(1)
	{
		file.Seek(rom_header.fnt_offset+fntdir_offset,CFile::begin);
		file.Read(&fntdir,sizeof(fntdir));
		file.Seek(fntdir.entry_start+rom_header.fnt_offset,CFile::begin);

		u8 fnamelen=0;
		bool isdir;
		for(fnamelen=0;;fnamelen++)
		{
			if(*(pathname+fnamelen)=='\0')
			{
				isdir=false;
				break;
			}
			if(*(pathname+fnamelen)=='/')
			{
				isdir=true;
				break;
			}
		}
		char* namebuf;
		namebuf=new char[fnamelen+1];
		memcpy(namebuf,pathname,fnamelen+1);
		namebuf[fnamelen]='\0';
		pathname+=fnamelen+1;

		u8 fh;
		u8 name_length;
		bool is_dir;
		u16 index_d;
		index_d=0;
		while(1)
		{
			file.Read(&fh,sizeof(fh));
			name_length=fh&0x7F;
			is_dir=fh&0x80?true:false;
			if(!name_length){delete[]namebuf;return 0xFFFF;}
			char* name_buf;
			name_buf=new char[name_length+1];
			file.Read(name_buf,name_length);
			name_buf[name_length]=0;
			if(is_dir==isdir && name_length==fnamelen)
			{
				_strlwr(name_buf);
				_strlwr(namebuf);
				if(!strcmp(name_buf,namebuf))
				{
					delete[] name_buf;
					break;
				}
				
				
			}
			delete[] name_buf;
			if(is_dir)
			{
				file.Seek(2,CFile::current);
			}
			else
			{
				index_d++;
			}
		}
		if(isdir)
		{
			u16 child_dir_id;
			file.Read(&child_dir_id,sizeof(child_dir_id));
			fntdir_offset=(child_dir_id-0xF000)*sizeof(ROM_FNTDir);
		}
		else
		{
			u16 fileid;
			fileid=fntdir.entry_file_id+index_d;
			delete[] namebuf;
			return fileid;
		}
		
		delete[] namebuf;
	}

	return 0xFFFF;

}
u32 Nitro::GetSubFileOffset(CFile& file,u16 id,u32* getlen)
{
	
	file.Seek(0,CFile::begin);
	ROM_HEADER rom_header;
	file.Read(&rom_header,sizeof(rom_header));
	if(id>=rom_header.fat_size/sizeof(ROM_FAT))return 0;

	file.Seek(rom_header.fat_offset+sizeof(ROM_FAT)*id,CFile::begin);
	ROM_FAT fat;
	file.Read(&fat,sizeof(fat));
	if(getlen)*getlen=fat.bottom-fat.top;
	return fat.top;

}

//Copy from DeSmuME
u16 Nitro::Crc16(void *buf,u32 len)
{
	u16 crc=0xFFFF;
	u16 currVal=0;
	const u16 val[] = { 0x0000,0xCC01,0xD801,0x1400,0xF001,0x3C00,0x2800,0xE401,0xA001,0x6C00,0x7800,0xB401,0x5000,0x9C01,0x8801,0x4400};
	len>>=1;
	for(u32 i=0;i<len;++i)
	{
		currVal=((u16*)buf)[i];
		for(int j=0;j<4;++j)
		{
			u16 tabVal=val[crc&0xF];
			crc>>=4;
			crc^=tabVal;
			u16 tempVal=currVal>>(j<<2);
			tabVal=val[tempVal&0xF];
			crc^=tabVal;
		}
	}
	return crc;
}