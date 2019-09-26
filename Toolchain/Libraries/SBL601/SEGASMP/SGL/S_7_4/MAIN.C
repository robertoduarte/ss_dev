/*----------------------------------------------------------------------*/
/*	Polygon & Texture						*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"

extern PDATA 	PD_PLANE;
extern TEXTURE	tex_sample[];
extern PICTURE	pic_sample[];

#define		max_texture		2

void set_texture(PICTURE *pcptr , Uint32 NbPicture)
{
	TEXTURE *txptr;
 
	for(; NbPicture-- > 0; pcptr++){
		txptr = tex_sample + pcptr->texno;
		slDMACopy((void *)pcptr->pcsrc,
			(void *)(SpriteVRAM + ((txptr->CGadr) << 3)),
			(Uint32)((txptr->Hsize * txptr->Vsize * 4) >> (pcptr->cmode)));
	}
}

void main()
{
	static ANGLE	ang[XYZ];
	static FIXED	pos[XYZ];

	slInitSystem(TV_320x224,tex_sample,1);
	set_texture(pic_sample,max_texture);
/*	slPrint("Sample program 7.4" , slLocate(9,2));	*/

	ang[X] = ang[Y] = ang[Z] = DEGtoANG(0.0);
	pos[X] = toFIXED(  0.0);
	pos[Y] = toFIXED(  0.0);
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

