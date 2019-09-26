/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include "ioaddrs.h"

int main( void) ;

void hw_initialise( void )
{
	unsigned long dummy=0xffff;

	*WCR1=0xa55aab7f;		/* for SH7615 */
	*BCR1=0xa55a63f1;		/* for SH7615 */
	*BCR2=0xa55a02f8;		/* for SH7615 */
	*BCR3=0xa55a0c01;		/* for SH7615 */
	*WCR2=0xa55a000b;		/* for SH7615 */
	*WCR3=0xa55a0010;		/* for SH7615 */
	*MCR=0xa55a9ddb;		/* for SH7615 */
	*RTCOR=0xa55a003a;		/* for SH7615 */
	*RTCNT=0xa55a0000;		/* for SH7615 */
	*RTCSR=0xa55a0008;		/* for SH7615 */
	SDMR=0xffff;			/* for SH7615 */
	dummy=*SD_BANK1;		/* SDRMA dummy read */
	dummy=*SD_BANK2;		/* SDRMA dummy read */
	dummy=*SD_BANK3;		/* SDRMA dummy read */
	dummy=*SD_BANK4;		/* SDRMA dummy read */

	*CCR&=0xfe;				/*Cache disable*/
	*CCR=0x10;				/*Cache purge*/
	*CCR=0x01;				/*Cache enable*/

	/* Initialize Serial port */
	*PBCR=0xAA80;
/*
	*SCSCR1=0x00;
	*SCFCR1=0x00;
	*SCSCR1=0x02;
	*SCSMR1=0x00;
	*SCBRR1=7;   // 38400 baud rate. This is useless..Baud rate is set from SW4
	*SCSCR1=0x72;
*/

	*IPRD=0x000f;
	*VCRL=0x5757;

};


#ifdef __cplusplus
}
#endif //__cplusplus
