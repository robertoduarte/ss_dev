
#define	SEGA3D_H		"3d\opa.h"
#define	SEGA3D_INI		"3d\opa.ini"
#define	SEGA3D_CC		"3d\opa.cc"
#define	SEGA3D_C		"3d\opa.c"
#define	SEGA3D_GR		"3d\opa.gr"
#define	SEGA3D_LIGHT	"3d\light.ini"

#define	SEGA3D_TEXDEF	"texture\texture.def"
#define	SEGA3D_TEXC		"texture\opa.c"
#define	SEGA3D_TEX		tex_opa
#define	SEGA3D_PIC		pic_opa
#define	SEGA3D_MAXTEX	1


#define	CAMERA_ZDEF	-100.0
#define	OFFSET_SCL	20.0

#if	1
#define		SEGA3D_GOUR
#define		VRAMaddr		(SpriteVRAM+0x70000)
#include	SEGA3D_GR
#endif

#if	0
#define		SEGA3D_TEXTURE
extern TEXTURE	SEGA3D_TEX[];
extern PICTURE	SEGA3D_PIC[];
#endif

#if	1
#define		SEGA3D
#define	VTXROT90(a,b,c,d)	VERTICES(b,c,d,a)	/*0123->1230*/

typedef struct{
	FIXED	cpos[XYZ];
	FIXED	ctarget[XYZ];
	ANGLE	cangle[XYZ];
	FIXED	pos[XYZ];
	ANGLE	ang[XYZ];
	FIXED	scl[XYZ];
}Sega3D, *Sega3DPtr;

#endif


