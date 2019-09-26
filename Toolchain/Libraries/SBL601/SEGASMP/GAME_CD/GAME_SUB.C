/*----------------------------------------------------------------------*/
/*	ＳＢＬ６サンプルゲームだよーん										*/
/*----------------------------------------------------------------------*/

#include	"act.h"
#include	"sega_scl.h"
#include	"sega_dbg.h"
#include	"sample.h"
#include	"game.h"
#include	"cd_ctrl.h"

#define	TRG_ANGLE	(TRG_X|TRG_Y|TRG_Z|TRG_R|TRG_L)

Uint16	fadetimer = 60;
Uint16	initflg_demo = 0;

static	CAMERA	cmbuf;				/* カメラの構造体変数	*/
extern	FIGHTER	f[];
extern	trigger_t	pltrigger[];


/* ゲームのイニシャル	*/
void game_init(){
	/* イニシャル	*/
/*
	slInitSystem(TV_320x224,NULL,1);
	SetVblank();
	InitCDBlock();
 */

	DBG_ClearScreen();

	SCL_SetPriority(SCL_SP0,2);
	SCL_SetSpriteMode(SCL_TYPE0,SCL_MIX,SCL_SP_WINDOW);
	initflg_demo=0;
	fadetimer=60;
	cmbuf.posZ = toFIXED(0.0);
	cmbuf.rotY = DEGtoANG(0.0);

	/* カメラの初期セット	*/
	cmbuf.angZ = DEGtoANG( 0.0 );		/* Z軸の角度	*/
	cmbuf.pos[X] = toFIXED(     0.0 );
	cmbuf.pos[Y] = toFIXED(     0.0 );
	cmbuf.pos[Z] = toFIXED( -1350.0 );
	cmbuf.target[X] = toFIXED(   0.0 );
	cmbuf.target[Y] = toFIXED(   0.0 );
	cmbuf.target[Z] = toFIXED( 300.0 );
	slLookAt( cmbuf.pos, cmbuf.target, cmbuf.angZ );

	init_objects();

}

/* ライフ表示	*/
void disp_life(void){
/*
	DBG_SetCursol(0, 18);
	DBG_Printf("0123456789012345678901234567890123456789");
*/
	DBG_SetCursol(0, 24);
	DBG_Printf("      LIFE                  LIFE");
	DBG_SetCursol(11, 24);
	DBG_Printf( "%d",f[0].hitpoint );
	DBG_SetCursol(33, 24);
	DBG_Printf( "%d",f[1].hitpoint );
}

/* 最初のアングル切り替えデモ	*/
int player_rotation(void){
	static int a,i,t1,t2;

	if( initflg_demo == 0 ){
		initflg_demo = 1;
		a = 0 ;
		i = 0 ;
		t1 = 0;
		t2 = 0;
	}

	slUnitMatrix(CURRENT);
	slLookAt( cmbuf.pos,cmbuf.target,cmbuf.angZ );

	_obje1_demo();
	_obje2_demo();

	t1++;
	if( t2 < 4 ){
		switch(t1){
			case 01:
				DBG_SetCursol(12, 3 );
				DBG_Printf("SBL6 SAMPLE GAME");
				break;
			case 30:
				DBG_SetCursol(12, 3 );
				DBG_Printf("                ");
				break;
			case 60:
				t1 = 0;
				t2++;
			default:
				break;
		}
	}
	else{
		if( t1 == 1 ){
			DBG_SetCursol(12, 3 );
			DBG_Printf("SBL6 SAMPLE GAME");
			DBG_SetCursol(16, 12);
			DBG_Printf(" PLAY !");
		}
		if( t1 == 50 ){
			DBG_SetCursol(17, 12);
			DBG_Printf("       ");
			i = 1;		/* デモ終了	*/
		}
	}

	/* オブジェの回転接近	*/
	if(a < 225){
		cmbuf.angZ += DEGtoANG( 8.0 );
		cmbuf.pos[Z] += toFIXED( 6.0 );
		a++;
	}

	return i;
}

/* 視点切り替え	*/
void chg_angle(){
	slUnitMatrix(CURRENT);
	switch( pltrigger[0] & TRG_ANGLE ){
		case TRG_X:
			cmbuf.pos[Z] += toFIXED(2.0);
			cmbuf.posZ += toFIXED(2.0);
			break;
		case TRG_Y:
			cmbuf.pos[Z] -= toFIXED(2.0);
			cmbuf.posZ -= toFIXED(2.0);
			break;
		case TRG_Z:
			cmbuf.rotY = DEGtoANG(0.0);
			cmbuf.posZ = toFIXED(0.0);
			cmbuf.pos[Z] = toFIXED(0.0);
			break;
		case TRG_L:
			cmbuf.rotY -= DEGtoANG(2.0);
			break;
		case TRG_R:
			cmbuf.rotY += DEGtoANG(2.0);
			break;
		default:
			break;
	}
}
/* 平行移動・回転のマトリクス演算	*/
void set_poly( ANGLE ang[XYZ] , FIXED pos[XYZ] ){
	slRotY(  cmbuf.rotY );
	slTranslate( pos[X], toFIXED(0.0), toFIXED(0.0) );
	slRotY( -cmbuf.rotY );
	slTranslate( toFIXED(0.0), pos[Y], pos[Z]-cmbuf.posZ );
	slRotX( ang[X] );
	slRotY( ang[Y] );
	slRotZ( ang[Z] );
}


/* 自機の移動	*/
void spr_move( trigger_t trigger , FIXED pos[XYZ], ANGLE ang[XYZ] ){
	/* 上下移動 	*/
	if(-toFIXED(120.0) < pos[Y] && (trigger & TRG_UP) != 0)
		pos[Y]-=toFIXED(3.0);
	if( toFIXED(100.0) > pos[Y] && (trigger & TRG_DOWN) != 0)
		pos[Y]+=toFIXED(3.0);
	/* 自転	*/
	ang[Y] += DEGtoANG( 12.0 );
	ang[Z] += DEGtoANG(  2.0 );
}

/* 当たり判定	*/
int	spr_col( FIXED posA[XYZ], FIXED posB[XYZ] ){
	FIXED a;

	if( posA[X] < posB[X] )
		a = posB[X] - posA[X];
	else
		a = posA[X] - posB[X];

	if( a < toFIXED( 10.0 ) ){
		if( posA[Y] < posB[Y] )
			a = posB[Y] - posA[Y];
		else
			a = posA[Y] - posB[Y];
		if( a < toFIXED(10.0) )
			return 1;
	}
	return 0;
}

