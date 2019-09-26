/*----------------------------------------------------------------------*/
/*	ＳＢＬ６サンプルゲームだよーん										*/
/*----------------------------------------------------------------------*/

#include	"act.h"
#include	"sega_scl.h"
#include	"sega_dbg.h"
#include	"sample.h"
#include	"game.h"


Uint16	endtimer=0,endflg=0;

extern	trigger_t	pltrigger[];
extern	Uint16	fadetimer;

/*FIGHTER	*p1,*p2;*/
FIGHTER	f[2];
BULLET	b[2][_BULLETS];
ONEPLANE	p[6];
FIGHTER	*pt[] = { &f[1],&f[0] };
PDATA	PDcube[2];
PDATA	*PDlist[] = { &PD_CUBE1 , &PD_CUBE2 };
PDATA	* const _PDplane[] = {
			&PD_BREAK1,
			&PD_BREAK2,
			&PD_BREAK3,
			&PD_BREAK4,
			&PD_BREAK5,
			&PD_BREAK6
		};

const short	spdtbl[6][XYZ]	= {
				{  0 , -2 ,  1 },
				{ -1 ,  0 ,  0 },
				{  0 , -2 , -1 },
				{  1 ,  0 ,  0 },
				{  0 , -1 ,  0 },
				{  0 ,  1 ,  0 }
			};
const ANGLE	rottbl[6][XYZ]	={
				{DEGtoANG(-10.0 ),DEGtoANG(-10.0 ),DEGtoANG(  0.0 )},
				{DEGtoANG(  0.0 ),DEGtoANG( 10.0 ),DEGtoANG(-10.0 )},
				{DEGtoANG( 10.0 ),DEGtoANG( 10.0 ),DEGtoANG(  0.0 )},
				{DEGtoANG(  0.0 ),DEGtoANG( 10.0 ),DEGtoANG( 10.0 )},
				{DEGtoANG( 10.0 ),DEGtoANG(  0.0 ),DEGtoANG( 10.0 )},
				{DEGtoANG(-10.0 ),DEGtoANG(  0.0 ),DEGtoANG(-10.0 )}
			};

unsigned short	flash_color[] = {	/* Flashの色	*/
			C_RGB(25,28,30),
			C_RGB(25,25,30),
			C_RGB(25,28,30),
			C_RGB(25,25,30),
			C_RGB(30,30,25),
			C_RGB(30,30,25)
			};
unsigned short	org_color[] = {	/* 元の色	*/
			C_RGB( 6,16,25),
			C_RGB( 6, 6,25),
			C_RGB( 6,16,25),
			C_RGB( 6, 6,25),
			C_RGB(20,20,10),
			C_RGB(20,20,10)
			};


#define	TRG_FIRE	(TRG_A|TRG_B)

#define	resetplayer( sts )	\
	do{						\
		sts->shotwait = 0;	\
		sts->collision = 0;	\
		sts->hitpoint = 3;	\
		endtimer = 0;		\
		endflg = 0;			\
	}while(0)



