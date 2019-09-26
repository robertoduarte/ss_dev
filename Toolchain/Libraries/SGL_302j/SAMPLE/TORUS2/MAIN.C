/********************************************************************
     < SGL Ver3.00 Sample Program >
  Sample Name : Torus (RealTime Gouraud & Color Light)
  Author      : T.A
  Date        : 09/10/1996
********************************************************************/

#include	"sgl.h"
#include	"sgl_cd.h"
#include	"sega_sys.h"
#define FLASH_SATURN_CD

#define	GRaddr	0xe000
#define	BACK_COL_ADR	( VDP2_VRAM_A1 + 0x1ffe )

#define	GRTBL(r,g,b)	(((b&0x1f)<<10) | ((g&0x1f)<<5) | (r&0x1f) )

static	Uint16	GourTbl[32] = {
	GRTBL(  0,  0,  0 ),
	GRTBL(  1,  1,  1 ),
	GRTBL(  2,  2,  2 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  4,  4,  4 ),
	GRTBL(  5,  5,  5 ),
	GRTBL(  6,  6,  6 ),
	GRTBL(  7,  7,  7 ),
	GRTBL(  8,  8,  8 ),
	GRTBL(  9,  9,  9 ),
	GRTBL( 10, 10, 10 ),
	GRTBL( 11, 11, 11 ),
	GRTBL( 12, 12, 12 ),
	GRTBL( 13, 13, 13 ),
	GRTBL( 14, 14, 14 ),
	GRTBL( 15, 15, 15 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 17, 17, 17 ),
	GRTBL( 18, 18, 18 ),
	GRTBL( 19, 19, 19 ),
	GRTBL( 20, 20, 20 ),
	GRTBL( 21, 21, 21 ),
	GRTBL( 22, 22, 22 ),
	GRTBL( 23, 23, 23 ),
	GRTBL( 24, 24, 24 ),
	GRTBL( 25, 25, 25 ),
	GRTBL( 26, 26, 26 ),
	GRTBL( 27, 27, 27 ),
	GRTBL( 28, 28, 28 ),
	GRTBL( 29, 29, 29 ),
	GRTBL( 30, 30, 30 ),
	GRTBL( 31, 31, 31 ),
};

static	Uint16	GourTbl2[32] = {
	GRTBL(  0,  0,  0 ),
	GRTBL(  1,  1,  1 ),
	GRTBL(  2,  2,  2 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  4,  4,  4 ),
	GRTBL(  5,  5,  5 ),
	GRTBL(  6,  6,  6 ),
	GRTBL(  7,  7,  7 ),
	GRTBL(  8,  8,  8 ),
	GRTBL(  9,  9,  9 ),
	GRTBL( 10, 10, 10 ),
	GRTBL( 11, 11, 11 ),
	GRTBL( 12, 12, 12 ),
	GRTBL( 13, 13, 13 ),
	GRTBL( 14, 14, 14 ),
	GRTBL( 15, 15, 15 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 17, 17, 17 ),
	GRTBL( 19, 19, 19 ),
	GRTBL( 22, 22, 22 ),
	GRTBL( 26, 26, 26 ),
	GRTBL( 31, 31, 31 ),
};

