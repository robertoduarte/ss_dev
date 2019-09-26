/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"

#define	GRaddr	0xe000

#define	BACK_COL_ADR	( VDP2_VRAM_A1 + 0x1ffe )

extern XPDATA torus,torus2,torus3;

static	GOURAUDTBL	gour[512*3];
static	Uint8	gggg[512];

void ss_main(void)
{
	static ANGLE ang1[XYZ];
	static FIXED pos1[XYZ], light[XYZ];
	Uint16 data;
	Uint32	r,g,b;
	

	slInitSystem(TV_352x224, NULL, 1);
	slPrint("torus", slLocate(2,2));
	slZdspLevel(7);
	
	slInitGouraud( gour,512*3, GRaddr, gggg );

	slSetAmbient(C_RGB(5,5,5));
	
	ang1[X] = ang1[Y] = ang1[Z] = DEGtoANG(0.0);
	pos1[X] = toFIXED(  0.0);
	pos1[Y] = toFIXED(  0.0);
	pos1[Z] = toFIXED(40.0);
	light[X] = toFIXED(0.57735);
	light[Y] = toFIXED(0.57735);
	light[Z] = toFIXED(0.57735);

	while(-1){
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

		slGouraudTblCopy();
		
		r = (slSin( ang1[X] )+toFIXED(1.0)) >> 12;
		g = (slSin( ang1[Y] )+toFIXED(1.0)) >> 12;
		b = (slSin( ang1[Z] )+toFIXED(1.0)) >> 12;
		slSetGouraudColor( C_RGB( r,g,b ) );
		
		
		data = Smpc_Peripheral[0].data;

		if( data & PER_DGT_TR ) pos1[Z] += toFIXED(0.5);
		if( data & PER_DGT_TL ) pos1[Z] -= toFIXED(0.5);
		if( data & PER_DGT_ST ) {
			ang1[X] += DEGtoANG(2.2);
			ang1[Y] += DEGtoANG(3.4);
			ang1[Z] += DEGtoANG(1.8);
		}

		slSynch();
	}
}
