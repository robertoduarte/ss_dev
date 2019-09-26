/*----------------------------------------------------------------------*/
/*	Rotation of Single Plane Polygon				*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"

extern	PDATA PD_PLANE;

void main()
{
	static ANGLE	ang[XYZ];
	static FIXED	pos[XYZ];

	slInitSystem(TV_320x224,NULL,1);
/*	slPrint("Sample program 7.2" , slLocate(6,2));	*/

	ang[X] = ang[Y] = ang[Z] = DEGtoANG(0.0);
	pos[X] = toFIXED( 0.0);
	pos[Y] = toFIXED( 0.0);
	pos[Z] = toFIXED(170.0);

	while(-1){

		slPushMatrix();
		{
			slTranslate(pos[X] , pos[Y] , pos[Z]);
			slRotX(ang[X]);
			slRotY(ang[Y]);
			slRotZ(ang[Z]);
			ang[Y] += DEGtoANG(2.0);
			slPutPolygon(&PD_PLANE);
		}
		slPopMatrix();

		slSynch();
	}
}
