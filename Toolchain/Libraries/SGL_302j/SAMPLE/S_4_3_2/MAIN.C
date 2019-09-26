/*----------------------------------------------------------------------*/
/*	Rotation of 1 Polygon [X & Y axis]				*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"

extern PDATA PD_PLANE1;

void ss_main(void)
{
	static ANGLE	ang[XYZ];
	static FIXED	pos[XYZ];

	slInitSystem(TV_320x224,NULL,1);
	slPrint("Sample program 4.3.2" , slLocate(9,2));

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
			ang[X] += DEGtoANG(4.0);
			ang[Y] += DEGtoANG(2.0);
			slPutPolygon(&PD_PLANE1);
		}
		slPopMatrix();

		slSynch();
	}
}
