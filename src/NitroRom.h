namespace Nitro
{
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
		// 0x0060 - 0x04000 Reserved region for system use
		//
		//u8	reserved_B[16];	// Reserved for system (should substitute with 0)
		u32 reserved_B1;
		u32 reserved_B2;
		u32 title_offset;
		u32 reserved_B4;

		u8	reserved_C[4*1024-0x70];	// Reserved for system C
		u8	reserved_D[12*1024];	// Reserved for system D
		
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
	};	// 0x08

	u16 GetSubFileId(CFile& file,char* subfilename);

	u32 GetSubFileOffset(CFile& file,u16 Id);

}