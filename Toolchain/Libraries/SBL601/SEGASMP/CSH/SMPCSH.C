#include "sega_xpt.h"
#include "sega_csh.h"

void main(void)
{
	CSH_Init(CSH_4WAY);
	CSH_AllClr();
	CSH_Purge((volatile void *)0x06000008, (Uint32)0x10);

	while(1)
		;
}