static	Uint16	GourTbl3[32] = {
	GRTBL(  3,  3,  3 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  4,  4,  4 ),
	GRTBL(  5,  5,  5 ),
	GRTBL(  6,  6,  6 ),
	GRTBL(  7,  7,  7 ),
	GRTBL(  8,  8,  8 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL( 13, 13, 13 ),
	GRTBL( 18, 18, 18 ),
	GRTBL( 24, 24, 24 ),
	GRTBL( 31, 31, 31 ),
};

extern XPDATA torus,torus2,torus3;

static	GOURAUDTBL	gour[512*3];
static	Uint8	gggg[512];

void ss_main(void)
{
	static ANGLE ang1[XYZ];
	static FIXED pos1[XYZ], light[XYZ],light2[XYZ];
	MATRIX mtrx;
	Uint16 data;
	Uint32	r,g,b;
	Uint32 flag=0;
	
	slInitSystem(TV_352x224, NULL, 1);
	slPrint("torus", slLocate(2,2));
	slZdspLevel(7);
	slBack1ColSet( (void*)BACK_COL_ADR, 0 );
	*(Uint16*)BACK_COL_ADR = C_RGB(0,0,0) | RGB_Flag;
	
	slInitGouraud( gour,512*3, GRaddr, gggg );
	slIntFunction( slGouraudTblCopy );

	ang1[X] = ang1[Y] = ang1[Z] = DEGtoANG(0.0);
	pos1[X] = toFIXED(  0.0);
	pos1[Y] = toFIXED(  0.0);
	pos1[Z] = toFIXED(40.0);
	light[X] = toFIXED(0.57735);
	light[Y] = toFIXED(0.57735);
	light[Z] = toFIXED(0.57735);

	while(-1){
#ifdef FLASH_SATURN_CD
		if ( CDC_GetHirqReq() & CDC_HIRQ_DCHG ) {
			SYS_Exit(1);
		}
#endif
		slLight(light);

		slPushMatrix();
		{
			slTranslate(pos1[X], pos1[Y], pos1[Z]);
			slRotX(ang1[X]);
			slRotY(ang1[Y]);
			slRotZ(ang1[Z]);
			slTranslate( 0, toFIXED(10.0), 0);
			slPutPolygonX(&torus,light);
		}
		slPopMatrix();

		slPushMatrix();
		{
			slTranslate(pos1[X], pos1[Y], pos1[Z]);
			slRotX(ang1[X]);
			slRotY(ang1[Y]);
			slRotZ(ang1[Z]);
			slTranslate( toFIXED(9.0), toFIXED(-7.0), 0);
			slRotZ(DEGtoANG(60.0));
			slPutPolygonX(&torus2,light);
		}
		slPopMatrix();

		slPushMatrix();
		{
			slTranslate(pos1[X], pos1[Y], pos1[Z]);
			slRotX(ang1[X]);
			slRotY(ang1[Y]);
			slRotZ(ang1[Z]);
			slTranslate( toFIXED(-9.0), toFIXED(-7.0), 0);
			slRotZ(DEGtoANG(-60.0));
			slPutPolygonX(&torus3,light);
		}
		slPopMatrix();

		if (flag) { /* First Pad Data No Check! */
			data = Smpc_Peripheral[0].data;

#ifdef FLASH_SATURN_CD
			if((data & (PER_DGT_ST | PER_DGT_TA | PER_DGT_TB | PER_DGT_TC)) == 0){
				SYS_Exit(0);
			}
#endif
			if( data & PER_DGT_TR ) pos1[Z] += toFIXED(0.5);
			if( data & PER_DGT_TL ) pos1[Z] -= toFIXED(0.5);

			if( !(data & PER_DGT_TX) ) slSetGouraudColor( C_RGB( 31,16,16 ) );
			if( !(data & PER_DGT_TY) ) slSetGouraudColor( C_RGB( 16,31,16 ) );
			if( !(data & PER_DGT_TZ) ) slSetGouraudColor( C_RGB( 16,16,31 ) );
			if( !(data & PER_DGT_TB) ) slSetGouraudTbl( GourTbl2 );
			if( !(data & PER_DGT_TC) ) slSetGouraudTbl( GourTbl3 );
			if( !(data & PER_DGT_TA) ) slSetGouraudTbl( GourTbl );
		
			if( data & PER_DGT_ST ) {
				ang1[X] += DEGtoANG(2.2);
				ang1[Y] += DEGtoANG(3.4);
				ang1[Z] += DEGtoANG(1.8);
			}
		} else {
			flag = 1;
		}
		slSynch();
	}
}
