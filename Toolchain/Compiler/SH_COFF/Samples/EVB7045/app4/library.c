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


void gdb_putc ( char c )
{
  /* wait for the previous byte to go */
  while( !( SSR & TDRE ));

  /* send the byte, clear read flag */
  TDR = c;
  SSR &= ~TDRE;

  return;
}


/*
  Blocks until a byte is received at the serial port.
 */
char gdb_getc ( void )
{
  char c;

  /* clear any detected errors */
  SSR &= ~( PER | FER | ORER );

  /* wait for a byte */
  while( !( SSR & RDRF ));

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
  	char c;
  	int i;
  	char cArr[100];
  	char cArr1[100];
  	char* cArray;
  	double d1,d2,d3,pi;

	SH2_704X_IO_PEDR |= 0x0020; /* LED off, hi pin 13 */

	putstring("Initialised SCI0 with 38.4K\r\n");
	putstring("Demonstrating use of Library functions ...\r\n");
  	putstring("Using sprintf ...\r\n");
	sprintf(cArr,"This string is written using sprintf....\r\n");
	putstring(&cArr[0]);
    putstring("Using strcpy.....\r\n");
	strcpy(cArr1,"This string is copied using strcpy...\r\n");
	putstring(&cArr1[0]);

	putstring("Allocating memory using malloc.....\r\n");
	cArray = (char *)malloc(100);
	putstring("Allocated 100 bytes.....\r\n");

	pi = 22.0/7.0;
	sprintf(cArray,"Approximate value of pi is %lf \r\n",pi);
	putstring(cArray);

	sprintf(cArray,"Suqare root of pi %lf \r\n",sqrt(pi));
	putstring(cArray);

	sprintf(cArray,"LOG to the base 10 of pi %lf \r\n",log10(pi));
	putstring(cArray);

	sprintf(cArr,"Deallocating memory ....\r\n");
	free(cArray);
	putstring(cArr);

	sprintf(cArr,"Deallocated memory ....\r\n");
	putstring(cArr);


	while(1)
	{
		c = gdb_getc();
		gdb_putc(c);
		SH2_704X_IO_PEDR ^= 0x0020;    //  Toggle green LED off
	}
}
