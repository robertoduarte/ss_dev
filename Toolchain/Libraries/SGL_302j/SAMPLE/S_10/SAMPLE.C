#include	"sgl.h"

extern PDATA PD_CUBE;

#define		POS_X			toFIXED(0.0)
#define		POS_X_UP		200.0 
#define		POS_Y			toFIXED(20.0)
#define		POS_Z			toFIXED(270.0)
#define		POS_Z_UP		50.0
#define		SET_COUNT		500

static void init_cube2(EVENT*);

typedef struct cube {
	FIXED	pos[XYZ];
	ANGLE	ang[XYZ];
	ANGLE	angx, angz;
	ANGLE	angx_up , angz_up;
	PDATA	*poly;
	Sint16	set_count , set_i;
	Sint16	unset_count , unset_i;
} CUBE;

static void disp_cube(EVENT *evptr)
{
	CUBE	 *cubeptr;

	cubeptr = (CUBE *)evptr->user;
	slPushMatrix();
	{
		slTranslate(cubeptr->pos[X] , cubeptr->pos[Y] , cubeptr->pos[Z]);
		cubeptr->pos[X] = POS_X + POS_X_UP * slSin(cubeptr->angx);
		cubeptr->pos[Y] = cubeptr->pos[Y];
		cubeptr->pos[Z] = POS_Z + POS_Z_UP * slCos(cubeptr->angz);
		cubeptr->angx += cubeptr->angx_up;
		cubeptr->angz += cubeptr->angz_up;
		slRotY(cubeptr->ang[Y]); 
		slRotX(cubeptr->ang[X]); 
		slRotZ(cubeptr->ang[Z]); 
		cubeptr->ang[X] += DEGtoANG(5.0);
		cubeptr->ang[Y] += DEGtoANG(5.0);
		cubeptr->ang[Z] += DEGtoANG(5.0);
		slPutPolygon(cubeptr->poly);
	}
	slPopMatrix();
	cubeptr->set_count -= cubeptr->set_i;
	if(cubeptr->set_count < 0 ) {
		slSetEvent( (void *)init_cube2 );
		cubeptr->set_count = SET_COUNT;
	}
	cubeptr->unset_count -= cubeptr->unset_i;
	if(cubeptr->unset_count < 0 ) slCloseEvent(evptr);
}

static void init_cube1(EVENT *evptr)
{
	CUBE	 *cubeptr;

	cubeptr = (CUBE *)evptr->user;
	cubeptr->pos[X] = POS_X;
	cubeptr->pos[Y] = POS_Y;
	cubeptr->pos[Z] = POS_Z;
	cubeptr->ang[X] = cubeptr->ang[Y] = cubeptr->ang[Z] = DEGtoANG(0.0);
	cubeptr->angx = cubeptr->angz = DEGtoANG( 0.0);
	cubeptr->angx_up = cubeptr->angz_up = DEGtoANG( 0.0);
	cubeptr->set_count = SET_COUNT;
	cubeptr->set_i = 1;
	cubeptr->unset_count = 0;
	cubeptr->unset_i = 0;
	cubeptr->poly = &PD_CUBE;
	evptr->exad = (void *)disp_cube;
	disp_cube(evptr);
}

static void init_cube2(EVENT *evptr)
{
	CUBE	 *cubeptr;

	cubeptr = (CUBE *)evptr->user;
	cubeptr->pos[X] = POS_X;
	cubeptr->pos[Y] = POS_Y - toFIXED(50);
	cubeptr->pos[Z] = POS_Z + POS_Z_UP;
	cubeptr->ang[X] = cubeptr->ang[Y] = cubeptr->ang[Z] = DEGtoANG(0.0);
	cubeptr->angx = cubeptr->angz = DEGtoANG( 0.0);
	cubeptr->angx_up = cubeptr->angz_up = DEGtoANG( 3.0) * (-1);
	cubeptr->set_count = 0;
	cubeptr->set_i = 0;
	cubeptr->unset_count = SET_COUNT / 2 ;
	cubeptr->unset_i = 1;
	cubeptr->poly = &PD_CUBE;
	evptr->exad = (void *)disp_cube;
	disp_cube(evptr);
}
 
static void init_cube3(EVENT *evptr)
{
	CUBE	 *cubeptr;

	cubeptr = (CUBE *)evptr->user;
	cubeptr->pos[X] = POS_X;
	cubeptr->pos[Y] = POS_Y + toFIXED(50);
	cubeptr->pos[Z] = POS_Z + POS_Z_UP;
	cubeptr->ang[X] = cubeptr->ang[Y] = cubeptr->ang[Z] = DEGtoANG(0.0);
	cubeptr->angx = cubeptr->angz = DEGtoANG( 0.0);
	cubeptr->angx_up = cubeptr->angz_up = DEGtoANG( 3.0);
	cubeptr->set_count = 0;
	cubeptr->set_i = 0;
	cubeptr->unset_count = 0;
	cubeptr->unset_i = 0;
	cubeptr->poly = &PD_CUBE;
	evptr->exad = (void *)disp_cube;
	disp_cube(evptr);
}
 
void *event_tbl[] = {
	init_cube1 ,
	init_cube2 ,
	init_cube3
};

void set_event()
{
	EVENT *evptr;
	void	**exptr;
	Uint16 cnt;

	slInitEvent();
	for(exptr = event_tbl , cnt = sizeof(event_tbl) / sizeof(void *); cnt-- > 0;) {
		evptr = slSetEvent(*exptr++);
	}
}
