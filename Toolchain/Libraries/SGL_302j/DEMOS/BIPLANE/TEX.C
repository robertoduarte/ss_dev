#include	"sgl.h"

#include	"./map/tex/miti00.c"
#include	"./map/tex/iwa.c"
#include	"./map/tex/kusa00.c"
#include	"./map/tex/kusa01.c"
#include	"./map/tex/kusa02.c"
#include	"./map/tex/ton00.c"
#include	"./map/tex/miti01.c"

enum	cga	{
	MITI00	=CGADDRESS,
	IWA00	=AdjCG(MITI00,32,32,COL_32K),
	KUSA00	=AdjCG(IWA00,32,32,COL_32K),
	KUSA01	=AdjCG(KUSA00,32,32,COL_32K),
	KUSA02	=AdjCG(KUSA01,32,32,COL_32K),
	TON00	=AdjCG(KUSA02,32,32,COL_32K),
	MITI01	=AdjCG(TON00,32,32,COL_32K),
};
TEXTURE tex_sample[]={
	TEXDEF(32,32,MITI00),
	TEXDEF(32,32,IWA00),
	TEXDEF(32,32,KUSA00),
	TEXDEF(32,32,KUSA01),
	TEXDEF(32,32,KUSA02),
	TEXDEF(32,32,TON00),
	TEXDEF(32,32,MITI01),
};

PICTURE	pic_sample[]={
	PICDEF( 0,COL_32K,miti00_32X32),
	PICDEF( 1,COL_32K,iwa_32X32),
	PICDEF( 2,COL_32K,kusa00_32X32),
	PICDEF( 3,COL_32K,kusa01_32X32),
	PICDEF( 4,COL_32K,kusa02_32X32),
	PICDEF( 5,COL_32K,ton00_32X32),
	PICDEF( 6,COL_32K,miti01_32X32),
};

