struct DetDesc
{
	u32 code;
	TCHAR* name;
	u16 tex_x,tex_y;
	
};

struct BMP1b16x16
{
	u16 data[16];
	inline bool Get(u8 x,u8 y)
	{
		return (data[y]>>x)&1?true:false;
	}
	inline void Set(u8 x,u8 y,bool v)
	{
		u16 i=1<<x;
		if(v)data[y]|=i;
		else data[y]&=~i;
	}
};
struct BMP3216x16
{
	COLORREF data[16][16];
};

const DetDesc* GetDet0Desc(u32 code,BMP1b16x16** GetDetBmp=0);
const DetDesc* GetDet0Desc/*ByStockCoord*/(u8 stockx,u8 stocky);
u32 GetDet1CodeByStockCoord(u8 stockx,u8 stocky);

void InitDet0Bmp(CDC* pDC);
void InitDet1Bmp(CDC* pDC);
BMP3216x16* GetDet1Bmp(u32 code);
extern const DetDesc DetDesc0[];
extern BMP1b16x16 Det0Bmp[];
extern const u32 Det1List[];
extern BMP3216x16 Det1Bmp[];
void GetDet1Desc(u32 code,CString &str);