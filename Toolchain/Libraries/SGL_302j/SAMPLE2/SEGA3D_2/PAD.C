#include	"sgl.h"
#include	"sega_sys.h"
#include	"sega3d.h"


extern void	init_camera(void *);
extern void	init_object(void *);

typedef struct{
	FIXED	cpos[XYZ];
	FIXED	ctarget[XYZ];
	ANGLE	cangle[XYZ];
	FIXED	pos[XYZ];
	ANGLE	ang[XYZ];
	FIXED	scl[XYZ];
}Sega3D, *Sega3DPtr;


void	get_pad(short *h, short *v, short *button)
{
	static Uint16	pad;
	static Sint32	i;

	if(!Per_Connect1) return;
	pad = Smpc_Peripheral[0].data;

	*h = *v = *button = 0;
	if((pad & PER_DGT_KU) == 0)
		*v = 1;
	if((pad & PER_DGT_KD) == 0)
		*v = -1;
	if((pad & PER_DGT_KL) == 0)
		*h = -1;
	if((pad & PER_DGT_KR) == 0)
		*h = 1;
	*button = Smpc_Peripheral[0].data & 0x0ff8;
}

void	chg_view(void *ptr)
{
	Sega3DPtr	segaPtr = (Sega3DPtr)ptr;
	short		h, v, button;

	get_pad(&h, &v, &button);

	if(!(button & PER_DGT_TX)){
		segaPtr->pos[X] += (h * toFIXED(1.0));
		segaPtr->ang[X] += (v * DEGtoANG(1.0));
	}
	if(!(button & PER_DGT_TY)){
		segaPtr->pos[Y] += (v * toFIXED(1.0));
		segaPtr->ang[Y] += (h * DEGtoANG(1.0));
	}
	if(!(button & PER_DGT_TZ)){
		segaPtr->pos[Z] += (v * toFIXED(1.0));
		segaPtr->ang[Z] += (h * DEGtoANG(1.0));
	}
	if(!(button & PER_DGT_ST)) {
		init_camera((void*)segaPtr);
		init_object((void*)segaPtr);
	}
#if 0
	if(!(button & PER_DGT_TX)){
		segaPtr->cpos[X] += toFIXED(h * 50);
		segaPtr->cpos[Y] += toFIXED(v * 50);
	}
	if(!(button & PER_DGT_TY)){
		segaPtr->cpos[X] += toFIXED(h * 50);
		segaPtr->cpos[Y] += toFIXED(v * 50);
	}
	if(!(button & PER_DGT_TZ)){
		segaPtr->cpos[Z] += toFIXED(v * 50);
		segaPtr->cangle[Z] += DEGtoANG(h);
	}

	if(!(button & PER_DGT_TA)){
		segaPtr->pos[X] += toFIXED(h * 50);
		segaPtr->pos[Y] -= toFIXED(v * 50);
	}
	if(!(button & PER_DGT_TB)){
		segaPtr->ang[Y] -= DEGtoANG(h * 2);
		segaPtr->ang[X] -= DEGtoANG(v * 2);
	}
	if(!(button & PER_DGT_TC)){
		segaPtr->pos[Z] += toFIXED(v * 50);
		segaPtr->ang[Z] += DEGtoANG(h * 2);
	}

	if(!(button & PER_DGT_ST)){
		init_camera((void*)segaPtr);
		init_object((void*)segaPtr);
	}
#endif
}



