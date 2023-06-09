/*----------------------------------------------------------------------*/
/*	Cube & A Light Source						*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"

extern PDATA PD_PLANE1;

void main()
{
	static ANGLE	ang[XYZ];
	static FIXED	pos[XYZ];
	static FIXED	light[XYZ];
	static ANGLE	tmp = DEGtoANG(0.0);

	slInitSystem(TV_320x224,NULL,1);

	ang[X] = DEGtoANG(30.0);
	ang[Y] = DEGtoANG(45.0);
	ang[Z] = DEGtoANG( 0.0);
	pos[X] = toFIXED(  0.0);
	pos[Y] = toFIXED(  0.0);
	pos[Z] = toFIXED(190.0);

	light[X] = toFIXED(-1.0);
	light[Y] = toFIXED( 0.0);
	light[Z] = toFIXED( 0.0);

/*	slPrint("Sample program 3.2" , slLocate(9,2));	*/

	while(-1){
		slPushMatrix();
		{
			slRotY(tmp);
			slRotX(DEGtoANG(15.0));
			slRotZ(DEGtoANG(15.0));
			slCalcPoint(toFIXED(0.0),toFIXED(0.0),toFIXED(1.0),light);
		}
		slPopMatrix();
		slLight(light);
		tmp += DEGtoANG(1.0);

		slPushMatrix();
		{
			slTranslate(pos[X] , pos[Y] , pos[Z]);
			slRotX(ang[X]);
			slRotY(ang[Y]);
			slRotZ(ang[Z]);
			slPutPolygon(&PD_PLANE1);
		}
		slPopMatrix();

		slSynch();
	}
}
