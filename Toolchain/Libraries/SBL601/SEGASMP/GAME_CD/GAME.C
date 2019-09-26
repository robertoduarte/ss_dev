/*----------------------------------------------------------------------*/
/*	ＳＢＬ６サンプルゲームだよーん										*/
/*----------------------------------------------------------------------*/

#include	"sgl.h"
#include	"sega_scl.h"
#include	"sega_dbg.h"
#include	"sample.h"
#include	"game.h"
#include	"cd_ctrl.h"



extern	SysPort		*PortData;
const SysDevice	*device;
trigger_t	pltrigger[2],pltriggerE[2];

Uint16	submode = 0, gameflg = 1;
trigger_t	*padbuf;
Uint16	playflg = 0, emptyflg = 0;
Uint16	pauseflg = 0;

Sint32	music_trk = 3;

/* Uint32	*padbuf = (Uint32*)0x00200000;	*/

	static sequence_t	action[] = {
		_obje1_act,
		_obje2_act,
		_shot1_1_act,
		_shot1_2_act,
		_shot2_1_act,
		_shot2_2_act,
	};

enum	__gamemodenumber__{
	GAME_SUB_INIT,
	GAME_SUB_DEMO,
	GAME_SUB_MAIN,
	GAME_SUB_END
};

static void _game_init( void ){
	game_init();
	playflg = 1;
	padbuf = ( trigger_t * )0x00200000;
	submode = GAME_SUB_DEMO;

	PlayMusic(music_trk);

}

static void _game_demo( void ){
	if( player_rotation() != 0 )
		submode = GAME_SUB_MAIN;
}

static void _game_main(void){
	int i;

	/* デモ用にデータを保存	*/
	/*-----  チャック用
	if( pltriggerE[0] != 0 ){
		padbuf[n++] = pltriggerE[0];
	}						--------*/
	/* ３分間分のデータを収集する	*/
	if( padbuf < (trigger_t*)0x00215180 ){
		*padbuf++ = pltrigger[0];
		*padbuf++ = pltrigger[1];
	}

	/* ポーズの処理	*/
	if( ( ( pltriggerE[0] | pltriggerE[1] ) & TRG_START) != 0 ){
		if( pauseflg == 0 )
			pauseflg = 1;			/* ポーズ		*/
		else
			pauseflg = 2;			/* ポーズ解除	*/
	}

	if( pauseflg != 0 ){
		if( pauseflg == 1 ){
			DBG_SetCursol( 15, 12 );
			DBG_Printf( "* PAUSE *" );
		}
		else{
			pauseflg = 0;
			DBG_SetCursol( 15, 12 );
			DBG_Printf("         ");
		}
		_pause_act();
		return;
	}

	chg_angle();

	/* アクションループ	*/
	for( i = 0 ; i < 6 ; i++ )
		action[i]();

	disp_life();

	if( endflg != 0 )
		submode = GAME_SUB_END;

}

static void _game_end(void){
	/*	submode=GAME_SUB_INIT;	*/
	gameflg = 0;
}

/* メインルーチン	*/
void game(){

	static sequence_t	sub[] = {
		_game_init,
		_game_demo,
		_game_main,
		_game_end,
	};

	/* ゲーム中のメインループ	*/
	while( gameflg ){

		/* 所得したパッドデータをコピー	*/
		device = PER_GetDeviceR( &PortData[0], 0 );
		pltriggerE[0] = pltrigger[0];
		pltrigger[0] = PER_GetTrigger( device );
		pltriggerE[0] = (pltrigger[0]) ^ (pltriggerE[0]);
		pltriggerE[0] = (pltrigger[0]) & (pltriggerE[0]);

		device = PER_GetDeviceR( &PortData[1], 0 );
		pltriggerE[1] = pltrigger[1];
		pltrigger[1] = PER_GetTrigger( device );
		pltriggerE[1] = (pltrigger[1]) ^ (pltriggerE[1]);
		pltriggerE[1] = (pltrigger[1]) & (pltriggerE[1]);

		/* sub[level[1]]();	*/
		sub[submode]();

		/* Ｖブランク待ち	*/
		slSynch();
		CheckCDOpen();
	}

	/* ゲーム終了後 (gameflg = 0)ここに来る、
		   メインシーケンスに戻る手続きをする	*/
	gameflg = 1;
	submode = 0;
	level[0] = SQ_INIT;
}


/* デモ用	*/
void game_demo(void){
	int i;

	/* バッファからペリフェラルデータを得る	*/
	if(padbuf < (trigger_t*)0x00215180){
		pltrigger[0] = *padbuf++;
		pltrigger[1] = *padbuf++;
	}
	else
		emptyflg = 1;				/* データが無くなった	*/

	chg_angle();

	/* アクションループ	*/
	for( i = 0 ; i < 6 ; i++ )
		action[i]();
}
