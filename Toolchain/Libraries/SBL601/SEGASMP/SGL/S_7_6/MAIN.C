/*----------------------------------------------------------------------*/
/*	Gouraud Shading							*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"

extern PDATA PD_CUBE;

#define		GRoffsetTBL(r,g,b)	(((b & 0x1f) << 10) | ((g & 0x1f) << 5) | (r & 0x1f))
#define		VRAMaddr		(SpriteVRAM+0x70000)

static Uint16 GRdata[6][4] = {
	{ GRoffsetTBL(  0,-16,-16) , GRoffsetTBL(  0,-16,-16) ,
	  GRoffsetTBL(  0,-16,-16) , GRoffsetTBL(-16, 15,  0) } , 
	{ GRoffsetTBL(  0,-16,-16) , GRoffsetTBL(  0,-16,-16) ,
	  GRoffsetTBL(-16, 15,  0) , GRoffsetTBL(  0,-16,-16) } , 
	{ GRoffsetTBL(-16, 15,  0) , GRoffsetTBL(  0,-16,-16) ,
	  GRoffsetTBL(  0,-16,-16) , GRoffsetTBL(  0,-16,-16) } , 
	{ GRoffsetTBL(  0,-16,-16) , GRoffsetTBL(-16, 15,  0) ,
	  GRoffsetTBL(  0,-16,-16) , GRoffsetTBL(  0,-16,-16) } , 
	{ GRoffsetTBL(  0,-16,-16) , GRoffsetTBL(-16, 15,  0) ,
	  GRoffsetTBL(  0,-16,-16) , GRoffsetTBL(  0,-16,-16) } , 
	{ GRoffsetTBL(-16, 15,  0) , GRoffsetTBL(  0,-16,-16) , 
	  GRoffsetTBL(  0,-16,-16) , GRoffsetTBL(  0,-16,-16) } 
};

void main()
{
	static ANGLE	ang[XYZ];
	static FIXED	pos[XYZ];

	slInitSystem(TV_320x224,NULL,1);
/*	slPrint("Sample program 7.6" , slLocate(9,2));	*/

	ang[X] = DEGtoANG(30.0);
	ang[Y] = DEGtoANG( 0.0);
	ang[Z] = DEGtoANG( 0.0);
	pos[X] = toFIXED( 0.0);
	pos[Y] = toFIXED( 0.0);
	pos[Z] = toFIXED(200.0);

	slDMACopy(GRdata,(void *)VRAMaddr,sizeof(GRdata));

	while(-1){
		slPushMatrix();
		{
			slTranslate(pos[X] , pos[Y] , pos[Z]);
			slRotX(ang[X]);
			slRotY(ang[Y]);
			slRotZ(ang[Z]);
			slPutPolygon(&PD_CUBE);
		}
		slPopMatrix();

		ang[Y] += DEGtoANG(1.0);

		slSynch();
	}
}

