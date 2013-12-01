#include "stdafx.h"
#include "Boss.h"
const BOSS_SCRIPT_DESC BossScriptDesc[]=
{
	{1	,12},
	{2	,22},
	{3	,36},
	{5	,68},
	{6	,86},
	{7	,108},
	{8	,130},
	{9	,156},
	{10	,182},
	{11	,212},
	{12	,242},
	{13	,276},
	{15	,348},
	{16	,386},
	{17	,428},
	{18	,470},
	{19	,516},
	{20	,562},
	{21	,612},
	{22	,662},
	{23	,716},
	{27	,948},
	{28	,1010},
	{29	,1076},
	{31	,1212},
	{32	,1282},
	{33	,1356},
	{37	,1668},
	{40	,1922},
	{48	,2690},
	{57	,3708},
	{0xFF,0}
};

const BOSS_SCRIPT_DESC& GetScriptDesc(u8 class_id)
{
	for(int i=0;;++i)
	{
		if(BossScriptDesc[i].class_id==class_id ||
			BossScriptDesc[i].class_id==0xFF)return BossScriptDesc[i];
	}
}