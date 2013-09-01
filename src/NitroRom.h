namespace Nitro
{
	//The structures below are adapted from the document in NitroSDK
	//
	struct ROM_HEADER
	{

		u8 name[12];
		u8 id[4];
		u8 maker_code[2];
		u8 product_code;
		u8 device_type;
		u8 device_caps;
		u8 rom_version;
		u16 rom_ctrl_info[5];

		//
		// 0x020 b)  Parameter for static module
		//
		//	ARM9
		u32	main_rom_offset;	// Transmit source ROM offset
		u32	main_entry_address;	// Execution start address (not implemented)
		u32	main_ram_address;	// Transmit destination RAM address
		u32	main_size;	// Transmit size
		//	ARM7
		u32	sub_rom_offset;	// Transmit source ROM offset
		u32	sub_entry_address;	// Execution start address (not implemented)
		u32	sub_ram_address;	// Transmit destination RAM address
		u32	sub_size;	// Transmit size
		
		//
		// 0x040 c)  Parameter for file name table
		//
		u32	fnt_offset;	// Top ROM offset 
		u32	fnt_size;	// Table size 
		//	
		// 0x048 e)  Parameter for file allocation table
		//
		u32	fat_offset;	// Top ROM offset 
		u32	fat_size;	// Table size 
		//
		// 0x0050 d)  Parameter for overlay header table
		//
		//	ARM9
		u32	main_ovt_offset;	// Top ROM offset
		u32	main_ovt_size;	// Table size 
		
		//	ARM7
		u32	sub_ovt_offset;	// Top ROM offset
		u32	sub_ovt_size;	// Table size 

		//
		// 0x0060 - 0x04000 Reserved region for system B
		//
		//u8	reserved_B[16];
		u32 reserved_B1;
		u32 reserved_B2;
		u32 title_offset;
		u16 CRC16;
		u16 ROMtimeout;

		/*
		u8	reserved_C[4*1024-0x70];	// Reserved for system C
		u8	reserved_D[12*1024];	// Reserved for system D
		*/
		u32     ARM9unk;
		u32     ARM7unk;
       
		u8      unknown3c[8];
		u32     ROMSize;
		u32     HeaderSize;
		u8      unknown5[56];
		u8      logo[156];
		u16     logoCRC16;
		u16     headerCRC16;
		u8      reserved[160];
		
	};
	struct ROM_FNTDir
	{
		u32	entry_start;	// Reference location of entry name
		u16	entry_file_id;	// File ID of top entry 
		u16	parent_id;	// ID of parent directory
	};
	struct ROM_FAT 
	{
		u32	top;	// Top ROM address of file 
		u32	bottom;	// Bottom ROM address of file
	};	
	struct ROM_TITLE
	{
		u16 version; //Version  (0001h)
		u16 crc16; //CRC16 across entries 020h..83Fh
		u8 reserved[0x1C]; //Reserved (zero-filled)
		u8 icon_pixel
			[/*block_y*/4]
			[/*block_x*/4]
			[/*pixel_y*/8]
			[/*pixel_x/2*/4];//4 bit per pixel
		u16 icon_palette[16];
		u16 title[6][128];//Unicode
	};


	u16 GetSubFileId(CFile& file,const char* subfilename);

	u32 GetSubFileOffset(CFile& file,u16 Id,u32* getlen=0);

	u16 Crc16(void *buf,u32 length);
}