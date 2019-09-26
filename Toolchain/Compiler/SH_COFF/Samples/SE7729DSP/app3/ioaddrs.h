/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/

#ifndef	_H_7729R
#define	_H_7729R
#include "macros.h"

/************************/
/*         LED          */
/************************/
#define	LEDREG	_DREF(_MPTR_(_VOL(_US(short)))0xB0C00000)

/************************/
/*       3.MMU          */
/************************/
#define	PTEH	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFFF0)
#define	PTEL	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFFF4)
#define	TTB	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFFF8)
#define	TEA	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFFFC)
#define	MMUCR	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFFE0)

/************************/
/*       4.EXP          */
/************************/
#define	TRA	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFFD0)
#define	EXPEVT	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFFD4)
#define	INTEVT	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFFD8)
#define	INTEVT2	_DREF(_MPTR_(_VOL(_US(long)))0xA4000000)

/**************************/
/*       5.CACHE          */
/**************************/
#define	CCR	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFFEC)
#define	CCR2	_DREF(_MPTR_(_VOL(_US(long)))0xA40000B0)

/**************************/
/*       6.X/YMemory      */
/**************************/


/**************************/
/*       7.INTC           */
/**************************/
#define	ICR0	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFEE0)
#define	ICR1	_DREF(_MPTR_(_VOL(_US(short)))0xA4000010)
#define	ICR2	_DREF(_MPTR_(_VOL(_US(short)))0xA4000012)
#define	PINTER	_DREF(_MPTR_(_VOL(_US(short)))0xA4000014)
#define	IPRA	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFEE2)
#define	IPRB	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFEE4)
#define	IPRC	_DREF(_MPTR_(_VOL(_US(short)))0xA4000016)
#define	IPRD	_DREF(_MPTR_(_VOL(_US(short)))0xA4000018)
#define	IPRE	_DREF(_MPTR_(_VOL(_US(short)))0xA400001A)
#define	IRR0	_DREF(_MPTR_(_VOL(_US(char))0xA4000004)
#define	IRR1	_DREF(_MPTR_(_VOL(_US(char))0xA4000006)
#define	IRR2	_DREF(_MPTR_(_VOL(_US(char))0xA4000008)

/**************************/
/*       8.UBC            */
/**************************/
#define	BARA	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFFB0)
#define	BAMRA	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFFB4)
#define	BBRA	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFFB8)

#define	BARB	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFFA0)
#define	BAMRB	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFFA4)
#define	BBRB	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFFA8)
#define	BDRB	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFF90)
#define	BDMRB	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFF94)
#define	BRCR	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFF98)

#define	BETR	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF9C)
#define	BRSR	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFFAC)
#define	BRDR	_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFFBC)

#define	BASRA	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFFE4)
#define	BASRB	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFFE8)

/**************************/
/*       9.Low Power      */
/**************************/
#define	STBCR	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFF82)
#define	STBCR2	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFF88)

/**************************/
/*      10.CPG            */
/**************************/
#define	FRQCR	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF80)

#define	WTCNT_R	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFF84)
#define	WTCNT_W	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF84)
#define	WTCSR_R	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFF86)
#define	WTCSR_W	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF86)

/**************************/
/*      11.BSC            */
/**************************/
#define	BCR1	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF60)
#define	BCR2	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF62)
#define	WCR1	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF64)
#define	WCR2	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF66)
#define	MCR	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF68)
#define	PCR	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF6C)
#define	RTCSR	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF6E)
#define	RTCNT	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF70)
#define	RTCOR	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF72)
#define	RFCR	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF74)

#define	SDMR	_DREF(_MPTR_(_VOL(_US(char)))0xFFFFE880)

#define	MCSCR0	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF50)
#define	MCSCR1	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF52)
#define	MCSCR2	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF54)
#define	MCSCR3	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF56)
#define	MCSCR4	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF58)
#define	MCSCR5	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF5A)
#define	MCSCR6	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF5C)
#define	MCSCR7	_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFF5E)

/**************************/
/*      12.DMAC           */
/**************************/
/*
	Chanel 0
*/
#define	SAR0	_DREF(_MPTR_(_VOL(_US(long)))0xA4000020)
#define	DAR0	_DREF(_MPTR_(_VOL(_US(long)))0xA4000024)
#define	DMATCR0	_DREF(_MPTR_(_VOL(_US(long)))0xA4000028)
#define	CHCR0	_DREF(_MPTR_(_VOL(_US(long)))0xA400002C)

/*----------------
	Chanel 1
------------------*/
#define	SAR1	_DREF(_MPTR_(_VOL(_US(long)))0xA4000030)
#define	DAR1	_DREF(_MPTR_(_VOL(_US(long)))0xA4000034)
#define	DMATCR1	_DREF(_MPTR_(_VOL(_US(long)))0xA4000038)
#define	CHCR1	_DREF(_MPTR_(_VOL(_US(long)))0xA400003C)

