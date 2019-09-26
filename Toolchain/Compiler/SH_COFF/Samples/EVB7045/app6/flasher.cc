/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/

#include "register.h"
#include "7045map.h"
#include "flash.h"

#define SSR SH2_704X_SCI_SSR0
#define TDR SH2_704X_SCI_TDR0
#define RDR SH2_704X_SCI_RDR0

#define TDRE SH2_704X_SCI_SSR_TDRE
#define RDRF SH2_704X_SCI_SSR_RDRF
#define PER  SH2_704X_SCI_SSR_PER
#define FER  SH2_704X_SCI_SSR_FER
#define ORER SH2_704X_SCI_SSR_ORER

#ifdef __cplusplus
extern "C" {
#endif

long multiplier = 100000l;

void gdb_putc ( char c );
char gdb_getc ( void );
void putstring (const char *str);
void delay(unsigned long ul){if(ul) while(ul--);}

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

void putchar (unsigned char c)
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
    putchar("0123456789ABCDEF"[((c & 0xF0) >> 4)]);
    putchar("0123456789ABCDEF"[(c & 0x0F)]);
    putchar(' ');
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

/*
  Stuffs a byte out the serial port.
 */
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

  SH2_704X_IO_PEDR = 0x00;
  /* got one-- return it */
  c = RDR;
  SSR &= ~RDRF;

  return c;
}

void putstring (const char *str)
{
  while (*str != '\0')
    gdb_putc(*str++);
}

#ifdef __cplusplus
} // extern "C" {
#endif

FLASH myflash;

int main (void)
{
    char c;
    int delay=100;

    SH2_704X_IO_PEDR |= 0x0020; /* LED off, hi pin 13 */
	putstring("Initialised SCI0 for 38.4K baud\r\n");
	putstring("Using C++ program \r\n");
	putstring("Enter blink rate\r\n\r\n");
	putstring("A: 1\r\n");
	putstring("B: 5\r\n");
	putstring("C: 10\r\n");
	putstring("D: 50\r\n");
	putstring("E: 100\r\n");
	putstring("\r\n");

	c = gdb_getc();
	
	putstring("You Selected ");
	
	switch(c)
	{
		case 'a':
		case 'A':
				 delay = 1;
				 putstring("A \r\n");
				 break;
		case 'b':
		case 'B':
				 delay = 5;
				 putstring("B \r\n");
				 break;
		case 'c':
		case 'C':
				 delay = 10;
				 putstring("C \r\n");
		 		 break;
		case 'd':
		case 'D':
				 delay = 50;
				 putstring("D \r\n");
				 break;				 
		case 'e':
		case 'E':
				 delay = 100;
				 putstring("E \r\n");
				 break;						 
		default:
			delay = 100;
			break;
	}
	
	while(1)
	{
		myflash.tog();
		myflash.fDelay(delay);
	}
}
