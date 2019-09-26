/*----------------------------------------------------------------------*/
/*	Event Demo							*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"

void ss_main(void)
{
	slInitSystem(TV_320x224,NULL,1) ;
	slPrint("Sample program 10" , slLocate(9,2)) ;

	set_event();
	while(-1) {
		slExecuteEvent() ;
		slSynch() ;
	}
}
