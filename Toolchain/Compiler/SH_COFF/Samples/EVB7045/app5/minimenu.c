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

void CopyToRAM(void)
{
  int i;
  char *psrc = (char *)(0x0401000);
  char *pdes = (char *)(0xFFFFF100);
  int size = 0x821;
  for (i=0;i<size;i++)
    *pdes++ = *psrc++;
}

void CopyStartup(void)
{
  int i;
  char *psrc = (char *)(0x6000);
  char *pdes = (char *)(0xFFFFFe00);
  int size = 0x41;
  for (i=0;i<size;i++)
    *pdes++ = *psrc++;
}

void RunUSER (void)
{
  SH2_704X_IO_PEDR |= 0x0020; /* LED off */
  putstring("\n\rUser Mode Begins");
  putstring("\n\rSoory.. Downloader implemention has not been done");

  /*
  Downloader();*/
}

void ReadRAM(unsigned long adrs)
{
  int i;
  unsigned char c;
  unsigned char *pc = (unsigned char *)adrs;
  for (i=0; i<0x100; i++)
  {
    c = *pc;
    put_char("0123456789ABCDEF"[((c & 0xF0) >> 4)]);
    put_char("0123456789ABCDEF"[(c & 0x0F)]);
    put_char(' ');
    pc++;
  }
}

void FillExternalRAM (void)
{
  unsigned char i;
  unsigned char *external_ram = (unsigned char *)0x400000;
  for (i=0; i<0xFF; i++)
    *external_ram++ = i;
}

/*   Stuffs a byte out the serial port.  */
void gdb_putc ( char c )
{
  /* wait for the previous byte to go */
  while( !( SSR & TDRE ));

  /* send the byte, clear read flag */
  TDR = c;
  SSR &= ~TDRE;

  return;
}


/* Blocks until a byte is received at the serial port.  */
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
    char c;
    int i;
    char exitflag=0;

    SH2_704X_IO_PEDR |= 0x0020; /* LED off, hi pin 13 */
	putstring("\r\nSample application demonstrating use of Menu on Serial port\r\n\r\n");
	while(!exitflag)
	{
		delay(1000000);
		SH2_704X_IO_PEDR ^= 0x0020;
		delay(1000000);
		putstring("\r\nWelcome\r\n\r\n");
		putstring("A. Menu A\r\n");
		putstring("B. Menu B\r\n");
		putstring("C. Menu C\r\n");
		putstring("D. Menu D\r\n");
		putstring("E. Menu E\r\n");
		putstring("Z. Exit Application\r\n");
		putstring("\r\nEnter Selection: \r\n");
		c = gdb_getc();
		putstring("You selected  ");
		switch(c)
		{
			case 0:
					putstring("timeout \r\n");
					break;
			case 'a':
			case 'A':
					putstring("A. \r\n");
					break;
			case 'b':
			case 'B':
					putstring("B. \r\n");
					break;
			case 'c':
			case 'C':
					putstring("C. \r\n");
					break;
			case 'd':
			case 'D':
					putstring("D. \r\n");
					break;
			case 'e':
			case 'E':
					putstring("E. \r\n");
					break;
			case 'z':
			case 'Z':
					exitflag = 1;
					break;
			default:
					putstring("Invalid Option \r\n");
					break;
		}
	}
	putstring("\r\nExiting Application\r\n");
	while(1)
	{
		delay(1000000);
		SH2_704X_IO_PEDR ^= 0x0020;
	}
}
