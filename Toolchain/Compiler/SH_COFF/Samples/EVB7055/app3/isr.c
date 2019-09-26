/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/
#include "ioaddrs.h"

unsigned long threshhold=500000l;

int __main(void) {}

#ifdef __cplusplus
extern "C" {
#endif


#pragma interrupt
void SW3_Int( void )
{
	threshhold = 900000l;
}

#pragma interrupt
void NMI_Int( void )
{
    threshhold = 400000l;
}

void  delay(unsigned long l)
{
 	if(l) while( l-- );
}

#ifdef __cplusplus
}		// extern "C"
#endif

int main (void)
{
  while(1)
  {
		delay(threshhold);
		PADR ^= 3;
   }
}
