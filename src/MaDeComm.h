#pragma once

class SqMa;
class SqDe;
struct MaDeComm
{
	SqMa *pMa;
	SqDe *pDe;
};

enum MadeCommNOTIFY
{
	N_DELETE,
	N_UP,
	N_DOWN,
	N_NEW
};