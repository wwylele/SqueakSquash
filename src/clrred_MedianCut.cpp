#include "stdafx.h"
#include <list>
#include "Nitro.h"

class RGBVec
{
public:
	u8 r,g,b;
	RGBVec():r(0),g(0),b(0){}
	RGBVec(u8 _r,u8 _g,u8 _b):r(_r),g(_g),b(_b){}
	RGBVec(COLORREF rgb):r(GetRValue(rgb)),g(GetGValue(rgb)),b(GetBValue(rgb)){}
	static enum CMPMODE{CMPMODE_R,CMPMODE_G,CMPMODE_B}CmpMode;
	u8 GetCmpValue()const
	{
		switch(CmpMode)
		{
		case CMPMODE_R:return r;
		case CMPMODE_G:return g;
		case CMPMODE_B:return b;
		default:return r;
		}
	}
	inline bool operator > (const RGBVec& c)const{return GetCmpValue()> c.GetCmpValue();}
	inline bool operator < (const RGBVec& c)const{return GetCmpValue()< c.GetCmpValue();}
	inline bool operator >=(const RGBVec& c)const{return GetCmpValue()>=c.GetCmpValue();}
	inline bool operator <=(const RGBVec& c)const{return GetCmpValue()<=c.GetCmpValue();}
	inline bool operator ==(const RGBVec& c)const{return GetCmpValue()==c.GetCmpValue();}
	inline bool operator !=(const RGBVec& c)const{return GetCmpValue()!=c.GetCmpValue();}
	u32 GetDistanceSquare(const RGBVec& c)const
	{
		return(r*c.r+g*c.g+b*c.b);
	}
};
RGBVec::CMPMODE RGBVec::CmpMode(CMPMODE_R);
typedef std::list<RGBVec> rvlist;
void clrred_MedianCut(
	rvlist &ColorIn,
	RGBVec ColorOut[/*256*/]//ColorOut[0]不输出，给背景色预留
	)
{
	rvlist::iterator iter,iter2;
	if(ColorIn.size()<256)
	{
		u8 i=1;
		for(iter=ColorIn.begin();iter!=ColorIn.end();iter++)
		{
			ColorOut[i]=*iter;
			++i;
		}
		return;
	}

	std::list<rvlist> rvlq,//任务序列
		rvlt;//新任务序列
	std::list<rvlist>::iterator iterq;
	rvlist rva,rvb;
	rvlq.push_front(ColorIn);//以主链作为第一个任务序列
	u8 rmax,rmin,gmax,gmin,bmax,bmin;
	for(int i=0;i<8;++i)//八轮任务，总计将链表从1份分割成2^8=256份
	{
		for(iterq=rvlq.begin();iterq!=rvlq.end();iterq++)
		{
			//搜索rgb的最值
			rmax=gmax=bmax=0;
			rmin=gmin=bmin=255;
			for(iter=iterq->begin();iter!=iterq->end();iter++)
			{
				if(iter->r>rmax)rmax=iter->r;
				if(iter->g>gmax)gmax=iter->g;
				if(iter->b>rmax)bmax=iter->b;
				if(iter->r<rmin)rmin=iter->r;
				if(iter->g<gmin)gmin=iter->g;
				if(iter->b<rmin)bmin=iter->b;
			}
			//根据盒子尺寸选择排序方式
			if(rmax-rmin>gmax-gmin && 
				rmax-rmin>bmax-bmin)RGBVec::CmpMode=RGBVec::CMPMODE_R;
			else if(gmax-gmin>bmax-bmin)RGBVec::CmpMode=RGBVec::CMPMODE_G;
			else RGBVec::CmpMode=RGBVec::CMPMODE_B;

			//排序
			iterq->sort();

			//从中央分割成两个链表rva,rvb
			iter=iterq->begin();
			for(u32 j=0;j<iterq->size()/2;++j)++iter;//得到中央元素的迭代器
			rva.insert(rva.begin(),iterq->begin(),iter);
			rvb.insert(rvb.begin(),iter,iterq->end());
			
			//将新链表塞入新任务序列
			rvlt.push_front(rva);
			rvlt.push_front(rvb);
			rva.clear();rvb.clear();
		}
		//将新任务序列替换原有序列
		rvlq=rvlt;
		rvlt.clear();
	}

	//吃掉第一个序列，将总数降为255
	iterq=rvlq.begin();
	iterq++;
	iterq->insert(iterq->begin(),rvlq.begin()->begin(),rvlq.begin()->end());
	rvlq.pop_front();

	ASSERT(rvlq.size()==255);

	//生成调色板
	u8 i=1;
	u32 rs,gs,bs;
	for(iterq=rvlq.begin();iterq!=rvlq.end();iterq++)
	{
		rs=gs=bs=0;
		for(iter=iterq->begin();iter!=iterq->end();iter++)
		{
			rs+=iter->r;
			gs+=iter->g;
			bs+=iter->b;
		}
		ColorOut[i].r=u8(rs/iterq->size());
		ColorOut[i].g=u8(gs/iterq->size());
		ColorOut[i].b=u8(bs/iterq->size());
		++i;
	}
}

void BmpToNitroPlt256(
	const COLORREF BmpIn[],
	u32 Size,
	COLORREF BackColor,
	u8 BmpOut[],
	Nitro::Color15 PltOut[/*256*/])
{
	RGBVec rgbt;
	rvlist rvl;

	u32* ColorMap=new u32[Size];
	
	//扫描颜色
	for(u32 i=0;i<Size;++i)
	{
		rgbt=RGBVec(BmpIn[i]);
		rvl.push_front(rgbt);
	}
	rvl.unique();

	//生成调色板
	RGBVec ColorOut[256];
	ColorOut[0]=RGBVec(BackColor);
	clrred_MedianCut(rvl,ColorOut);
	


	delete[]ColorMap;
}