/*---------------
	Chanel 2
-----------------*/
#define	SAR2	_DREF(_MPTR_(_VOL(_US(long)))0xA4000040)
#define	DAR2	_DREF(_MPTR_(_VOL(_US(long)))0xA4000044)
#define	DMATCR2	_DREF(_MPTR_(_VOL(_US(long)))0xA4000048)
#define	CHCR2	_DREF(_MPTR_(_VOL(_US(long)))0xA400004C)

/*----------------
	Chanel 3
------------------*/
#define	SAR3		_DREF(_MPTR_(_VOL(_US(long)))0xA4000050)
#define	DAR3		_DREF(_MPTR_(_VOL(_US(long)))0xA4000054)
#define	DMATCR3		_DREF(_MPTR_(_VOL(_US(long)))0xA4000058)
#define	CHCR3		_DREF(_MPTR_(_VOL(_US(long)))0xA400005C)

/*--------------
	Common
----------------*/
#define	DMAOR		_DREF(_MPTR_(_VOL(_US(short)))0xA4000060)


/**************************/
/*      13.TMU            */
/**************************/
/*-------------
	Common
---------------*/
#define	TOCR		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFE90)
#define	TSTR		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFE92)

/*---------------
	Chanel 0
-----------------*/
#define	TCOR0		_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFE94)
#define	TCNT0		_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFE98)
#define	TCR0		_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFE9C)

/*----------------
	Chanel 1
------------------*/
#define	TCOR1		_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFEA0)
#define	TCNT1		_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFEA4)
#define	TCR1		_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFEA8)

/*----------------
	Chanel 2
------------------*/
#define	TCOR2		_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFEAC)
#define	TCNT2		_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFEB0)
#define	TCR2		_DREF(_MPTR_(_VOL(_US(short)))0xFFFFFEB4)
#define	TCPR2		_DREF(_MPTR_(_VOL(_US(long)))0xFFFFFEB8)

/**************************/
/*      14.RTC            */
/**************************/
#define	R64CNT		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFEC0)
#define	RSECCNT		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFEC2)
#define	RMINCNT		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFEC4)
#define	RHRCNT		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFEC6)
#define	RWKCNT		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFEC8)
#define	RDAYCNT		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFECA)
#define	RMONCNT		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFECC)

#define	RYRCNT		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFECE)
#define	RSECAR		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFED0)
#define	RMINAR		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFED2)
#define	RHRAR		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFED4)

#define	RWKAR		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFED6)
#define	RDAYAR		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFED8)
#define	RMONAR		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFEDA)
#define	RCR1		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFEDC)
#define	RCR2		_DREF(_MPTR_(_VOL(_US(char))0xFFFFFEDE)

/**************************/
/*      15.SCI            */
/**************************/
#define	SCSMR	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFE80)
#define	SCBRR	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFE82)
#define	SCSCR	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFE84)
#define	SCTDR	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFE86)
#define SCSSR	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFE88)
#define	SCRDR	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFE8A)

/**************************/
/*      16.SmartC         */
/**************************/
#define	SMART_SCSMR	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFE80)
#define	SMART_SCBRR	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFE82)
#define	SMART_SCSCR	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFE84)
#define	SMART_SCTDR	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFE86)
#define SMART_SCSSR	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFE88)
#define	SMART_SCRDR	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFE8A)
#define	SMART_SCSCMR	_DREF(_MPTR_(_VOL(_US(char))0xFFFFFE8C)

/**************************/
/*      17.SCIF2          */
/**************************/
#define	SCSMR2		_DREF(_MPTR_(_VOL(_US(char))0xA4000150)
#define	SCBRR2		_DREF(_MPTR_(_VOL(_US(char))0xA4000152)
#define	SCSCR2		_DREF(_MPTR_(_VOL(_US(char))0xA4000154)
#define	SCSFDR2		_DREF(_MPTR_(_VOL(_US(char))0xA4000156)
#define	SCSSR2		_DREF(_MPTR_(_VOL(_US(short)))0xA4000158)
#define	SCFRDR2		_DREF(_MPTR_(_VOL(_US(char))0xA400015A)
#define	SCFCR2		_DREF(_MPTR_(_VOL(_US(char))0xA400015C)
#define	SCFDR2		_DREF(_MPTR_(_VOL(_US(short)))0xA400015E)

