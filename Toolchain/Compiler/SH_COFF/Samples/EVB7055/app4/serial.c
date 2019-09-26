/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/

#include <string.h>
#include <stdio.h>

#include "ioaddrs.h"

int __main(void) {}

void MyDelay(long l)
{
	while(l--);
}

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*fp) (void);
void start (void);                      /* Startup code (in start.s)  */
void SW3_Int( void );
void NMI_Int( void );

#ifdef __cplusplus
}
#endif

long multiplier = 10;

#pragma interrupt
#ifdef __cplusplus
extern "C" {
#endif

void SW3_Int( void )
{
	if(multiplier)
		multiplier *= 2;
	else
		multiplier = 1;
}

#pragma interrupt
void NMI_Int( void )
{
	if(multiplier > 1)
		multiplier -- ;
	else
		multiplier = 10;
}


#ifdef __cplusplus

} // extern "C"

#endif

void Sci0Init( void )
{
	/* Clear TE & RE bits to 0 in SCR */
	SCR0 = 0;
	SSR0 &= ~ (SSR_PER + SSR_FER + SSR_ORER);          /* Clear Rx error bits */

	SMR0 = 0;

	BRR0 = B38400 ;
	SCR0 = 0x30 ; // TE, RE, internal clock
}

void Sci1Init( void )
{
	/* Clear TE & RE bits to 0 in SCR */
	SCR1 = 0;
	SSR1 &= ~ (SSR_PER + SSR_FER + SSR_ORER);          /* Clear Rx error bits */

	SMR1 = 0;

	BRR1 = B38400 ;
	SCR1 = 0x30 ; // TE, RE, internal clock
}

void sci0_putc ( char c )
{
  /* wait for the previous byte to go */
  while(! (SSR0 & SSR_TDRE));  	/* Wait until TDRE = 1  */
  /* send the byte, clear read flag */
  TDR0 = c ;   				/* Set transmit data    */
  SSR0 &= ~ SSR_TDRE;               	/* Clear TDRE bit       */

  return;
}

void sci1_putc ( char c )
{
  /* wait for the previous byte to go */
  while(! (SSR1 & SSR_TDRE));  	/* Wait until TDRE = 1  */
  /* send the byte, clear read flag */
  TDR1 = c ;   				/* Set transmit data    */
  SSR1 &= ~ SSR_TDRE;               	/* Clear TDRE bit       */

  return;
}


/* Blocks until a byte is received at the serial port. */
char sci0_getc ( void )
{
  char c;

  /* clear any detected errors */
  SSR0 &= ~ (SSR_ORER | SSR_FER  | SSR_PER);		/* Clear error bits */

  /* wait for a byte */
  while( !( SSR0 & SSR_RDRF ));

  /* got one-- return it */
  c = RDR0;
  SSR0 &= ~SSR_RDRF;

  return c;
}


/* Blocks until a byte is received at the serial port. */
char sci1_getc ( void )
{
  char c;

  /* clear any detected errors */
  SSR1 &= ~ (SSR_ORER | SSR_FER  | SSR_PER);		/* Clear error bits */

  /* wait for a byte */
  while( !( SSR1 & SSR_RDRF ));

  /* got one-- return it */
  c = RDR1;
  SSR1 &= ~SSR_RDRF;

  return c;

}

/////////////////////////////////////////////////////////////////

#ifdef __cplusplus
FLASH f1(3);
#endif

int main (void)
{
	char szM0[500];
	char szM1[500];
	int i=0,len;
	char rec;

	Sci0Init();
	Sci1Init();
	sprintf(szM0,"\r\nInitialised SCI0 for 38.4K.....\r\n");
	sprintf(szM1,"\r\nInitialised SCI1 for 38.4K.....\r\n");
	len= strlen(szM0);
	for(i=0;i<len;i++) sci0_putc(szM0[i]);
	len= strlen(szM1);
	for(i=0;i<len;i++) sci1_putc(szM1[i]);

#ifndef __cplusplus
	while(1)
	{
		rec = sci0_getc();
		sci0_putc(rec);
		PADR ^= 3;
	}
#else
	while(1)
	{
		rec = sci1_getc();
		sci0_putc(rec);
	}
#endif
}
