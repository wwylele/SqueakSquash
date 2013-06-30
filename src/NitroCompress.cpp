#include "StdAfx.h"
#include "NitroCompress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
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
u32 Nitro::CompressRL(const u8 *srcp, u32 size, u8 *dstp)
{
    u32 RLDstCount;                    // Number of bytes of compressed data
    u32 RLSrcCount;                    // Processed data volume of the compression target data (in bytes)
    u8  RLCompFlag;                    // 1 if performing run length encoding
    u8  runLength;                     // Run length
    u8  rawDataLength;                 // Length of data not run
    u32 i;
    
    const u8 *startp;                  // Point to the start of compression target data for each process loop
    
    ASSERT( srcp != NULL );
    ASSERT( dstp != NULL );
    ASSERT( size > 4     );
    
    //  Data header (The size after decompression)
    // Because the same output data as for Nitro is created, an endian strategy is implemented.
    *(u32 *)dstp = size << 8 | 0x30;       // data header
    RLDstCount = 4;

    RLSrcCount = 0;
    rawDataLength = 0;
    RLCompFlag = 0;

    while (RLSrcCount < size)
    {
        startp = &srcp[RLSrcCount];    // Set compression target data

        for (i = 0; i < 128; i++)      // Data volume that can be expressed in 7 bits is 0 to 127
        {
            // Reach the end of the compression target data
            if (RLSrcCount + rawDataLength >= size)
            {
                rawDataLength = (u8)(size - RLSrcCount);
                break;
            }

            if (RLSrcCount + rawDataLength + 2 < size)
            {
                if (startp[i] == startp[i + 1] && startp[i] == startp[i + 2])
                {
                    RLCompFlag = 1;
                    break;
                }
            }
            rawDataLength++;
        }

        // Store data that is not encoded
        // If the 8th bit of the data length storage byte is 0, the data series that is not encoded.
        // The data length is x - 1, so 0-127 becomes 1-128.
        if (rawDataLength)
        {
            if (RLDstCount + rawDataLength + 1 >= size) // Quit on error if size becomes larger than source
            {
                return 0;
            }
            dstp[RLDstCount++] = (u8)(rawDataLength - 1);       // Store "data length - 1" (7 bits)
            for (i = 0; i < rawDataLength; i++)
            {
                dstp[RLDstCount++] = srcp[RLSrcCount++];
            }
            rawDataLength = 0;
        }

        // Run Length Encoding
        if (RLCompFlag)
        {
            runLength = 3;
            for (i = 3; i < 128 + 2; i++)
            {
                // Reach the end of the data for compression
                if (RLSrcCount + runLength >= size)
                {
                    runLength = (u8)(size - RLSrcCount);
                    break;
                }

                // If run is interrupted
                if (srcp[RLSrcCount] != srcp[RLSrcCount + runLength])
                {
                    break;
                }
                // Run continues
                runLength++;
            }

            // If the 8th bit of the data length storage byte is 1, the data series that is encoded.
            if (RLDstCount + 2 >= size) // Quit on error if size becomes larger than source
            {
                return 0;
            }
            dstp[RLDstCount++] = (u8)(0x80 | (runLength - 3));  // Add 3, and store 3 to 130
            dstp[RLDstCount++] = srcp[RLSrcCount];
            RLSrcCount += runLength;
            RLCompFlag = 0;
        }
    }

    // Align to 4-byte boundary
    //   Does not include Data0 used for alignment as data size
    i = 0;
    while ((RLDstCount + i) & 0x3)
    {
        dstp[RLDstCount + i] = 0;
        i++;
    }
    return RLDstCount;
}
