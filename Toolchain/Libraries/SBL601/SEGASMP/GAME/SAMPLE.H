#if	!defined( __SAMPLE_H )
	#define	__SMAPLE_H
	
	/****************************************
	*			include files				*
	****************************************/
	#include	"per_x.h"
	
	/****************************************
	*			define constants			*
	****************************************/
	enum	{
		SQ_INIT,
		SQ_TITLE,
		SQ_DEMO,
		SQ_LOGO,
		SQ_GAME,
		SQ_BOOT
	};
	
	#define	INTERRUPTS_PER_SECOND	60
	
	/****************************************
	*		declare object types			*
	****************************************/
	typedef	void	( *sequence_t )( void );
	
	/****************************************
	*		declare external objects		*
	****************************************/
	extern int			level[2];
	extern int			timer;
	extern trigger_t	trigger;
	extern trigger_t	triggerE;
	extern Uint16		demoflg;
	
	extern void	title( void );
	extern void	demo( void );
	extern void	logo( void );
	extern void	game( void );
	extern void	boot( void );
	
#endif
