//------------------------------------------------------------------------
//
//	X
//
//	POLYGON DATA
//
//	CONFIDENTIAL
//	Copyright (c) 1996, Sega Technical Institute
//
//	AUTHOR
//  CHRIS COFFIN	 
//
//	TARGET		    
//	SEGA SATURN
//								   
//	REVISION 
//	00/00/00
//
//	ORIGINAL FILE 
//	s:\sgl_3\sgl\sample\chrome\poly.c
//
//------------------------------------------------------------------------

#include	"sgl.h"
#include	"sega_sys.h"
							   

//#define	NO_TEXTURE

// test chrome/bump object

#ifdef	NO_TEXTURE

	// NON TEXTURED FLAGS
	#define		Chrome_ATR	(MESHoff | CL_Gouraud | CL256Bnk)
	#define		Chrome_OPT	(UseGouraud | UsePalette)
	#define		Chrome_COL	(16)
	#define		Chrome_TEX 	(No_Texture)
	#define		Chrome_POLY	(sprPolygon)

#else

	// TEXTURED FLAGS
	#define		Chrome_ATR	(CL256Bnk|CL_Gouraud|MESHoff|SPdis|ECdis)
	#define		Chrome_OPT	(UseGouraud)
	#define		Chrome_COL	(1024)
	#define		Chrome_TEX	(0)
	#define		Chrome_POLY	(sprNoflip)

#endif


// DEMO MODEL DATA
							  
#include	"mdl_6.mdl"