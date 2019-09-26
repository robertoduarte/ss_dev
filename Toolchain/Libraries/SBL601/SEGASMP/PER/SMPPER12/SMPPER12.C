/****************************************
*			include files				*
****************************************/
#define	_SPR2_
#include	"sega_spr.h"

#include	"machine.h"
#include	"sega_int.h"
#include	"sega_scl.h"

#include	"image.h"

#include	"per_x.h"
#include	"smpc.h"
#include	"vdp2.h"

/****************************************
*		declare public objects			*
****************************************/
Uint16	tvstat;

/****************************************
*		declare private objects			*
****************************************/
static SysPort	*__port;

/****************************************
*			declare functions			*
****************************************/
void	_spr2_transfercommand( void );
void	_spr2_transferimage( void	*image, Uint32	size );
void	_spr2_initialize( void );
void	_spr2_setspritecoord( int	x, int	y );
void	_spr2_getspritecoord( int	*x, int	*y );
void	_spr2_setscreencolor( int	color );
void	move_cursor( const SysPort	*port, int	*x, int	*y );
trigger_t	get_trigger( const SysPort	*port );

extern	void scl_main(void);

static void	smpper12( SysPort	*port ){
	Uint16	color = 0;
	int		x,y;
	
	_spr2_getspritecoord( &x, &y );
	for(;;){
#if	1
		trigger_t	trigger = get_trigger( port );
		
		if( color != 0 ){
			move_cursor( port, &x, &y );
			_spr2_setspritecoord( x, y );
		}
		
		_spr2_setscreencolor( color );
		_spr2_transfercommand();
		
		if( trigger & (TRG_C|TRG_START))
			color = 0xffff;
		else
			color = 0x0000;
		
#else
		move_cursor( port, &x, &y );
		_spr2_setscreencolor( 0xffff );
		_spr2_setspritecoord( x, y );
		_spr2_transfercommand();
#endif
		SCL_DisplayFrame();
	}
}

static void	_intr_v_blank_in( void ){
	SCL_VblankStart();
}

static void	_intr_v_blank_out( void ){
	SCL_VblankEnd();
	
	SMPC_IOSEL = 0;
	SMPC_EXLE  = 0;
	SMPC_DDR(0) = 0;
	SMPC_DDR(1) = 0;
	
	VDP2_EXTEN = VDP2_EXTEN_EXLTEN;
	PER_GetPort( __port );
	
	tvstat = VDP2_TVSTAT;
}

int	main( void ){
	SCL_Vdp2Init();
	SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
					SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,7);
	SCL_SetSpriteMode(SCL_TYPE1,SCL_MIX,SCL_SP_WINDOW);
	SCL_SetColRamMode(SCL_CRM24_1024);
	
	__port = PER_OpenPort();
	
	INT_ChgMsk( INT_MSK_NULL, INT_MSK_VBLK_IN|INT_MSK_VBLK_OUT );
	INT_SetScuFunc( INT_SCU_VBLK_IN,  _intr_v_blank_in  );
	INT_SetScuFunc( INT_SCU_VBLK_OUT, _intr_v_blank_out );
	INT_ChgMsk( INT_MSK_VBLK_IN|INT_MSK_VBLK_OUT, INT_MSK_NULL );

	set_imask(0);
	
	_spr2_initialize();
	_spr2_transfercommand();
	_spr2_transferimage( cursor_image, sizeof( cursor_image ));
	
	scl_main();
	SCL_SetFrameInterval( 1 );
	
	smpper12( __port );
	
	return	EXIT_SUCCESS;
}
