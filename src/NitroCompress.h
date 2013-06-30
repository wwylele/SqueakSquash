#pragma once

namespace Nitro{
	u32 UncompressRL( const u8 *srcp, u8 *destp );
	u32 CompressRL(const u8 *srcp, u32 size, u8 *dstp);
}