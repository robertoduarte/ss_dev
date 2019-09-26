/*----------------------------------------------------------------------*/
/*	Camera Action							*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"

#define		POS_Z		100.0

typedef struct cam{
	FIXED	pos[XYZ];
	FIXED	target[XYZ];
	ANGLE	ang[XYZ];
} CAMERA;

extern PDATA PD_CUBE;

static FIXED cubepos[][3] = {
	POStoFIXED( 20 , 0 , 270) ,
	POStoFIXED(-70 , 0 , 270) ,
	POStoFIXED( 40 , 0 , 350) ,
	POStoFIXED(-60 , 0 , 370)
};

void dispCube(FIXED pos[XYZ])
{
	slPushMatrix();
	{
		slTranslate(pos[X] , pos[Y] , pos[Z]);
		slPutPolygon(&PD_CUBE);
	}
	slPopMatrix();
}

void main()
{
	static ANGLE	ang[XYZ];
	static CAMERA	cmbuf;
	static ANGLE	tmp = DEGtoANG(0.0);

	slInitSystem(TV_320x224,NULL,1);
/*	slPrint("Sample program 6.3" , slLocate(9,2));	*/

	cmbuf.ang[X] = cmbuf.ang[Y] = cmbuf.ang[Z] = DEGtoANG(0.0);
	cmbuf.target[X] = cmbuf.target[Y] = toFIXED(0.0);
	cmbuf.target[Z] = toFIXED(320.0);
	cmbuf.pos[X] = toFIXED(  0.0);
	cmbuf.pos[Y] = toFIXED(-20.0);
	cmbuf.pos[Z] = toFIXED(  0.0);

	while(-1){
		slUnitMatrix(CURRENT);
		slLookAt(cmbuf.pos , cmbuf.target , cmbuf.ang[Z]);
		tmp += DEGtoANG(2.0);
		cmbuf.pos[X] = POS_Z * slCos(tmp);
		cmbuf.pos[Z] = POS_Z * slSin(tmp);

		dispCube(cubepos[0]);
		dispCube(cubepos[1]);
		dispCube(cubepos[2]);
		dispCube(cubepos[3]);

		slSynch();
	}
}

