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

#define	CCR_VALUE	(0x00000003)	/* Cache write through mode	*/
#define	FRQCR_VALUE	(0xA101)

int main( void) ;

void hw_initialise( void )
{
	BCR1 = 0x0008;
	BCR2 = 0x2EF0;	/* bus width  A6:16 A5:32 A4:16 A3:32 A2:16		*/
	WCR1 = 0x0C30;	/* idle cycle A6:1 A5:3  A4:1 A3:1 A2:3 A0:1	*/
	WCR2 = 0xBEDD;	/* wait cycle A6:6 A5:10 A4:6 A3:2 A2:3 A0:6	*/
	MCR  = 0x002C;
	PCR   = 0x0000;
	RTCSR = 0xA508;
	RTCNT = 0xA500;
	RTCOR = 0xA580;
	RFCR  = 0xa400;

	while( RFCR < 8 );	/* Auto Reflesh cycle 8 count	*/

	SDMR = 0;

	CCR = (CCR_VALUE | 0x00000008);	/* cache flash ON	*/

	WTCSR_W = 0xA502;
	FRQCR = FRQCR_VALUE;	/* I:B:P=?:?:?	*/

/*
	SCSCR2 = 0x03;
	SCFCR2 = 0x06;
	SCSMR2 = 0x00;
	SCFCR2 = 0x30;
	SCSCR2 = 0x73;
*/

	IPRA = 0x0000;
	IPRB = 0x0000;
	IPRC = 0x0000;
	IPRD = 0x0000;
	IPRE = 0x00F0;		/* SCIF Level 15	*/
	ICR1   = 0x4000;	/* IRLS0-3 Disable	*//* 01.04.11 */

	SCPCR &= 0xF0FF;	/* SCIF.TXD2,RXD2 On	*/

};

#ifdef __cplusplus
}
#endif //__cplusplus