/**************************/
/*      18.IrDA           */
/**************************/
#define	SCSMR1		_DREF(_MPTR_(_VOL(_US(char))0xA4000150)
#define	SCBRR1		_DREF(_MPTR_(_VOL(_US(char))0xA4000152)
#define	SCSCR1		_DREF(_MPTR_(_VOL(_US(char))0xA4000154)
#define	SCFTDR1		_DREF(_MPTR_(_VOL(_US(char))0xA4000156)
#define	SCSSR1		_DREF(_MPTR_(_VOL(_US(short)))0xA4000158)
#define	SCFRDR1		_DREF(_MPTR_(_VOL(_US(char))0xA400015A)
#define	SCFCR1		_DREF(_MPTR_(_VOL(_US(char))0xA400015C)
#define	SCFDR1		_DREF(_MPTR_(_VOL(_US(short)))0xA400015E)

/**************************/
/*      19.IOMPX          */
/**************************/
#define	PACR		_DREF(_MPTR_(_VOL(_US(short)))0xA4000100)
#define	PBCR		_DREF(_MPTR_(_VOL(_US(short)))0xA4000102)
#define	PCCR		_DREF(_MPTR_(_VOL(_US(short)))0xA4000104)
#define	PDCR		_DREF(_MPTR_(_VOL(_US(short)))0xA4000106)
#define	PECR		_DREF(_MPTR_(_VOL(_US(short)))0xA4000108)
#define	PFCR		_DREF(_MPTR_(_VOL(_US(short)))0xA400010A)
#define	PGCR		_DREF(_MPTR_(_VOL(_US(short)))0xA400010C)
#define	PHCR		_DREF(_MPTR_(_VOL(_US(short)))0xA400010E)
#define	PJCR		_DREF(_MPTR_(_VOL(_US(short)))0xA4000110)
#define	PKCR		_DREF(_MPTR_(_VOL(_US(short)))0xA4000112)
#define	PLCR		_DREF(_MPTR_(_VOL(_US(short)))0xA4000114)
#define SCPCR		_DREF(_MPTR_(_VOL(_US(short)))0x04000116)

/**************************/
/*      20.I/O PORT       */
/**************************/
#define	PADR		_DREF(_MPTR_(_VOL(_US(char))0xA4000120)
#define	PBDR		_DREF(_MPTR_(_VOL(_US(char))0xA4000122)
#define	PCDR		_DREF(_MPTR_(_VOL(_US(char))0xA4000124)
#define	PDDR		_DREF(_MPTR_(_VOL(_US(char))0xA4000126)
#define	PEDR		_DREF(_MPTR_(_VOL(_US(char))0xA4000128)
#define	PFDR		_DREF(_MPTR_(_VOL(_US(char))0xA400012A)
#define	PGDR		_DREF(_MPTR_(_VOL(_US(char))0xA400012C)
#define	PHDR		_DREF(_MPTR_(_VOL(_US(char))0xA400012E)
#define	PJDR		_DREF(_MPTR_(_VOL(_US(char))0xA4000130)
#define	PKDR		_DREF(_MPTR_(_VOL(_US(char))0xA4000132)
#define	PLDR		_DREF(_MPTR_(_VOL(_US(char))0xA4000134)
#define	SCPDR		_DREF(_MPTR_(_VOL(_US(char))0xA4000136)

/**************************/
/*      21.A/D            */
/**************************/
#define	ADDRAH		_DREF(_MPTR_(_VOL(_US(char))0xA4000080)
#define	ADDRAL		_DREF(_MPTR_(_VOL(_US(char))0xA4000082)
#define	ADDRBH		_DREF(_MPTR_(_VOL(_US(char))0xA4000084)
#define	ADDRBL		_DREF(_MPTR_(_VOL(_US(char))0xA4000086)
#define	ADDRCH		_DREF(_MPTR_(_VOL(_US(char))0xA4000088)
#define	ADDRCL		_DREF(_MPTR_(_VOL(_US(char))0xA400008A)
#define	ADDRDH		_DREF(_MPTR_(_VOL(_US(char))0xA400008C)
#define	ADDRDL		_DREF(_MPTR_(_VOL(_US(char))0xA400008E)
#define	ADCSR		_DREF(_MPTR_(_VOL(_US(char))0xA4000090)
#define	ADCR		_DREF(_MPTR_(_VOL(_US(char))0xA4000092)

/**************************/
/*      22.D/A            */
/**************************/
#define	DADR0		_DREF(_MPTR_(_VOL(_US(char))0xA40000A0)
#define	DADR1		_DREF(_MPTR_(_VOL(_US(char))0xA40000A2)
#define	DACR		_DREF(_MPTR_(_VOL(_US(char))0xA40000A4)

/**************************/
/*      23.HUDI           */
/**************************/
#define	SDBPR		()
#define	SDIR		_DREF(_MPTR_(_VOL(_US(short)))0xA4000200)
#define	SDBSR		()

#endif
