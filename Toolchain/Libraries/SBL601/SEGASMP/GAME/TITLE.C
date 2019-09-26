/****************************************
*			include files				*
****************************************/
#include	"sega_dbg.h"
#include	"per_x.h"
#include	"sample.h"

/****************************************
*			define constants			*
****************************************/
enum	{
	SQ_SUB_INIT,
	SQ_SUB_MAIN
};

static Uint16 count1,count2;

static void	_title_init( void ){
	count1=0;
	count2=0;
	DBG_ClearScreen();
	DBG_SetCursol(11, 12);
	DBG_Printf("SBL6 SAMPLE GAME");
	level[1] = SQ_SUB_MAIN;
}

static void	_title_main( void ){
	count1+=1;
	switch(count1){
		case	01:	DBG_SetCursol(10, 22);
					DBG_Printf("PRESS START BUTTON");
					if(count2==4){
						DBG_SetCursol(11, 12);
						DBG_Printf("  BATTLE CUBE   ");
					}
					break;
		case	30:	DBG_SetCursol(10, 22);
					DBG_Printf("                  ");
					break;
		case	60:	count1=0;
					count2+=1;
	}
	if(count2 == 10)
		level[0] = SQ_DEMO;
	if((triggerE & TRG_START) != 0)
		level[0] = SQ_GAME;
}

void	title( void ){
	static sequence_t	sub[] = {
		_title_init,
		_title_main,
	};
	
	sub[level[1]]();
}

