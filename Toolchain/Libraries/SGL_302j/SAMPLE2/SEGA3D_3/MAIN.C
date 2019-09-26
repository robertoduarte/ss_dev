/********************************************************************
           SEGA3D-->SGL & SEGA2D-->SGL SAMPLE PROGRAM

********************************************************************/

#include	"sgl.h"
#include	"sega_sys.h"

#include	"sega2d.h"
#include	SEGA2D_H

#include	"sega3d.h"
#include	SEGA3D_H

#include	"scsp.h"


#define		RBG0_CEL_ADR	(VDP2_VRAM_A0	)
#define		RBG0_MAP_ADR	(VDP2_VRAM_B0	)
#define		RBG0_COL_ADR	(VDP2_COLRAM	)
#define		RBG0_KTB_ADR	(VDP2_VRAM_A1	)
#define		RBG0_PRA_ADR	(VDP2_VRAM_A1	+ 0x1fe00)

#define		BACK_COL_ADR	(VDP2_VRAM_B1   + 0x1fffe)

static void set_texture(void);
static void set_matrix(FIXED *, ANGLE *, FIXED *);
static void init_matrix(FIXED *, ANGLE *, FIXED *);

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
	segaPtr->ang[X] = 	DEGtoANG(90.0);
	segaPtr->ang[Y] =	DEGtoANG(0.0);	/*for Scroll to Sprite*/
	segaPtr->ang[Z] = 	DEGtoANG(0.0);
}

static void init_matrix(FIXED *pos, ANGLE *ang, FIXED *scl)
{
	pos[X] = toFIXED(0.0);
	pos[Y] = toFIXED(0.0);
	pos[Z] = toFIXED(80.0);
	ang[X] = DEGtoANG(-70.0);
	ang[Y] = DEGtoANG(0.0);
	ang[Z] = DEGtoANG(0.0);
	scl[X] = toFIXED(1.0);
	scl[Y] = toFIXED(1.0);
	scl[Z] = toFIXED(1.0);
}

static void set_matrix(FIXED *pos, ANGLE *ang, FIXED *scl)
{
	slTranslate(pos[X], pos[Y], pos[Z]);
	slRotZ(ang[Z]);
	slRotY(ang[Y]);
	slRotX(ang[X]);
	slScale(scl[X], scl[Y], scl[Z]);
}
#define	set_polygon	set_matrix

static void set_polygon2(FIXED *pos, ANGLE *ang, FIXED *scl)
{
	slRotZ(ang[Z]);
	slRotY(ang[Y]);
	slRotX(ang[X]);
	slTranslate(pos[X], pos[Y], pos[Z]);
	slScale(scl[X], scl[Y], scl[Z]);
}

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

static setAngLimit(ANGLE *ang, float min, float max)
{
	if(*ang > DEGtoANG(max))
		*ang = DEGtoANG(max);
	else if(*ang < DEGtoANG(min))
		*ang = DEGtoANG(min);
}

long clipflag = 0;
short	opaStat = 0;
#include	SEGA3D_INI

void	ss_main(void)
{
	FIXED	pos[XYZ], scl[XYZ], gpos[XYZ];
	ANGLE	ang[XYZ];
	Sega3D	sega3D;
	Uint8	map[16] = {	0,0,0,0,
						0,0,0,0,
						0,0,0,0,
						0,0,0,0
					};
	long cliplevel = 1;
#include	SEGA3D_LIGHT

#ifdef	SEGA3D_TEXTURE
	slInitSystem(TV_320x224, SEGA3D_TEX, 1);
	set_texture();
#else
	slInitSystem(TV_320x224,NULL,1);
#endif

	slTVOff();
	slPrint("SEGA3D & SEGA2D SAMPLE" , slLocate(9,2));
	slColRAMMode(CRM16_2048);
	slBack1ColSet((void *)BACK_COL_ADR, C_RGB(31,15,31));

	slZdspLevel(cliplevel);

	slRparaInitSet((void *)RBG0_PRA_ADR);
	slMakeKtable((void *)RBG0_KTB_ADR);
	slCharRbg0(S2D_COLTYPE, S2D_CHRSIZE);
	slPageRbg0((void *)RBG0_CEL_ADR, 0, S2D_PNBSIZE);
	slPlaneRA(PL_SIZE_1x1);
	sl1MapRA((void *)RBG0_MAP_ADR);
	slOverRA(3);
	slKtableRA((void *)RBG0_KTB_ADR, K_FIX | K_DOT | K_2WORD | K_ON);
	slRparaMode(RA);

	slDMACopy((void*)S2D_CEL, (void*)RBG0_CEL_ADR, (Uint32)S2D_CELSIZE);
	slDMACopy((void*)S2D_MAP, (void*)RBG0_MAP_ADR, (Uint32)(MAP_HSIZE * MAP_VSIZE * MAP_UNIT));
	slDMACopy((void*)S2D_PAL, (void*)RBG0_COL_ADR, (Uint32)S2D_PALSIZE);

	slScrTransparent(RBG0ON);
	slColRAMOffsetNbg0(7);
	slScrAutoDisp(NBG0ON | RBG0ON);
	slTVOn();

#ifdef	SEGA3D
	light[Y] *= -1;
#endif
#ifdef	SEGA3D_GOUR
	slDMACopy(GRdata,(void *)VRAMaddr,sizeof(GRdata));
#endif

	init_camera((void*)&sega3D);
	init_object((void*)&sega3D);
	init_matrix(pos, ang, scl);

	while(1)
	{
		if(pad_matrix(pos, ang, scl))
			init_matrix(pos, ang, scl);
		if ( clipflag ) {
			if (cliplevel >= 3 ) {
				cliplevel = 1;
			} else {
				cliplevel++;
			}
			slZdspLevel(cliplevel);
			clipflag = 0;
		}
		slPrint("Cliplevel" , slLocate(23,4));
		slPrintHex( cliplevel, slLocate(22,5));

		whichAction((void*)&sega3D);

		slUnitMatrix(CURRENT);
		slLight(light);
		slLookAt(sega3D.cpos, sega3D.ctarget, sega3D.cangle[Z]);

		slPushMatrix();
		{
			set_matrix(pos, ang, scl);

			slPushMatrix();
			{
				set_polygon(sega3D.pos, sega3D.ang, sega3D.scl);
#include	SEGA3D_CC	/*disp model*/
			}
			slPopMatrix();

			slPushMatrix();
			{
				slCurRpara(RA);
				slTranslate(toFIXED(-256.0), toFIXED(-256.0), toFIXED(0.0));
				slScrMatConv();
				slScrMatSet();
			}
			slPopMatrix();

		}
		slPopMatrix();
		reset_vbar(39);
		slSynch();
		set_vbar(39);
	} 
}

