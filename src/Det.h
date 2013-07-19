struct DetDesc
{
	u32 code;
	TCHAR* name;
	u16 tex_x,tex_y;
	
};
const DetDesc* GetDet0Desc(u32 code);