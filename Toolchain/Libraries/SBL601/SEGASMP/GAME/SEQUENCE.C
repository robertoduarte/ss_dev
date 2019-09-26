/*
**	ゲームシーケンス制御のサンプル
*/

/****************************************
*			include files				*
****************************************/
#include	"sgl.h"
#include	"sega_int.h"
#include	"sega_scl.h"
#include	"sega_dbg.h"
#include	"sample.h"
#include	"game.h"

/****************************************
*		declare publib objects			*
****************************************/
SysPort		*PortData;
int			level[2];
int			timer;
trigger_t	trigger;
trigger_t	triggerE;

/****************************************
*		declare private objects			*
****************************************/
void	init(void){
	slInitSystem(TV_320x224,NULL,1);
	SetVblank();
/*	level[0]=SQ_TITLE;	*/
	level[0]=SQ_LOGO;
}

void	main( void ){
	level[0] = SQ_INIT;
	level[1] = 0;
	timer = 0;
	trigger = 0;
	
	/* シーケンスのメインループ	*/
	for(;;){
		static sequence_t	sequence[] = {
			init,
			title,
			demo,
			logo,
			game,
		};
		const SysDevice	*device;
		
		int		sq_current = level[0];
		
		device  = PER_GetDeviceR( &PortData[0], 0 );
		triggerE = trigger;
		trigger = PER_GetTrigger( device );
		triggerE = (trigger) ^ (triggerE);
		triggerE = (trigger) & (triggerE);
		
		sequence[level[0]]();
		
		/* メインモードが変わっていたらタイマ、サブモードをクリアー	*/
		if( level[0] != sq_current ){
			level[1] = 0;
			timer = 0;
		}
		else{
			timer++;
		}
		
		/* Ｖブランク待ち	*/
		slSynch();
	}
}
