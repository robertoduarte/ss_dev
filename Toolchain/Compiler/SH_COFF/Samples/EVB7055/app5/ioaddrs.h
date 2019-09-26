/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/
#ifndef _SH7055
#define _SH7055
#include "macros.h"

#define SCI0_SMR _DREF(_MPTR_(_VOL(_US(char)))(0xFFFFF000))
#define SCI0_BRR _DREF(_MPTR_(_VOL(_US(char)))(0xFFFFF001))
#define SCI0_SCR _DREF(_MPTR_(_VOL(_US(char)))(0xFFFFF002))
#define SCI0_TDR _DREF(_MPTR_(_VOL(_US(char)))(0xFFFFF003))
#define SCI0_SSR _DREF(_MPTR_(_VOL(_US(char)))(0xFFFFF004))
#define SCI0_RDR _DREF(_MPTR_(_VOL(_US(char)))(0xFFFFF005))
#define SCI0_SDCR _DREF(_MPTR_(_VOL(_US(char)))(0xFFFFF006))


#define SCI1_SMR _DREF(_MPTR_(_VOL(_US(char)))(0xFFFFF008))
#define SCI1_BRR _DREF(_MPTR_(_VOL(_US(char)))(0xFFFFF009))
#define SCI1_SCR _DREF(_MPTR_(_VOL(_US(char)))(0xFFFFF00A))
#define SCI1_TDR _DREF(_MPTR_(_VOL(_US(char)))(0xFFFFF00B))
#define SCI1_SSR _DREF(_MPTR_(_VOL(_US(char)))(0xFFFFF00C))
#define SCI1_RDR _DREF(_MPTR_(_VOL(_US(char)))(0xFFFFF00D))
#define SCI1_SDCR _DREF(_MPTR_(_VOL(_US(char)))(0xFFFFF00E))


#define UBC_BRCR  _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFEC0A))
#define MSTCR	  _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFF70A))
#define BCR1	  _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFEC20))
#define BCR2	  _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFEC22))
#define WCR	  _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFEC24))

#define PEIOR  _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFF750))
#define PECR   _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFF752))
#define PFIOR  _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFF748))
#define PFCRH  _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFF74A))
#define PFCRL  _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFF74C))
#define PHIOR  _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFF728))
#define PHCR   _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFF72A))

#define PAIOR _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFF720))
#define PACRH _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFF722))
#define PACRL _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFF724))
#define PADR  _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFF726))


#define PCIOR _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFF73A))
#define PCCR  _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFF73C))
#define PCDR  _DREF(_MPTR_(_VOL(_US(short)))(0xFFFFF73E))

#define IPRLK   _DREF(_MPTR_(_VOL(_US(short)))0xFFFFED14) /*7055 SCI0 priority reg. Address*/

#define SCR0 SCI0_SCR
#define SMR0 SCI0_SMR
#define SSR0 SCI0_SSR
#define TDR0 SCI0_TDR
#define RDR0 SCI0_RDR
#define BRR0 SCI0_BRR

#define SCR1 SCI1_SCR
#define SMR1 SCI1_SMR
#define SSR1 SCI1_SSR
#define TDR1 SCI1_TDR
#define RDR1 SCI1_RDR
#define BRR1 SCI1_BRR


/* Serial stats register bit values */
#define SSR_TDRE 0x80
#define SSR_RDRF 0x40
#define SSR_ORER 0x20
#define SSR_FER  0x10
#define SSR_PER  0x8
#define SSR_TEND 0x4
#define SSR_MPB  0x2
#define SSR_MPBT 0x1


#define B4800		129		/* +0.16 Error */
#define B9600		64		/* +0.16 Error */
#define B14400		42		/* +0.94% Error */
#define B19200		32    	/* -1.36% Error */
#define B28800		21		/* -1.36% Error */
#define B31250		19		/* +0.00% Error */
#define B38400		15  	/* error +1.73%  40Mhz clock rate the only possible for EVB */
#define B57600		10  	/* error +1.99%  40Mhz clock rate */


#endif //  _SH7055
