#include	"sgl.h"
#include	"sega_sys.h"

#include	"scsp.h"

extern long clipflag;

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

short	pad_matrix(FIXED *pos, ANGLE *ang, FIXED *scl)
{
	static  int Rflag = 0;
	short	h, v, button;

	get_pad(&h, &v, &button);
	h *= 4, v *= 4;

	if(!(button & PER_DGT_TR)) {
		if ( !Rflag ) {
			clipflag = 1;
			Rflag = 1;
		}
	} else {
		Rflag = 0;
	}
	if(!(button & PER_DGT_TX)){
		pos[X] += (h * toFIXED(1.0));
		ang[X] += (v * DEGtoANG(1.0));
	}
	if(!(button & PER_DGT_TY)){
		pos[Y] += (v * toFIXED(1.0));
		ang[Y] += (h * DEGtoANG(1.0));
	}
	if(!(button & PER_DGT_TZ)){
		pos[Z] += (v * toFIXED(1.0));
		ang[Z] += (h * DEGtoANG(1.0));
	}
	if(!(button & PER_DGT_ST))
		return(1);
	return(0);
}


extern short	opaStat;






