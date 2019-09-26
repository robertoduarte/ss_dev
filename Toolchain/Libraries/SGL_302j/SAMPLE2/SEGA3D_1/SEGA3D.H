
#define	SEGA3D_INC		"3d\dolphin.h"
#define	SEGA3D_INI		"3d\dolphin.ini"
#define	SEGA3D_CC		"3d\dolphin.cc"
#define	SEGA3D_C		"3d\dolphin.c"
#define	SEGA3D_GR		"3d\dolphin.gr"
#define	SEGA3D_LIGHT	"3d\light.ini"

#define	SEGA3D_TEXDEF	"texture\texture.def"
#define	SEGA3D_TEXC		"texture\dolphin.c"
#define	SEGA3D_TEX		tex_dolphin
#define	SEGA3D_PIC		pic_dolphin
#define	SEGA3D_MAXTEX	96

#define	CAMERA_ZDEF	-3000.0
#define	OFFSET_SCL	1.0

#if	1
#define		SEGA3D_GOUR
#endif

#if	0
#define		SEGA3D_TEXTURE
#endif

#if	1
#define		SEGA3D
#define	VTXROT90(a,b,c,d)	VERTICES(b,c,d,a)	/*0123->1230*/
#endif

