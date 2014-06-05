#pragma once

class SqMa;
class SqDe;
struct MaDeComm
{
	SqMa *pMa;
	SqDe *pDe;
};

enum MaDeCommNOTIFY
{
	N_DELETE,
	N_DOWN
};