#define		FNT_USA		0
#define		FNT_JAPAN	1

extern	Uint16	FntXsize;
extern	Uint16	FntYsize;
extern	Uint8	FntContry;
extern	Uint8	FntBitPat[];

extern	void  FNT_SetBuffSize(Uint16 x,Uint16 y,Uint8 contry);
/* #if !(__GNUC__) */
#ifndef __GNUC__
extern	void  FNT_Print256(Uint8 *vram,Uint8 *str,
								Uint16 x,Uint16 y,Uint16 col,Uint16 back);
#else
extern	void  FNT_Print256(volatile Uint8 *vram,Uint8 *str,
								Uint16 x,Uint16 y,Uint16 col,Uint16 back);
#endif
