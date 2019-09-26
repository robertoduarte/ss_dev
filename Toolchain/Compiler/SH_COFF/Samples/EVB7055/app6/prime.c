/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/

#include <string.h>
#include <math.h>
#include <stdio.h>

#include "ioaddrs.h"

#define TEST(f,x)	(*(f+(x)/16)&(1<<(((x)%16L)/2)))
#define SET(f,x)	*(f+(x)/16)|=1<<(((x)%16L)/2)

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

void printsci0(char * arr)
{
	int i,len;
	len = strlen(arr);
	for(i=0;i<len;i++) sci0_putc(arr[i]);
}


int main (void)
{
	char szMessage[100];
	char rec;
    unsigned char *feld=NULL, *zzz;
    unsigned long teste=1, max, mom, hits=1, count, alloc, s=0, e=1;
    char msg[1000];
    unsigned long ulimit=200000l;

	Sci0Init();
	Sci1Init();

	sprintf(szMessage,"\r\nInitialised SCI0 for 38.4K.....\r\n");
	printsci0(szMessage);

	sprintf(szMessage,"Started searching for prime nos till %ld .....\r\n",max);
	printsci0(szMessage);

   	/* max = 14010000L; */
   	max = 10000L + ulimit;

	while (feld==NULL)
        	zzz = feld = (unsigned char *)malloc (alloc=(((max-=10000L)>>4)+1L));

  	for (count=0; count<alloc; count++) *zzz++ = 0x00;


	while ((teste+=2) < max)
	{
        if (!TEST(feld, teste))
        {
            if  (++hits%2000L==0)
            {
				sprintf (msg," %ld. prime number", hits);
				printsci0(msg);
			} /* if */
            for (mom=3L*teste; mom<max; mom+=teste<<1)
            {
            	SET (feld, mom);
			} /* for */
        } /* if */
	} /* while */

	sprintf (msg," %ld prime numbers found\n",hits);
	printsci0(msg);

	while (s<e)
	{
		s = 1;
		e = ulimit;
		count=s-2;
		if (s%2==0)
			count++;
		while ((count+=2)<e)
		{
			if (!TEST(feld,count))
			{
				sprintf (msg,"%7d ", count);
				printsci0(msg);
			}
		}
		break;
	}
	free (feld);

	sprintf (msg,"\r\nFinished searching of prime nos\r\n", count);
	printsci0(msg);

	sprintf (msg,"Total %ld prime nos found \r\n", hits);
	printsci0(msg);

	while(1)
	{
		rec = sci0_getc();
		sci0_putc(rec);
		PADR ^= 3;
	}
}
