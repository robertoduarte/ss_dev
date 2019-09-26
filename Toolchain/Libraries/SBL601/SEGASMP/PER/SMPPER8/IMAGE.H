#if	!defined( __IMAGE_H )
	#define	__IMAGE_H
	
	/****************************************
	*			include files				*
	****************************************/
	#include	"sega_xpt.h"
	
	/****************************************
	*			define constants			*
	****************************************/
	#define	CURSOR_WIDTH		8
	#define	CURSOR_HEIGHT		16
	#define	SZ_CURSOR_HEIGHT	(CURSOR_HEIGHT*sizeof( Uint16 ))
	#define	SZ_CURSOR_WIDTH		(CURSOR_WIDTH*sizeof( Uint16 ))
	#define	RGB( r, g, b )		(0x8000U|((b) << 10)|((g) << 5 )|(r))
	#define	WHITE	RGB( 31, 31, 31 )
	#define	BLACK	RGB(  7,  7,  7 )
	#define	VOID	0x0000U
	
	/****************************************
	*		declare external objects		*
	****************************************/
	extern Uint16	cursor_image[CURSOR_WIDTH*CURSOR_HEIGHT];
	
#endif
