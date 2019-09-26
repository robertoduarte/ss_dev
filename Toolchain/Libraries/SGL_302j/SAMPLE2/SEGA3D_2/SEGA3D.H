#define	SEGA3D_INC		"3d\cube.h"
#define	SEGA3D_INI		"3d\cube.ini"
#define	SEGA3D_CC		"3d\cube.cc"
#define	SEGA3D_C		"3d\cube.c"
#define	SEGA3D_GR		"3d\cube.gr"
#define	SEGA3D_LIGHT	"3d\light.ini"

#define	SEGA3D_TEXDEF	"texture\texture.def"
#define	SEGA3D_TEXC		"texture\cube.c"
#define	SEGA3D_TEX		tex_cube
#define	SEGA3D_PIC		pic_cube
#define	SEGA3D_MAXTEX	3

#define	CAMERA_ZDEF	-160.0
#define	OFFSET_SCL	8.0

#if	1
#define		SEGA3D_GOUR
#endif

#if	1
#define		SEGA3D_TEXTURE
extern TEXTURE	SEGA3D_TEX[];
extern PICTURE	SEGA3D_PIC[];
#endif

#if	1
#define		SEGA3D
#define	VTXROT90(a,b,c,d)	VERTICES(b,c,d,a)	/*0123->1230*/
#endif

