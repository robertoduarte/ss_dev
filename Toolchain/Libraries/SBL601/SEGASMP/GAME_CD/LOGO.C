/****************************************
*			include files				*
****************************************/
#include	"sega_dbg.h"
#include	"sample.h"

/****************************************
*			define constants			*
****************************************/
enum	{
	SQ_SUB_INIT,
	SQ_SUB_MAIN
};

static Uint16 count1;

static void	_logo_init( void ){
	count1=0;
	DBG_ClearScreen();
	DBG_SetCursol(17, 12);
	DBG_Printf("SEGA");
	level[1] = SQ_SUB_MAIN;
}

static void	_logo_main( void ){
	count1+=1;
	if(count1 == 60*5)
		level[0] = SQ_TITLE;
}

void	logo( void ){
	static sequence_t	sub[] = {
		_logo_init,
		_logo_main,
	};
	
	sub[level[1]]();
}

