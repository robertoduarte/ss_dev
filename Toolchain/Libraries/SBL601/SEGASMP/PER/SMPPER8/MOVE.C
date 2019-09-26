/****************************************
*			include files				*
****************************************/
#include	"per_x.h"

/*@*/
/************************************************************************
*																		*
*	【synopsis】														*
*																		*
*		move_cursor( device, &x, &y );									*
*																		*
*		ペリフェラルデバイスからのデータにしたがってカーソルを移動さ	*
*		せます。														*
*																		*
*																		*
*	【parameters】														*
*																		*
*		const SysDevice	*device;										*
*		int		*x,*y;													*
*																		*
*		<device> は、ペリフェラルデバイスからのデータ					*
*		<x>, <y> は、ペリフェラルデータにしたがって変更されます。		*
*																		*
*																		*
*	【return value】													*
*																		*
*		なし															*
*																		*
*																		*
*	【influence/reference objects】										*
*																		*
*		_move_cursor	reference	カーソル移動関数のテーブル			*
*									ＩＤをインデックスにして参照します。*
*																		*
*																		*
************************************************************************/
void	move_cursor( const SysDevice	*device, int	*x, int	*y ){
	trigger_t	trigger = PER_GetTrigger( device );
	
	if( trigger & TRG_UP )
		(*y) -= 3;
	else if( trigger & TRG_DOWN )
		(*y) += 3;
	
	if( trigger & TRG_LEFT )
		(*x) -= 4;
	else if( trigger & TRG_RIGHT )
		(*x) += 4;
	
	if( *x < 0 )
		*x = 0;
	if( *y < 0 )
		*y = 0;
	
	if( *x >= 320 )
		*x = 320-1;
	if( *y >= 224 )
		*y = 224-1;
}

