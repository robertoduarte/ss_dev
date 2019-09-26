/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/
#include "ioaddrs.h"

/*
All registeres are 32 bit registeres
For all the initial values refer 651 of 900 page of the hardware manual of the 7055

volatile:
The volatile keyword is a type qualifier used to declare that an object
can be modified in the program by something other than statements,
such as the operating system, the hardware, or a concurrently executing thread

*/

int __main(void) {}

long flashrate = 500000l;

void delay(unsigned long d)
{
	if(d) while(d--);
}

int main (void)
{
  while(1)
  {
  	delay(flashrate);
  	PADR ^= 3; /* Toggle lower 2 bits */
  }
}
