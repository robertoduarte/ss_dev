/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/

#include "7045map.h"

#define SSR SH2_704X_SCI_SSR0
#define TDR SH2_704X_SCI_TDR0
#define RDR SH2_704X_SCI_RDR0

#define TDRE SH2_704X_SCI_SSR_TDRE
#define RDRF SH2_704X_SCI_SSR_RDRF
#define PER  SH2_704X_SCI_SSR_PER
#define FER  SH2_704X_SCI_SSR_FER
#define ORER SH2_704X_SCI_SSR_ORER

int __main(void) {}

void gdb_putc ( char c );
char gdb_getc ( void );
void putstring (unsigned char *str);

void delay(unsigned long ul){if(ul) while(ul--);}

int count = 0;

unsigned char getchar (void)
{
  unsigned char ch1;
  while ((SCI1_SSR & 0x40) != 0x40)
  {
    /* ignore errors */
    if ((SCI1_SSR & 0x38) != 0x00)
      SCI1_SSR &= 0xC7;
  }
  ch1 = SCI1_RDR;
  SCI1_SSR &= 0xBF;
  return (ch1);
}

void put_char (unsigned char c)
{
  while ((SCI1_SSR & 0x80) != 0x80);
  SCI1_TDR = c;
  SCI1_SSR &= 0x7F;
}

/*   Stuffs a byte out the serial port. */

void gdb_putc ( char c )
{
  /* wait for the previous byte to go */
  while( !( SSR & TDRE ));

  /* send the byte, clear read flag */
  TDR = c;
  SSR &= ~TDRE;

  return;
}


/*   Blocks until a byte is received at the serial port.  */

char gdb_getc ( void )
{
  char c;

  /* clear any detected errors */
  SSR &= ~( PER | FER | ORER );

  /* wait for a byte */
  while( !( SSR & RDRF ));

  SH2_704X_IO_PEDR = 0x00;
  /* got one-- return it */
  c = RDR;
  SSR &= ~RDRF;

  return c;
}

void putstring (unsigned char *str)
{
  while (*str != '\0')
    gdb_putc(*str++);
}

int main (void)
{
	SH2_704X_IO_PEDR |= 0x0020; /* LED off, hi pin 13 */
	putstring("Initialised Serial port SCI0 for 38.4K \n\r");
	WDT_W_TCSR = 0xA53E;	/* start the timer */
	while(1)
	{
		SH2_704X_IO_PEDR ^= 0x0020;    //  Toggle green LED off
		putstring("In the main loop\n\r");
		delay(5000000);
	}
}
