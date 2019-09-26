#include	"sgl.h"
#include	"sega_sys.h"

#include	"sega3D.h"
#include	"scsp.h"


extern FIXED	scl_model_00[];
extern ANGLE	ang_model_00[];
extern FIXED	pos_model_00[];

extern FIXED	scl_model_0[];
extern ANGLE	ang_model_0[];
extern FIXED	pos_model_0[];

extern FIXED	scl_model_1[];
extern ANGLE	ang_model_1[];
extern FIXED	pos_model_1[];

extern FIXED	scl_model_2[];
extern ANGLE	ang_model_2[];
extern FIXED	pos_model_2[];

extern FIXED	scl_model_3[];
extern ANGLE	ang_model_3[];
extern FIXED	pos_model_3[];

extern FIXED	scl_model_4[];
extern ANGLE	ang_model_4[];
extern FIXED	pos_model_4[];

extern FIXED	scl_model_5[];
extern ANGLE	ang_model_5[];
extern FIXED	pos_model_5[];

extern FIXED	scl_model_6[];
extern ANGLE	ang_model_6[];
extern FIXED	pos_model_6[];

extern FIXED	scl_model_7[];
extern ANGLE	ang_model_7[];
extern FIXED	pos_model_7[];

extern FIXED	scl_model_8[];
extern ANGLE	ang_model_8[];
extern FIXED	pos_model_8[];

extern FIXED	scl_model_9[];
extern ANGLE	ang_model_9[];
extern FIXED	pos_model_9[];

extern FIXED	scl_model_10[];
extern ANGLE	ang_model_10[];
extern FIXED	pos_model_10[];

extern FIXED	scl_model_11[];
extern ANGLE	ang_model_11[];
extern FIXED	pos_model_11[];

extern FIXED	scl_model_12[];
extern ANGLE	ang_model_12[];
extern FIXED	pos_model_12[];

extern FIXED	scl_model_13[];
extern ANGLE	ang_model_13[];
extern FIXED	pos_model_13[];


extern short	opaStat;

void			whichAction(void *);
static short	doInit(void *);
static short	doJump(void);
static short	doRot(short, short);

static void		setAngLimit(ANGLE *, float, float);

void	whichAction(void *ptr)
{
	Sega3DPtr		segaPtr = (Sega3DPtr)ptr;
	short			h, v, button;

slPrint("Y angle", slLocate(6,4));
slPrintHex((ang_model_0[Y]), slLocate(6,5));

	get_pad(&h, &v, &button);

	if(!(button & PER_DGT_TX)
	|| !(button & PER_DGT_TY)
	|| !(button & PER_DGT_TZ))
		return;
	if(!(button & PER_DGT_TC))
		opaStat = 1;
	if(!(button & PER_DGT_ST))
		opaStat = -1;
	if(h | v)
		opaStat = 3;

	pos_model_0[X] += h * toFIXED(0.3);
	pos_model_0[Z] += v * toFIXED(0.3);

	switch(opaStat){
		case	-1:
				opaStat = doInit(ptr);
				break;
		case	0:
				break;
		case	1:
		case	2:
				opaStat = doJump();
				break;
		case	3:
				opaStat = doRot(h, v);
				break;
	}


}

static short	doInit(void *ptr)
{
	init_object(ptr);

	scl_model_0[X] = toFIXED(1.0);
	scl_model_0[Y] = toFIXED(1.0);
	scl_model_0[Z] = toFIXED(1.0);

	ang_model_0[X] = DEGtoANG(0.0);
	ang_model_0[Y] = DEGtoANG(0.0);
	ang_model_0[Z] = DEGtoANG(0.0);

	pos_model_0[X] = toFIXED(0.0);
	pos_model_0[Y] = toFIXED(0.0);
	pos_model_0[Z] = toFIXED(0.0);

	return(0);
}


#define	JUMPSPD	0.5
#define	JUMPACC	-0.0175
static short	doJump(void)
{
	static FIXED	spd = toFIXED(JUMPSPD),
					acc = toFIXED(JUMPACC);
	short	stat;

	if(pos_model_0[Y] < toFIXED(0.0)){
		pos_model_0[Y] = toFIXED(0.0);
		ang_model_7[X] = DEGtoANG(0.0);
		ang_model_9[X] = DEGtoANG(0.0);
		spd = toFIXED(JUMPSPD);
		acc = toFIXED(JUMPACC);
		return(0);
	}
	spd += acc;
	pos_model_0[Y] += spd;
	if(spd < toFIXED(0.0)){
		ang_model_7[X] += DEGtoANG(4.0);
		ang_model_9[X] += DEGtoANG(4.0);
		stat = 2;
	}
	else{
		ang_model_7[X] -= DEGtoANG(8.0);
		ang_model_9[X] -= DEGtoANG(8.0);
		stat = 1;
	}
	setAngLimit(&ang_model_7[X], -90.0, 0.0);
	setAngLimit(&ang_model_9[X], -90.0, 0.0);
	return(stat);
}


#define	ROTANG	5.0
static short	doRot(short h, short v)
{
	short	stat;

	if(h > 0){
		if((ang_model_0[Y] - DEGtoANG(90.0)) >= DEGtoANG(180.0))
			ang_model_0[Y] -= DEGtoANG(ROTANG);
		else
			ang_model_0[Y] += DEGtoANG(ROTANG);
	}
	if(h < 0){
		if((ang_model_0[Y] - DEGtoANG(90.0)) >= DEGtoANG(180.0))
			ang_model_0[Y] += DEGtoANG(ROTANG);
		else
			ang_model_0[Y] -= DEGtoANG(ROTANG);
	}

	if(h | v)
		return(3);
	return(0);
}



static void	setAngLimit(ANGLE *ang, float min, float max)
{
	if(*ang > DEGtoANG(max))
		*ang = DEGtoANG(max);
	else if(*ang < DEGtoANG(min))
		*ang = DEGtoANG(min);
}














