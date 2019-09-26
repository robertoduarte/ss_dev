#include	"sgl.h"
#include	"sega_sys.h"

#define		X_POS_INIT		toFIXED(0.0)
#define		Y_POS_INIT		toFIXED(0.0)
#define		Z_POS_INIT		toFIXED(0.0)
#define		X_ANG_INIT		DEGtoANG(0.0)
#define		Y_ANG_INIT		DEGtoANG(0.0)
#define		Z_ANG_INIT		DEGtoANG(0.0)
#define		SCL_INIT		toFIXED(1.0)
#define		X_POS_UP		toFIXED(2.0)
#define		Y_POS_UP		toFIXED(2.0)
#define		Z_POS_UP		toFIXED(2.0)
#define		X_ANG_UP		DEGtoANG(2.0)
#define		Y_ANG_UP		DEGtoANG(2.0)
#define		Z_ANG_UP		DEGtoANG(2.0)
#define		X_ANG_DOWN		DEGtoANG(-2.0)
#define		Y_ANG_DOWN		DEGtoANG(-2.0)
#define		Z_ANG_DOWN		DEGtoANG(-2.0)
#define		SCL_UP			toFIXED(0.01)

extern MATRIX poly_mt;

void get_pad2(FIXED pos[XYZ] , ANGLE ang[XYZ] , FIXED scl[XYZ] , ANGLE kang[XYZ])
{
	Uint16 data;

	if(!Per_Connect1) return;
	data = Smpc_Peripheral[0].data;

	kang[X] = kang[Y] = kang[Z] = DEGtoANG(0.0);

	/* Translation */
	if((data & PER_DGT_TA) == 0){
		if((data & PER_DGT_KR) == 0) pos[X] += X_POS_UP;
		if((data & PER_DGT_KL) == 0) pos[X] -= X_POS_UP;
		if((data & PER_DGT_KU) == 0) pos[Y] -= Y_POS_UP;
		if((data & PER_DGT_KD) == 0) pos[Y] += Y_POS_UP;
		if((data & PER_DGT_TR) == 0) pos[Z] -= Z_POS_UP;
		if((data & PER_DGT_TL) == 0) pos[Z] += Z_POS_UP;
	}

	/* Rotation */
	if((data & PER_DGT_TB) == 0){
		if((data & PER_DGT_KU) == 0) kang[X] = X_ANG_UP;
		if((data & PER_DGT_KD) == 0) kang[X] = X_ANG_DOWN;
		if((data & PER_DGT_KR) == 0) kang[Y] = Y_ANG_UP;
		if((data & PER_DGT_KL) == 0) kang[Y] = Y_ANG_DOWN;
		if((data & PER_DGT_TR) == 0) kang[Z] = Z_ANG_DOWN;
		if((data & PER_DGT_TL) == 0) kang[Z] = Z_ANG_UP;
	}

	/* Scaling */
	if((data & PER_DGT_TC) == 0){
		if((data & PER_DGT_KU) == 0){
			scl[X] += SCL_UP;
			scl[Y] += SCL_UP;
			scl[Z] += SCL_UP;
		}
		if((data & PER_DGT_KD ) == 0){
			scl[X] -= SCL_UP;
			scl[Y] -= SCL_UP;
			scl[Z] -= SCL_UP;
		}
	}

	/* Initialization */
	if((data & PER_DGT_TX) == 0){
		pos[X] = X_POS_INIT;
		pos[Y] = Y_POS_INIT;
		pos[Z] = Z_POS_INIT;
	}
	if((data & PER_DGT_TY) == 0){
		ang[X] = X_ANG_INIT;
		ang[Y] = Y_ANG_INIT;
		ang[Z] = Z_ANG_INIT;
		slUnitMatrix(poly_mt);
	}
	if((data & PER_DGT_TZ) == 0){
		scl[X] = scl[Y] = scl[Z] = SCL_INIT;
	}
	if((data & PER_DGT_ST) == 0){
		slUnitMatrix(poly_mt);
		pos[X] = X_POS_INIT;
		pos[Y] = Y_POS_INIT;
		pos[Z] = Z_POS_INIT;
		ang[X] = X_ANG_INIT;
		ang[Y] = Y_ANG_INIT;
		ang[Z] = Z_ANG_INIT;
		scl[X] = scl[Y] = scl[Z] = SCL_INIT;
	}
}

