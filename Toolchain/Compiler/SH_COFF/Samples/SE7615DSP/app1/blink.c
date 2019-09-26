/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#define LED   (*(volatile unsigned short *)(0x02200000))

unsigned int led_data = 0xFFFF ;

int __main(void) {}

int main( void )
{
	unsigned long threshhold = 0x00020000;
	unsigned long count = 0 ;
	unsigned long delay = 0 ;
	int temp = 0 ;

	LED = led_data ; // LEDs ON
	while ( 1 )
	{
		count++ ;
		if( count == threshhold )
		{
			led_data ^= 0xFFFF ;
			LED = led_data ; // Toggle the LEDs
			count = 0 ;
		}

	}
	return 0 ;
}
#ifdef __cplusplus
}
#endif //__cplusplus
