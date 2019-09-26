/*----------------------------------------------------------------------*/
/*	Expansion & Reduction of 1 Polygon [X & Y axis]			*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"

extern PDATA PD_PLANE1;

void ss_main(void)
{
	static ANGLE	ang[XYZ];
	static FIXED	pos[XYZ];
	static FIXED	sclx, scly, sclz, tmp = toFIXED(0.0);
	static Sint16	flag = 1;

	slInitSystem(TV_320x224,NULL,1);
	slPrint("Sample program 4.3.4" , slLocate(9,2));

	ang[X] = ang[Y] = ang[Z] = DEGtoANG(0.0);
	pos[X] = toFIXED(  0.0);
	pos[Y] = toFIXED(  0.0);
	pos[Z] = toFIXED(220.0);
	sclx = scly = sclz = toFIXED(1.0);

	while(-1){
		slPushMatrix();
		{
			slTranslate(pos[X] , pos[Y] , pos[Z]);
			slRotX(ang[X]);
			slRotY(ang[Y]);
			slRotZ(ang[Z]);
			if (flag == 1) tmp += toFIXED(0.1);
			else tmp -= toFIXED(0.1);
			if (tmp > toFIXED( 1.0)) flag = 0;
			if (tmp < toFIXED(-1.0)) flag = 1;
			slScale(sclx + tmp , scly - tmp , sclz);
			slPutPolygon(&PD_PLANE1);
		}
		slPopMatrix();

		slSynch();
	}
}
