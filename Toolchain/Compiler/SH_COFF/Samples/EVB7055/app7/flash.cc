/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/
#include "flash.h"
#include "ioaddrs.h"

#ifdef __cplusplus

extern long multiplier;

FLASH::FLASH()
{
	togval = 3;
	PADR = 1;
}

FLASH::FLASH(int val)
{
	togval = 3;
	PADR = (val & 3);
}

FLASH::FLASH(int rval,int gval)
{
	int i=0;
	togval = 3;
	i = (((rval & 1) << 1) | (gval & 1));
	PADR = (i & 3);
}

void FLASH::fDelay(unsigned long ul)
{
	ul *= multiplier;
	if(ul) while(ul--);
}

void FLASH::red(int val)
{
	val = (val != 0 ) ? 1 : 0;
	val = val << 1;
	PADR ^= val;
}

void FLASH::green(int val)
{
	val = (val != 0 ) ? 1 : 0;
	PADR ^= val;
}

void FLASH::tog()
{
	PADR ^= togval;
}

#endif // __cplusplus
