/*----------------------------------------------------------------------*/
/*	Parallel Translation of 1 Polygon [X axis]			*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"

#define		POS_Z		50.0

extern PDATA PD_PLANE1;

void ss_main(void)
{
	static ANGLE	ang[XYZ];
	static FIXED	pos[XYZ];
	static ANGLE	tmp = DEGtoANG(0.0);

	slInitSystem(TV_320x224,NULL,1);
	slPrint("Sample program 4.3.3" , slLocate(9,2));

	ang[X] = ang[Y] = ang[Z] = DEGtoANG(0.0);
	pos[X] = toFIXED(  0.0);
	pos[Y] = toFIXED(  0.0);
	pos[Z] = toFIXED(220.0);

	while(-1){
		slPushMatrix();
		{
			slTranslate(pos[X] , pos[Y] , pos[Z]);
			slRotX(ang[X]);
			slRotY(ang[Y]);
			slRotZ(ang[Z]);
			tmp += DEGtoANG(5.0);
			pos[X] = slMulFX(toFIXED(POS_Z), slSin(tmp));
			slPutPolygon(&PD_PLANE1);
		}
		slPopMatrix();

		slSynch();
	}
}
