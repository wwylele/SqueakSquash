#include "StdAfx.h"
#include "NitroCompress.h"

u32 Nitro::UncompressRL( const u8 *srcp, u8 *destp )
{
    const u8 *pSrc  = srcp;
    u8       *pDst  = destp;
    u32      destCount =*(u32*)pSrc >> 8;
    
    pSrc += 4;
    while ( destCount > 0 )
    {
        u8  flags  = *pSrc++;
        s32 length = flags & 0x7f;
        if ( !(flags & 0x80) )
        {   
            length++;
            destCount -= length;
            do
            {
                *pDst++ = *pSrc++;
            } while ( --length > 0 );
        }
        else
        {
            u8 srcTmp;
            
            length    += 3;
            destCount -= length;
            srcTmp    = *pSrc++;
            do
            {
                *pDst++ =  srcTmp;
            } while ( --length > 0 );
        }
    }
	return pSrc-srcp;
}