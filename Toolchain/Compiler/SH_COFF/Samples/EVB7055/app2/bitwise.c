/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/


/*
Following bitwise operators to be tested.
1. >> right shift
2. << left shift
3. ^ exclusive or
4. | bitwise or
5. & bitwise and
6. ~ bitwise not

*/
#include "ioaddrs.h"

#define PMODE_DR				PADR		/* PMode port Data Register */
#define PMODE_SET				0x0004		/* PMode bit set */
#define PMODE_CLEAR				0xFFFB		/* PMode bit not set */

int __main(void) {}

char cArray[100] = {0,1,1,3,3,2,2,0,3,1,
					1,0,1,1,3,3,2,2,0,3,
					1,1,0,1,1,3,3,2,2,0,
					3,1,1,0,1,1,3,2,2,9,
					3,3,1,1,0,1,1,3,2,2,
					2,3,3,1,1,0,1,1,3,2,
					2,2,3,3,1,1,0,1,1,3,
					0,2,2,3,3,1,1,0,1,1,
					3,0,2,2,3,3,1,1,0,1,
					1,3,0,2,2,3,3,1,1,0,
				    };

void MySleep(int delay)
{
	long l = 1,l1 = 1;
	long l2=0;
	l = delay * 10000;

	for(l1=0;l1<l;l1++)
	{
	 	l2 = l2*2;
	}
}

int main (void)
{
  char c = 0x03;
  long count=0;
  long threshhold=50000;
  int i =0;
  int i1 = 0;
  char cMyChar = 0xFC;

  PMODE_DR &= PMODE_CLEAR;

  while (1)
  {
	  PADR = c; 	/*Both LED's OFF */
	  MySleep(250);
	  PADR = ~(c); 	/*Both LED's ON */ /*Use of ~ operator */
	  MySleep(250);
	  PADR = (c & 0x1); /*Only red LED's ON */ /*Use of & operator */
	  MySleep(250);
	  PADR = (c & 0x2); /*Only green LED's ON */ /*Use of & operator */
	  MySleep(250);
	  PADR = (c & (0x4 >> 2)); /*Only red LED's ON */ /*Use of >> operator */
	  MySleep(250);
	  PADR = (c & (0x1 << 1)); /*Only green LED's ON */ /*Use of << operator */
	  MySleep(250);
	  PADR = (c | 3 ); /*Both LED's OFF */ /*Use of | operator */
	  MySleep(250);
	  PADR = (c & 0x1 ); /*Red LED ON */ /*Use of & operator */
	  MySleep(250);
	  PADR ^= 0x3; /* Compliment the 2 LSB's */
	  MySleep(250);
	  PADR ^= 0x3; /* Compliment the 2 LSB's */
	  MySleep(2500);
  }
}
