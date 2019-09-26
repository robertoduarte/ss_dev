/****************************************
*			include files				*
****************************************/
#include	"sega_dbg.h"
#include	"per_x.h"

/****************************************
*		declare private objects			*
****************************************/
/*
**	device_name[]
**		ペリフェラルデバイスの名前のテーブル
**		ＩＤをインデックスにして参照します。
*/
static char	*device_name[] = {
	"DIGITAL ","DIGITAL ","DIGITAL ","DIGITAL ",
	"DIGITAL ","DIGITAL ","DIGITAL ","DIGITAL ",
	"DIGITAL ","DIGITAL ","DIGITAL ","DIGITAL ",
	"DIGITAL ","DIGITAL ","DIGITAL ","DIGITAL ",
	
	"ANALOGUE","ANALOGUE","ANALOGUE","ANALOGUE",
	"ANALOGUE","ANALOGUE","ANALOGUE","ANALOGUE",
	"ANALOGUE","ANALOGUE","ANALOGUE","ANALOGUE",
	"ANALOGUE","ANALOGUE","ANALOGUE","ANALOGUE",
	
	"POINTING","POINTING","POINTING","POINTING",
	"POINTING","POINTING","POINTING","POINTING",
	"POINTING","POINTING","POINTING","POINTING",
	"POINTING","POINTING","POINTING","POINTING",
	
	"KEYBOARD","KEYBOARD","KEYBOARD","KEYBOARD",
	"KEYBOARD","KEYBOARD","KEYBOARD","KEYBOARD",
	"KEYBOARD","KEYBOARD","KEYBOARD","KEYBOARD",
	"KEYBOARD","KEYBOARD","KEYBOARD","KEYBOARD",
	
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	
	"MD??????","MD3B    ","MD6B    ","MDMOUSE ",
	"MD??????","MD??????","MD??????","MD??????",
	"MD??????","MD??????","MD??????","MD??????",
	"MD??????","MD??????","MD??????","MD??????",
	
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","VR-GUN  ","UNKNOWN ",
	"UNKNOWN ","UNKNOWN ","UNKNOWN ","NONE    ",
};

/*@*/
/************************************************************************
*																		*
*	【synopsis】														*
*																		*
*		display_device_info( device, x, y );							*
*																		*
*		ペリフェラルライブラリによって収集されたデータのダンプを出力	*
*		します。														*
*																		*
*																		*
*	【parameters】														*
*																		*
*		const SysDevice	*device;										*
*		int			x,y;												*
*																		*
*		<device> は、ペリフェラルデータへのポインタ（NULL は、未接続）	*
*		<x>, <y> は、表示座標（左上が 0,0 ）							*
*																		*
*																		*
*	【return value】													*
*																		*
*		なし															*
*																		*
*																		*
*	【influence/reference objects】										*
*																		*
*		device_name	reference	ペリフェラルデバイスの名前のテーブル	*
*								ＩＤをインデックスにして参照します。	*
*																		*
*																		*
************************************************************************/
void	display_device_info( const SysDevice	*device, int	x, int	y ){
	DBG_SetCursol( x, y );
	
	if( device == NULL ){
		DBG_Printf( "NONE     -- -- -- -- -- -- --" );
	}
	else{
		Uint8	*data = ( Uint8 * )device->data;
		int		id = PER_GetID( device );
		int		size  = PER_GetSize( device );
		char	*name = device_name[id];
		int		n = 0;
		
		DBG_Printf( "%s %02X", name, id );
		while( n < 6 ){
			if( n < size )
				DBG_Printf( " %02X", data[n] );
			else
				DBG_Printf( " --" );
			
			n++;
		}
	}
}

