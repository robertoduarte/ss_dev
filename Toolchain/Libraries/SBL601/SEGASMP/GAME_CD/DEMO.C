/****************************************
*			include files				*
****************************************/
#include	"sgl.h"
#include	"sega_dbg.h"
#include	"per_x.h"
#include	"sample.h"
extern void game_init(void);
extern void game_demo(void);
extern Uint16 endflg,playflg,emptyflg;
extern trigger_t *padbuf;
extern trigger_t demo_data[];

/****************************************
*			define constants			*
****************************************/
enum	{
	SQ_SUB_INIT,
	SQ_SUB_PLAY
};

static Uint16 count1;
/*	static Uint32 *data_ptr;	*/

static void	_demo_init( void ){
	count1=0;
	DBG_ClearScreen();

	/* 一度でもプレイしていたら (playflg=1) プレイデモを流す	*/
	if(playflg == 0){
		slDMACopy((Uint32*)demo_data,(Uint32*)0x00200000,(Uint32)0x03000 );
		slDMAWait();
	}

	level[1] = SQ_SUB_PLAY;
	game_init();
	padbuf=(trigger_t*)0x00200000;
	emptyflg=0;
}

static void	_demo_play( void ){
	count1++;
	switch( count1  ){
		case 01:
			DBG_SetCursol( 10, 22 );
			DBG_Printf("PRESS START BUTTON");
			DBG_SetCursol( 15, 12 );
			if( playflg == 0 )
				DBG_Printf("DEMO PLAY");
			else
				DBG_Printf(" REPLAY  ");
			break;
		case 30:
			DBG_SetCursol( 10, 22 );
			DBG_Printf("                  ");
			DBG_SetCursol( 15, 12 );
			DBG_Printf("         ");
			break;
		case 60:
			count1 = 0;
		default:
			break;
	}

	if( ( triggerE & TRG_START ) != 0 )
		level[0] = SQ_TITLE;

	game_demo();

	if( ( endflg != 0 ) || ( emptyflg != 0 ) )
		level[0] = SQ_LOGO;

}

void	demo( void ){
	static sequence_t	sub[] = {
		_demo_init,
		_demo_play,
	};

	sub[level[1]]();
}