void	player(	FIGHTER	*sts ){
	int	i,j;
	trigger_t	myswitch = pltrigger[sts->id];

	if(( myswitch & TRG_C) != 0 ){
		resetplayer( sts );
	}

	if( sts->hitpoint != 0 ){					/* 生きている */
		if( sts->shotwait == 0 ){				/* 攻撃 */
			if( ( myswitch & TRG_FIRE ) != 0 ){
				for( i = 0 ; i < _BULLETS ; i++ ){
					if( sts->myb[i]->busy == 0 ){
						FIXED	speed = _BULLETSPEED;
						ANGLE	rotate = _BULLETROT;
						for( j = X ; j < XYZ ; j++){
							sts->myb[i]->pos[j] = sts->pos[j];
						}
						sts->myb[i]->busy = 1;
						sts->myb[i]->target = pt[sts->id];
						if( sts->pos[X] > 0 ){
							speed *= -1;
							rotate *= -1;
						}
						sts->myb[i]->spd[X] = speed;
						sts->myb[i]->rot[Z] = rotate;
						sts->shotwait = _SHOTINTERVAL;
						break;
					}
				}
			}
		}
		else{
			if( --sts->shotwait < 0 )
				sts->shotwait = 0;
		}

		if( sts->collision == 0 ){				/* 被弾 */
			if( sts->hit != 0){
				sts->collision = _NODAMAGE;
				if( --sts->hitpoint == 0 )
					break_init( sts->ang,sts->pos );
			}
		}
		else{	/*無敵*/
			if( --sts->collision < 0 )
				sts->collision = 0;
			else{
				unsigned short	*pCOLOR;
				if( sts->collision & 0x01 )
					pCOLOR = flash_color;
				else
					pCOLOR = org_color;
				for( i = 0 ; i < 6 ; i++ )
					sts->pd->attbl[i].colno = pCOLOR[i];
			}
		}
		sts->hit = 0;

		slPushMatrix();							/* 表示 */
		spr_move( myswitch,sts->pos,sts->ang);
		set_poly(sts->ang , sts->pos);
		slPutPolygon( sts->pd );
		slPopMatrix();

	}
	else{	/* 死んでる */
		spr_break();	/* 爆発 */
	}
}



void _obje1_act(void){
	player( &f[0] );
}

void _obje2_act(void){
	player( &f[1] );
}


void	bullet( BULLET	*sts ){
	if( sts->busy != 0 ){
		FIXED	distance;
		if( ( distance = sts->pos[X] ) < 0 )
			distance *= -1;

		if( toFIXED( 280.0 ) > distance ){
			sts->pos[X] += sts->spd[X];
			sts->ang[Z] += sts->rot[Z];

			slPushMatrix();
			set_poly( sts->ang, sts->pos );
			slPutPolygon( sts->pd );
			slPopMatrix();
		}
		else{
			sts->busy = 0;
			return;
		}
		if( sts->target->hitpoint != 0 ){
			if( spr_col( sts->pos , sts->target->pos ) ){
				sts->target->hit++;
				/*	sts->busy = 0;	*/
			}
		}
	}
}


void _shot1_1_act(void){
	int	i;
	for( i = 0 ; i < _BULLETS ; i++)
		bullet( &b[0][i] );
}
void _shot1_2_act(void){
	;
}
void _shot2_1_act(void){
	int	i;
	for( i = 0 ; i < _BULLETS ; i++)
		bullet( &b[1][i] );
}
void _shot2_2_act(void){
	;
}

void	rot_player( int	idnum ){
	/* 自転	*/
	f[idnum].ang[Y] += DEGtoANG( 12.0 );
	f[idnum].ang[Z] += DEGtoANG(  2.0 );
	slPushMatrix();
	set_poly( f[idnum].ang , f[idnum].pos );
	slPutPolygon( f[idnum].pd );
	slPopMatrix();
}

void _obje1_demo(void){
	rot_player( 0 );
}

void _obje2_demo(void){
	rot_player( 1 );
}



