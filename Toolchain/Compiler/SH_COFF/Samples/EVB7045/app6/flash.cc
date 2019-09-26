/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/
#include "flash.h"
#include "7045map.h"

#ifdef __cplusplus

extern long multiplier;

FLASH::FLASH()
{
	togval = 0x0020;
	SH2_704X_IO_PEDR = 1;
}

FLASH::FLASH(int val)
{
	togval = 0x0020;
	SH2_704X_IO_PEDR = (val & 3);
}

void FLASH::fDelay(unsigned long ul)
{
	ul *= multiplier;
	if(ul) while(ul--);
}


void FLASH::tog()
{
	SH2_704X_IO_PEDR ^= togval;
}

#endif // __cplusplus
