/****************************************
*			include files				*
****************************************/
#include	"per_x.h"
#include	"smpc.h"
#include	"vdp2.h"

/****************************************
*			define constants			*
****************************************/
#define	THRESHOLD	4

/****************************************
*		declare oexternal objects		*
****************************************/
extern Uint16	tvstat;

static trigger_t	GetTriggerGun( void ){
	trigger_t	trigger = 0;
	
	Uint8	portbit = ( 0x01 << 0 );
	Uint8	data;
	
	SMPC_IOSEL = portbit;
#ifdef (ADDRESS_CHECKER_ERROR)
	SMPC_DDR( 0 ) = 0;
	SMPC_PDR( 0 ) = ~0;
#else
	// 961120 -- jay
	// Write to the 7-bit DDR/PDR regs; insure that all upper bits are zero.
	SMPC_DDR( 0 ) = 0;
	SMPC_PDR( 0 ) = ~0 & 0x7F;
#endif
	SMPC_EXLE  = portbit;
	
	data = SMPC_PDR( 0 );
	
	if( ~data & GUN_START )
		trigger |= TRG_START;
	if( ~data & GUN_TRIGGER )
		trigger |= TRG_C;
	
	return	trigger;
}

trigger_t	get_trigger( const SysPort	*port ){
	static trigger_t keep_trigger = 0;
	
	Uint32		mid = port[0].id;
	
	trigger_t	edge = 0;
	
	if( mid == MID_GUN ){
		trigger_t	trigger = GetTriggerGun();
		
		edge = PER_GetPressEdge( keep_trigger, trigger );
		keep_trigger = trigger;
	}
	
	return	edge;
}

void	move_cursor( const SysPort	*port, int	*x, int	*y ){
	Uint32	mid = port[0].id;
	
	if( mid == MID_GUN ){
		if( tvstat & VDP2_TVSTAT_EXLTFG ){
			/*
			**	バーチャガンは、ブレがものすごいので、
			**	こうでもしないと使いものにならない。
			*/
			static int	hcnt1 = 0;
			static int	vcnt1 = 0;
			
			int		hcnt2 = ( int )((( short )VDP2_HCNT & 0x03fe ) >> 1 );
			int		vcnt2  = ( int )( ( short )VDP2_VCNT & 0x03ff );
			
			if( hcnt1 < hcnt2 ){
				int		dx = hcnt2 - hcnt1;
				
				if( dx >= THRESHOLD )
					hcnt1 += ( dx - THRESHOLD );
			}
			else if( hcnt1 > hcnt2 ){
				int		dx = hcnt1 - hcnt2;
				
				if( dx >= THRESHOLD )
					hcnt1 -= ( dx - THRESHOLD );
			}
			
			if( vcnt1 < vcnt2 ){
				int		dy = vcnt2 - vcnt1;
				
				if( dy >= THRESHOLD )
					vcnt1 += ( dy - THRESHOLD );
			}
			else if( vcnt1 > vcnt2 ){
				int		dy = vcnt1 - vcnt2;
				
				if( dy >= THRESHOLD )
					vcnt1 -= ( dy - THRESHOLD );
			}
			
			if( x != NULL )
				*x = hcnt1-0x30;
			if( y != NULL )
				*y = vcnt1-0x10;
			
			if( *x < 0 )
				*x = 0;
			else if( *x > 320 )
				*x = 320-1;
			
			if( *y < 0 )
				*y = 0;
			else if( *y > 224 )
				*y = 224-1;
		}
		else{
			*x = -100;
			*y = -100;
		}
	}
	else{
		*x = -100;
		*y = -100;
	}
}

