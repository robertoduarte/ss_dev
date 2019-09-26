/*------------------------------------------------------------------------
 *  FILE:	work.c
 *
 *  PURPOSE:
 *		ワークエリアのユーザー定義サンプル
 * 
 *------------------------------------------------------------------------
 */

#include    <sgl.h>

	extern Uint32	MaxVertices ;
	extern Uint32	MaxPolygon ;
	extern void*	SortList ;	
	extern void*	TransList ;
	extern void*	Zbuffer ;	
	extern void*	SlSpriteBuf ;	
	extern void*	MsSpriteBuf ;	
	extern Uint32	SpriteBufSize ;
	extern void*	Pbuffer ;	
	extern void*	CLOfstBuf ;	
	extern void*	CommandBuf ;	

/*
#define		MAX_VERTICES	2440
#define		MAX_POLYGON		1800
*/

#define		MAX_VERTICES	500
#define		MAX_POLYGON		100
#define		MAX_TRANSFER	20
#define		Zmaxcnt			128
#define		SRT_SIZE		36
#define		XYZ				12

#define		SORTLIST		0x060c0000
#define		TRANSLIST		SORTLIST+(MAX_POLYGON+5)*3*4
#define		ZBUFFER			TRANSLIST+(MAX_TRANSFER)*3*4
#define		SLSPRBUF		ZBUFFER+Zmaxcnt*4*4
#define		MSSPRBUF		SLSPRBUF+(MAX_POLYGON+5)*(SRT_SIZE)
#define		PBUFFER			MSSPRBUF+(MAX_POLYGON+5)*(SRT_SIZE)
#define		CLOFSTBUF		PBUFFER+(MAX_VERTICES)*(XYZ+4)
#define		COMMANDBUF		(CLOFSTBUF+33*32)|0x20000000

void Work_Set(void){

	MaxVertices = MAX_VERTICES ;
	MaxPolygon  = MAX_POLYGON  ;
	SortList    = (void*)(SORTLIST) ;
	TransList   = (void*)(TRANSLIST) ;
	Zbuffer     = (void*)(ZBUFFER) ;
	SlSpriteBuf = (void*)(SLSPRBUF) ;
	MsSpriteBuf = (void*)(MSSPRBUF) ;
	SpriteBufSize = (MAX_POLYGON+5)*36 ;
	Pbuffer     = (void*)(PBUFFER) ;
	CLOfstBuf   = (void*)(CLOFSTBUF) ;
	CommandBuf  = (void*)(COMMANDBUF) ;

/*
	MaxVertices = 2440 ;
	MaxPolygon  = 1800 ;
	SortList    = (void*)0x060c0000 ;
	TransList   = (void*)0x060c549c ;
	Zbuffer     = (void*)0x060c558c ;
	SlSpriteBuf = (void*)0x060c5d8c ;
	MsSpriteBuf = (void*)0x060d5b60 ;
	SpriteBufSize = (1800+5)*36 ;
	Pbuffer     = (void*)0x060e5934 ;
	CLOfstBuf   = (void*)0x060ef1b4 ;
	CommandBuf  = (void*)0x260ef5d4 ;
*/

}
