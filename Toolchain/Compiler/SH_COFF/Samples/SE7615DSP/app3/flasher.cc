/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/

#define LED   (*(volatile unsigned short *)(0x02200000))

#include "ioaddrs.h"

class flash
{
	private:
		unsigned short on;
		unsigned short off;
		unsigned long delay;
	public:
		flash();
		void FlashOn(void);
		void FlashOff(void);
		void Delay(void);
	
};


flash::flash()
{
	off = 0x0000;
	on  = 0xFFFF;
	delay = 50000;
}

void flash::FlashOn(void)
{
   LED = on;	
}

void flash::FlashOff(void)
{
	LED = off;
}

void flash::Delay(void)
{
	unsigned long l=0;
	for(l=0;l<delay;l++);
}

    flash Flash ;
int main( void )
{

	for(;;)
	{
		Flash.FlashOn();
		Flash.Delay();
		Flash.FlashOff();
		Flash.Delay();
	}
}
