/*----------------------------------------------------------------------*/
/*	Double Cube Circle Action					*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"

#define		DISTANCE_R1		40
#define		DISTANCE_R2		40

extern PDATA PD_CUBE;

static void set_star(ANGLE ang[XYZ] , FIXED pos[XYZ])
{
	slTranslate(pos[X] , pos[Y] , pos[Z]);
	slRotX(ang[X]);
	slRotY(ang[Y]);
	slRotZ(ang[Z]);
}

void ss_main(void)
{
	static ANGLE	ang1[XYZ] , ang2[XYZ];
	static FIXED	pos1[XYZ] , pos2[XYZ];
	static ANGLE	tmp = DEGtoANG(0.0);

	slInitSystem(TV_320x224,NULL,1);
	slPrint("Sample program 5.2" , slLocate(6,2));

	ang1[X] = ang2[X] = DEGtoANG(30.0);
	ang1[Y] = ang2[Y] = DEGtoANG(45.0);
	ang1[Z] = ang2[Z] = DEGtoANG( 0.0);

	pos2[X] = toFIXED(DISTANCE_R2);
	pos2[Y] = toFIXED(0.0);
	pos2[Z] = toFIXED(0.0);
	while(-1){
		slUnitMatrix(CURRENT);

		slPushMatrix();
		{
			pos1[X] = DISTANCE_R1 * slSin(tmp);
			pos1[Y] = toFIXED(30.0);
			pos1[Z] = toFIXED(220.0) + DISTANCE_R1 * slCos(tmp);
			set_star(ang1 , pos1);
			slPutPolygon(&PD_CUBE);

			slPushMatrix();
			{
				set_star(ang2 , pos2);
				slPutPolygon(&PD_CUBE);
			}
			slPopMatrix();
		}
		slPopMatrix();

		ang1[Y] += DEGtoANG(1.0);
		ang2[Y] -= DEGtoANG(1.0);
		tmp += DEGtoANG(1.0);

		slSynch();
	}
}

