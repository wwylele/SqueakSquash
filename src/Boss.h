#pragma once

struct BOSS_SCRIPT_DESC
{
	u8 class_id;
	u32 data_len;
};

const BOSS_SCRIPT_DESC& GetScriptDesc(u8 class_id);