void	init_objects( void ){
	int	i,j;
	endtimer = 0;
	endflg = 0;

	/* 自機オブジェの初期ポジション	*/
	f[0].ang[X] = f[1].ang[X] = DEGtoANG( 0.0 );
	f[0].ang[Y] = f[1].ang[Y] = DEGtoANG( 0.0 );
	f[0].ang[Z] = f[1].ang[Z] = DEGtoANG( 0.0 );

	f[0].id = 0;
	f[1].id = 1;

	f[0].shotwait  = f[1].shotwait = 0;
	f[0].collision = f[1].collision = 0;
	f[0].hitpoint  = f[1].hitpoint = 3;

	f[0].pos[X] = toFIXED(-130.0 );
	f[0].pos[Y] = toFIXED(   0.0 );
	f[0].pos[Z] = toFIXED( 240.0 );

	f[1].pos[X] = toFIXED( 130.0);
	f[1].pos[Y] = toFIXED(   0.0);
	f[1].pos[Z] = toFIXED( 240.0);

	for( i = 0 ; i < 2 ; i++ ){
		unsigned char	*src,*dst;
		src = ( unsigned char * )PDlist[i];
		dst = ( unsigned char * )&PDcube[i];
		f[i].pd = &PDcube[i];
		for( j = 0 ; j < ( int )sizeof( PDATA ) ; j++ )
			*dst++ = *src++;
		for( j = 0 ; j < _BULLETS ; j++ )
			f[i].myb[j] = &b[i][j];
	}


	/* ショット弾の初期ポジション	*/
	for( i = 0 ; i < 2 ; i++ ){
		for( j = 0 ; j < _BULLETS ; j++ ){
			b[i][j].ang[X] = b[i][j].ang[Y] = b[i][j].ang[Z] = DEGtoANG( 0.0 );
			b[i][j].pos[X] = b[i][j].pos[Y] = b[i][j].pos[Z] = toFIXED( 0.0 );
			b[i][j].busy = 0;
			b[i][j].pd = &PD_PLANE1;
			b[i][j].id = ( i << 12 ) + ( j << 8 ) + i*_BULLETS + j;
		}
	}

	/* 飛び散るポリゴンの初期ポジション	*/
	for( i = 0 ; i < 6 ; i++ ){
		p[i].ang[X] = p[i].ang[Y] = p[i].ang[Z] = DEGtoANG( 0.0);
		p[i].pos[X] = p[i].pos[Y] = p[i].pos[Z] = toFIXED( 0.0);
		p[i].pd = _PDplane[i];
		for( j = X ; j < XYZ ; j++ ){
			p[i].spd[j] = spdtbl[i][j];
			p[i].rot[j] = rottbl[i][j];
		}
	}
}



/* 破壊後の初期化	*/
void break_init(ANGLE ang[XYZ],FIXED pos[XYZ]){
	int	i,j;
	for( i = 0 ; i < 6 ; i++ ){
		for( j = X ; j < XYZ ; j++ ){
			p[i].ang[j] = ang[j];
			p[i].pos[j] = pos[j];
		}
	}
}

/* ポリゴンが飛び散る	*/
void spr_break(void){
	static Uint16 c1,c2,c3,c4,c5;
	FIXED	x = toFIXED( fadetimer )/6;
	int	i,j;
	for( i = 0 ; i < 6 ; i++ ){
		for( j = X ; j < XYZ ; j ++ ){
			switch( abs( p[i].spd[j] ) ){
				case 0:
					break;
				case 1:
					p[i].pos[j] +=  x *  p[i].spd[j];
					break;
				case 2:
					p[i].pos[j] +=  p[i].spd[j];
					break;
				default:
					break;
			}
			p[i].ang[j] += p[i].rot[j];

		}
	 	slPushMatrix();
		set_poly( p[i].ang,p[i].pos );
		slPutPolygon( p[i].pd );
		slPopMatrix();
	}

	if( ++endtimer == 90 )
		endflg=1;					/* ゲーム終了	*/

	if(fadetimer >0)
		fadetimer--;

	c1 =  6 * fadetimer/60;
	c2 = 25 * fadetimer/60;
	c3 = 16 * fadetimer/60;
	c4 = 10 * fadetimer/60;
	c5 = 20 * fadetimer/60;
	attribute_BREAK[0].colno=C_RGB(c1,c1,c2);
	attribute_BREAK[1].colno=C_RGB(c1,c3,c2);
	attribute_BREAK[2].colno=C_RGB(c5,c5,c4);
}

void	_pause_act( void ){
	int	i,j;
	for( i = 0 ; i < 2 ; i++){
		slPushMatrix();							/* 表示 */
		set_poly( f[i].ang , f[i].pos );
		slPutPolygon( f[i].pd );
		slPopMatrix();
	}
	for( i = 0 ; i < 2 ; i++){
		for( j = 0 ; j < _BULLETS ; j++ ){
			if( b[i][j].busy != 0 ){
				slPushMatrix();							/* 表示 */
				set_poly( b[i][j].ang , b[i][j].pos );
				slPutPolygon( &PD_PLANE1 );
				slPopMatrix();
			}
		}
	}
}
