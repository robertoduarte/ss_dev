/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#define LED   (*(volatile unsigned short *)(0xB0C00000))

int __main(void) {}

unsigned int led_data = 0x0001 ;
int main( void )
{
	unsigned long threshhold = 150000;
	unsigned long count = 0 ;
	unsigned long delay = 0 ;
	int temp = 0 ;

	while ( 1 )
	{
		led_data = 0x8000; //Set first LED in upper word
		for(count=0;count <8; count++)
		{
			LED = led_data ; // set LED's
			led_data = led_data >> 1 ;
			for(delay = 0; delay<threshhold; delay++)
			{
				temp = temp+1;
			}
		}
	}
	return 0 ;
}
#ifdef __cplusplus
}
#endif //__cplusplus
