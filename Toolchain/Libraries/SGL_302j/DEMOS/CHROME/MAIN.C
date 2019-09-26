//------------------------------------------------------------------------
//
//	X
//
//	CHROME TEST
//
//	CONFIDENTIAL
//	Copyright (c) 1996, Sega Technical Institute
//
//	AUTHOR
//  CHRIS COFFIN
//
//	TARGET
//	SEGA SATURN
//
//	REVISION 
//	00/00/00
//
//	ORIGINAL FILE 
//	s:\sgl_3\sgl\sample\chrome\main.c
//
//------------------------------------------------------------------------



#include	"sgl.h"
#include	"sega_sys.h"



// EXTERNAL TEXTURE DEFS  (texture.c)
extern	TEXTURE tex_table;

#define		GRreal(n)		(0x1c38+(n))
#define		GRaddr			0x1c38
#define		GRTBL(r,g,b)	(((b&0x1f)<<10) | ((g&0x1f)<<5) | (r&0x1f) )



extern		XPDATA			xpdata_test;
extern		GOURAUDTBL		grreal_test;
static		GOURAUDTBL		gour[1024];
static		Uint8			vwork[1024];

extern		Uint16			TotalPolygons;
extern		Uint16			TotalVertices;
extern		Uint16			DispPolygons;


static	Uint16			GourTbl[32] = {
	GRTBL(  0, 16, 16 ),
	GRTBL(  1, 16, 16 ),
	GRTBL(  2, 16, 16 ),
	GRTBL(  3, 16, 16 ),
	GRTBL(  4, 16, 16 ),
	GRTBL(  5, 16, 16 ),
	GRTBL(  6, 16, 16 ),
	GRTBL(  7, 16, 16 ),
	GRTBL(  8, 16, 16 ),
	GRTBL(  9, 16, 16 ),
	GRTBL( 10, 16, 16 ),
	GRTBL( 11, 16, 16 ),
	GRTBL( 12, 16, 16 ),
	GRTBL( 13, 16, 16 ),
	GRTBL( 14, 16, 16 ),
	GRTBL( 15, 16, 16 ),
	GRTBL( 16, 16, 16 ),						  
	GRTBL( 17, 16, 16 ),
	GRTBL( 18, 16, 16 ),
	GRTBL( 19, 16, 16 ),
	GRTBL( 20, 16, 16 ),
	GRTBL( 21, 16, 16 ),
	GRTBL( 22, 16, 16 ),
	GRTBL( 23, 16, 16 ),
	GRTBL( 24, 16, 16 ),
	GRTBL( 25, 16, 16 ),
	GRTBL( 26, 16, 16 ),
	GRTBL( 27, 16, 16 ),
	GRTBL( 28, 16, 16 ),
	GRTBL( 29, 16, 16 ),
	GRTBL( 30, 16, 16 ),
	GRTBL( 31, 16, 16 ),
};

static	Uint16	palette[32] = {
	0xA0A5,
	0x9043,
	0x8846,
	0x8868,
	0x90AC,
	0x9532,
	0xA593,
	0xC656,
	0x90AC,
	0x8084,
	0x8042,
	0x8000,
	0x8042,
	0x9043,
	0x8042,
	0x8000,
	0x8000,
	0x8000,
	0x8044,
	0x8886,
	0x9CA8,
	0x8886,
	0x8886,
	0x8000,
	0x9043,
	0x9043,
	0x9884,
	0x9884,
	0xA0A5,
	0xB54A,
	0xFB7B,
	0xC9AE,
};

void ss_main(void)
{

	ANGLE	t_angX,t_angY,t_angZ;

	MATRIX mtrx;
	FIXED light[XYZ];
	FIXED cm_pos[XYZ] =
		{toFIXED(0.0), toFIXED(-1.0), toFIXED(-35.0) };
	FIXED cm_target[XYZ] =
		{toFIXED(0.0), toFIXED(0.0), toFIXED(0.0)};
	ANGLE cm_ang[XYZ] =
		{DEGtoANG(0.0), DEGtoANG(0.0), DEGtoANG(0.0) };
								   





	slInitSystem(TV_352x256,(TEXTURE*)&tex_table,1);	// INITIALIZE SGL
	Texture_Load();								// INITALIZE ALL TEXTURES (texture.c)


	slInitGouraud( (GOURAUDTBL *)grreal_test, 1024, GRreal(0),(Uint8 *) vwork );
	slIntFunction( slGouraudTblCopy );			// Register gouraud table copy function
	slZdspLevel(7);								// Set Z display level

	slSetGouraudTbl( (Uint16 *)GourTbl );					// Specify custom gouraud table
	slDMACopy( (void*)palette, (void*)0x25f00000, sizeof(palette) );

	slCurColor(15);								// specify text color
	slPrint("Chrome/Bump demo"	, slLocate(1,23));
	slPrint("SGL 3.0"			, slLocate(1,24));


	slPrint("V Count"			, slLocate(1,2));
	slPrint("Polys Processed"	, slLocate(1,3));
	slPrint("Polys Drawn"		, slLocate(1,4));
	slPrint("TotalVertices"		, slLocate(1,5));



	// LIGHT VECTOR IN SCENE
		light[X] = toFIXED(0.0);
        light[Y] = toFIXED(0.851);
        light[Z] = toFIXED(0.451);

	// SET LIGHT VECTOR IN SGL
	slLight(light);


	// MAIN LOOP

	while(-1)
	{

		// Get unit matrix and specify camera line of sight in world
		slUnitMatrix(CURRENT);
		slLookAt(cm_pos, cm_target, cm_ang[Z] );

		// SPIN OBJECT
		slRotZ(t_angZ);
		slRotY(t_angY);			   
		slRotX(t_angX);
								 
		t_angZ	+=	DEGtoANG(0.25);
		t_angY	+=	DEGtoANG(1.0);
		t_angX	+=	DEGtoANG(1.1);
	
		// DRAW OBJECT
		slPutPolygonX((XPDATA*)&xpdata_test,light);


		// DEBUG INFO
		slPrintHex(	slHex2Dec(TotalPolygons)	, slLocate(16,3));
		slPrintHex(	slHex2Dec(DispPolygons)		, slLocate(16,4));
		slPrintHex(	slHex2Dec(TotalVertices)	, slLocate(16,5));

		slPrintHex(	slHex2Dec(slGetVCount())	, slLocate(16,2));	// Get vertical count position

		
		slSynch();	// SYNC SGL

	}
}

