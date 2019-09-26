#include	"sgl.h"
#include	"sega_sys.h"

#include	"sega3d.h"
#include	SEGA3D_INC
#ifdef	SEGA3D_GOUR
#define		VRAMaddr		(SpriteVRAM+0x70000)
#include	SEGA3D_GR
#endif

typedef struct{
	FIXED	cpos[XYZ];
	FIXED	ctarget[XYZ];
	ANGLE	cangle[XYZ];
	FIXED	pos[XYZ];
	ANGLE	ang[XYZ];
	FIXED	scl[XYZ];
}Sega3D, *Sega3DPtr;


static void set_texture(void)
{
#ifdef	SEGA3D_TEXTURE
	TEXTURE	*txptr;
	PICTURE	*pcPtr;
	Uint32	max;

	max = SEGA3D_MAXTEX;
	pcPtr = SEGA3D_PIC;
	for( ; max-- > 0 ; pcPtr++) {
		txptr = SEGA3D_TEX + pcPtr->texno;
		slDMACopy((void*)pcPtr->pcsrc, (void*)(SpriteVRAM + ((txptr->CGadr) << 3)),
			(Uint32)((txptr->Hsize * txptr->Vsize * 4) >> (pcPtr->cmode)));
	}
#endif
}

static void set_polygon(FIXED *pos, ANGLE *ang, FIXED *scl)
{
	slTranslate(pos[X], pos[Y], pos[Z]);
	slRotX(ang[X]);
	slRotY(ang[Y]);
	slRotZ(ang[Z]);
	slScale(scl[X], scl[Y], scl[Z]);
}

static void set_polygon2(FIXED *pos, ANGLE *ang, FIXED *scl)
{
	slRotX(ang[X]);
	slRotY(ang[Y]);
	slRotZ(ang[Z]);
	slTranslate(pos[X], pos[Y], pos[Z]);
	slScale(scl[X], scl[Y], scl[Z]);
}

void	init_camera(void *ptr)
{
	Sega3DPtr	segaPtr = (Sega3DPtr)ptr;

	segaPtr->cpos[X]	= toFIXED(0.0);
	segaPtr->cpos[Y]	= toFIXED(0.0);
	segaPtr->cpos[Z]	= toFIXED(CAMERA_ZDEF);
	segaPtr->ctarget[X]	= toFIXED(0.0);
	segaPtr->ctarget[Y]	= toFIXED(0.0);
	segaPtr->ctarget[Z]	= toFIXED(0.0);
	segaPtr->cangle[X]	= DEGtoANG(0.0);
	segaPtr->cangle[Y] 	= DEGtoANG(0.0);
	segaPtr->cangle[Z] 	= DEGtoANG(0.0);
}

void	init_object(void *ptr)
{
	Sega3DPtr	segaPtr = (Sega3DPtr)ptr;

	segaPtr->pos[X] =	toFIXED(0.0);
	segaPtr->pos[Y] =	toFIXED(0.0);
	segaPtr->pos[Z] =	toFIXED(0.0);
	segaPtr->scl[X] =	toFIXED(1.0 * OFFSET_SCL);
#ifdef	SEGA3D
	segaPtr->scl[Y] =	toFIXED(-1.0 * OFFSET_SCL);
#else
	segaPtr->scl[Y] =	toFIXED(1.0 * OFFSET_SCL);
#endif
	segaPtr->scl[Z] =	toFIXED(1.0 * OFFSET_SCL);
	segaPtr->ang[X] = 	DEGtoANG(0.0);
	segaPtr->ang[Y] =	DEGtoANG(0.0);
	segaPtr->ang[Z] = 	DEGtoANG(0.0);
}

void ss_main()
{
	Sega3D	sega3D;

#include	SEGA3D_LIGHT
#include	SEGA3D_INI

#ifdef	SEGA3D_TEXTURE
	slInitSystem(TV_320x224, SEGA3D_TEX, 1);
	set_texture();
#else
	slInitSystem(TV_320x224,NULL,1);
#endif

	slPrint("SEGA3D FLAT & Gouraud" , slLocate(8,2));
#ifdef	SEGA3D
	light[Y] *= -1;
#endif
	slLight(light);

	init_camera((void*)&sega3D);
	init_object((void*)&sega3D);
#ifdef	SEGA3D_GOUR
	slDMACopy(GRdata,(void *)VRAMaddr,sizeof(GRdata));
#endif

	while(-1){
		slUnitMatrix(CURRENT);

		chg_view((void*)&sega3D);

		slLookAt(sega3D.cpos, sega3D.ctarget, sega3D.cangle[Z]);
		slPushMatrix();
		{
			set_polygon(sega3D.pos, sega3D.ang, sega3D.scl);
#include	SEGA3D_CC
		}
		slPopMatrix();

		slSynch();
	}
}

