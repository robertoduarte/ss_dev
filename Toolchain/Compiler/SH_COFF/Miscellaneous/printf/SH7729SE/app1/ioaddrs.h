/****************************************************************
KPIT Infosystems Ltd, Pune, India. - 03-Dec-2001.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/

#ifndef	_H_7729R
#define	_H_7729R

/************************/
/*         LED          */
/************************/
#define	LEDREG	(*(volatile unsigned short *)0xB0C00000)

/************************/
/*       3.MMU          */
/************************/
#define	PTEH	(*(volatile unsigned long *)0xFFFFFFF0)
#define	PTEL	(*(volatile unsigned long *)0xFFFFFFF4)
#define	TTB		(*(volatile unsigned long *)0xFFFFFFF8)
#define	TEA		(*(volatile unsigned long *)0xFFFFFFFC)
#define	MMUCR	(*(volatile unsigned long *)0xFFFFFFE0)

/************************/
/*       4.EXP          */
/************************/
#define	TRA		(*(volatile unsigned long *)0xFFFFFFD0)
#define	EXPEVT	(*(volatile unsigned long *)0xFFFFFFD4)
#define	INTEVT	(*(volatile unsigned long *)0xFFFFFFD8)
#define	INTEVT2	(*(volatile unsigned long *)0xA4000000)

/**************************/
/*       5.CACHE          */
/**************************/
#define	CCR		(*(volatile unsigned long *)0xFFFFFFEC)
#define	CCR2	(*(volatile unsinged long *)0xA40000B0)

/**************************/
/*       6.X/YMemory      */
/**************************/


/**************************/
/*       7.INTC           */
/**************************/
#define	ICR0		(*(volatile unsigned short *)0xFFFFFEE0)
#define	ICR1		(*(volatile unsigned short *)0xA4000010)
#define	ICR2		(*(volatile unsigned short *)0xA4000012)
#define	PINTER		(*(volatile unsigned short *)0xA4000014)
#define	IPRA		(*(volatile unsigned short *)0xFFFFFEE2)
#define	IPRB		(*(volatile unsigned short *)0xFFFFFEE4)
#define	IPRC		(*(volatile unsigned short *)0xA4000016)
#define	IPRD		(*(volatile unsigned short *)0xA4000018)
#define	IPRE		(*(volatile unsigned short *)0xA400001A)
#define	IRR0		(*(volatile unsigned char *)0xA4000004)
#define	IRR1		(*(volatile unsigned char *)0xA4000006)
#define	IRR2		(*(volatile unsigned char *)0xA4000008)

/**************************/
/*       8.UBC            */
/**************************/
#define	BARA		(*(volatile unsigned long *)0xFFFFFFB0)
#define	BAMRA		(*(volatile unsigned long *)0xFFFFFFB4)
#define	BBRA		(*(volatile unsigned short *)0xFFFFFFB8)

#define	BARB		(*(volatile unsigned long *)0xFFFFFFA0)
#define	BAMRB		(*(volatile unsigned long *)0xFFFFFFA4)
#define	BBRB		(*(volatile unsigned short *)0xFFFFFFA8)
#define	BDRB		(*(volatile unsigned long *)0xFFFFFF90)
#define	BDMRB		(*(volatile unsigned long *)0xFFFFFF94)
#define	BRCR		(*(volatile unsigned long *)0xFFFFFF98)

#define	BETR		(*(volatile unsigned short *)0xFFFFFF9C)
#define	BRSR		(*(volatile unsigned long *)0xFFFFFFAC)
#define	BRDR		(*(volatile unsigned long *)0xFFFFFFBC)

#define	BASRA		(*(volatile unsigned char *)0xFFFFFFE4)
#define	BASRB		(*(volatile unsigned char *)0xFFFFFFE8)

/**************************/
/*       9.Low Power      */
/**************************/
#define	STBCR		(*(volatile unsigned char *)0xFFFFFF82)
#define	STBCR2		(*(volatile unsigned char *)0xFFFFFF88)

/**************************/
/*      10.CPG            */
/**************************/
#define	FRQCR		(*(volatile unsigned short *)0xFFFFFF80)

#define	WTCNT_R		(*(volatile unsigned char *)0xFFFFFF84)
#define	WTCNT_W		(*(volatile unsigned short *)0xFFFFFF84)
#define	WTCSR_R		(*(volatile unsigned char *)0xFFFFFF86)
#define	WTCSR_W		(*(volatile unsigned short *)0xFFFFFF86)

/**************************/
/*      11.BSC            */
/**************************/
#define	BCR1		(*(volatile unsigned short *)0xFFFFFF60)
#define	BCR2		(*(volatile unsigned short *)0xFFFFFF62)
#define	WCR1		(*(volatile unsigned short *)0xFFFFFF64)
#define	WCR2		(*(volatile unsigned short *)0xFFFFFF66)
#define	MCR			(*(volatile unsigned short *)0xFFFFFF68)
#define	PCR			(*(volatile unsigned short *)0xFFFFFF6C)
#define	RTCSR		(*(volatile unsigned short *)0xFFFFFF6E)
#define	RTCNT		(*(volatile unsigned short *)0xFFFFFF70)
#define	RTCOR		(*(volatile unsigned short *)0xFFFFFF72)
#define	RFCR		(*(volatile unsigned short *)0xFFFFFF74)

#define	SDMR		(*(volatile unsigned char  *)0xFFFFE880)

#define	MCSCR0		(*(volatile unsigned short *)0xFFFFFF50)
#define	MCSCR1		(*(volatile unsigned short *)0xFFFFFF52)
#define	MCSCR2		(*(volatile unsigned short *)0xFFFFFF54)
#define	MCSCR3		(*(volatile unsigned short *)0xFFFFFF56)
#define	MCSCR4		(*(volatile unsigned short *)0xFFFFFF58)
#define	MCSCR5		(*(volatile unsigned short *)0xFFFFFF5A)
#define	MCSCR6		(*(volatile unsigned short *)0xFFFFFF5C)
#define	MCSCR7		(*(volatile unsigned short *)0xFFFFFF5E)

/**************************/
/*      12.DMAC           */
/**************************/
/*
	Chanel 0
*/
#define	SAR0		(*(volatile unsigned long *)0xA4000020)
#define	DAR0		(*(volatile unsigned long *)0xA4000024)
#define	DMATCR0		(*(volatile unsigned long *)0xA4000028)
#define	CHCR0		(*(volatile unsigned long *)0xA400002C)

/*
	Chanel 1
*/
#define	SAR1		(*(volatile unsigned long *)0xA4000030)
#define	DAR1		(*(volatile unsigned long *)0xA4000034)
#define	DMATCR1		(*(volatile unsigned long *)0xA4000038)
#define	CHCR1		(*(volatile unsigned long *)0xA400003C)

/*
	Chanel 2
*/
#define	SAR2		(*(volatile unsigned long *)0xA4000040)
#define	DAR2		(*(volatile unsigned long *)0xA4000044)
#define	DMATCR2		(*(volatile unsigned long *)0xA4000048)
#define	CHCR2		(*(volatile unsigned long *)0xA400004C)

/*
	Chanel 3
*/
#define	SAR3		(*(volatile unsigned long *)0xA4000050)
#define	DAR3		(*(volatile unsigned long *)0xA4000054)
#define	DMATCR3		(*(volatile unsigned long *)0xA4000058)
#define	CHCR3		(*(volatile unsigned long *)0xA400005C)

/*
	Common
*/
#define	DMAOR		(*(volatile unsigned short *)0xA4000060)


/**************************/
/*      13.TMU            */
/**************************/
/*
	Common
*/
#define	TOCR		(*(volatile unsigned char *)0xFFFFFE90)
#define	TSTR		(*(volatile unsigned char *)0xFFFFFE92)

/*
	Chanel 0
*/
#define	TCOR0		(*(volatile unsigned long *)0xFFFFFE94)
#define	TCNT0		(*(volatile unsigned long *)0xFFFFFE98)
#define	TCR0		(*(volatile unsigned short *)0xFFFFFE9C)

/*
	Chanel 1
*/
#define	TCOR1		(*(volatile lunsigned ong *)0xFFFFFEA0)
#define	TCNT1		(*(volatile unsigned long *)0xFFFFFEA4)
#define	TCR1		(*(volatile unsigned short *)0xFFFFFEA8)

/*
	Chanel 2
*/
#define	TCOR2		(*(volatile unsigned long *)0xFFFFFEAC)
#define	TCNT2		(*(volatile unsigned long *)0xFFFFFEB0)
#define	TCR2		(*(volatile unsigned short *)0xFFFFFEB4)
#define	TCPR2		(*(volatile unsigned long *)0xFFFFFEB8)

/**************************/
/*      14.RTC            */
/**************************/
#define	R64CNT		(*(volatile unsigned char *)0xFFFFFEC0)
#define	RSECCNT		(*(volatile unsigned char *)0xFFFFFEC2)
#define	RMINCNT		(*(volatile unsigned char *)0xFFFFFEC4)
#define	RHRCNT		(*(volatile unsigned char *)0xFFFFFEC6)
#define	RWKCNT		(*(volatile unsigned char *)0xFFFFFEC8)
#define	RDAYCNT		(*(volatile unsigned char *)0xFFFFFECA)
#define	RMONCNT		(*(volatile unsigned char *)0xFFFFFECC)

#define	RYRCNT		(*(volatile unsigned char *)0xFFFFFECE)
#define	RSECAR		(*(volatile unsigned char *)0xFFFFFED0)
#define	RMINAR		(*(volatile unsigned char *)0xFFFFFED2)
#define	RHRAR		(*(volatile unsigned char *)0xFFFFFED4)

#define	RWKAR		(*(volatile unsigned char *)0xFFFFFED6)
#define	RDAYAR		(*(volatile unsigned char *)0xFFFFFED8)
#define	RMONAR		(*(volatile unsigned char *)0xFFFFFEDA)
#define	RCR1		(*(volatile unsigned char *)0xFFFFFEDC)
#define	RCR2		(*(volatile unsigned char *)0xFFFFFEDE)

/**************************/
/*      15.SCI            */
/**************************/
#define	SCSMR	(*(volatile unsigned char *)0xFFFFFE80)
#define	SCBRR	(*(volatile unsigned char *)0xFFFFFE82)
#define	SCSCR	(*(volatile unsigned char *)0xFFFFFE84)
#define	SCTDR	(*(volatile unsigned char *)0xFFFFFE86)
#define SCSSR	(*(volatile unsigned char *)0xFFFFFE88)
#define	SCRDR	(*(volatile unsigned char *)0xFFFFFE8A)

/**************************/
/*      16.SmartC         */
/**************************/
#define	SMART_SCSMR		(*(volatile unsigned char *)0xFFFFFE80)
#define	SMART_SCBRR		(*(volatile unsigned char *)0xFFFFFE82)
#define	SMART_SCSCR		(*(volatile unsigned char *)0xFFFFFE84)
#define	SMART_SCTDR		(*(volatile unsigned char *)0xFFFFFE86)
#define SMART_SCSSR		(*(volatile unsigned char *)0xFFFFFE88)
#define	SMART_SCRDR		(*(volatile unsigned char *)0xFFFFFE8A)
#define	SMART_SCSCMR	(*(volatile unsigned char *)0xFFFFFE8C)

/**************************/
/*      17.SCIF2          */
/**************************/
#define	SCSMR2		(*(volatile unsigned char *)0xA4000150)
#define	SCBRR2		(*(volatile unsigned char *)0xA4000152)
#define	SCSCR2		(*(volatile unsigned char *)0xA4000154)
#define	SCSFDR2		(*(volatile unsigned char *)0xA4000156)
#define	SCSSR2		(*(volatile unsigned short *)0xA4000158)
#define	SCFRDR2		(*(volatile unsigned char *)0xA400015A)
#define	SCFCR2		(*(volatile unsigned char *)0xA400015C)
#define	SCFDR2		(*(volatile unsigned short *)0xA400015E)

/**************************/
/*      18.IrDA           */
/**************************/
#define	SCSMR1		(*(volatile unsigned char *)0xA4000150)
#define	SCBRR1		(*(volatile unsigned char *)0xA4000152)
#define	SCSCR1		(*(volatile unsigned char *)0xA4000154)
#define	SCFTDR1		(*(volatile unsigned char *)0xA4000156)
#define	SCSSR1		(*(volatile unsigned short *)0xA4000158)
#define	SCFRDR1		(*(volatile unsigned char *)0xA400015A)
#define	SCFCR1		(*(volatile unsigned char *)0xA400015C)
#define	SCFDR1		(*(volatile unsigned short *)0xA400015E)

/**************************/
/*      19.IOMPX          */
/**************************/
#define	PACR		(*(volatile unsigned short *)0xA4000100)
#define	PBCR		(*(volatile unsigned short *)0xA4000102)
#define	PCCR		(*(volatile unsigned short *)0xA4000104)
#define	PDCR		(*(volatile unsigned short *)0xA4000106)
#define	PECR		(*(volatile unsigned short *)0xA4000108)
#define	PFCR		(*(volatile unsigned short *)0xA400010A)
#define	PGCR		(*(volatile unsigned short *)0xA400010C)
#define	PHCR		(*(volatile unsigned short *)0xA400010E)
#define	PJCR		(*(volatile unsigned short *)0xA4000110)
#define	PKCR		(*(volatile unsigned short *)0xA4000112)
#define	PLCR		(*(volatile unsigned short *)0xA4000114)
#define SCPCR		(*(volatile unsigned short *)0x04000116)

/**************************/
/*      20.I/O PORT       */
/**************************/
#define	PADR		(*(volatile unsigned char *)0xA4000120)
#define	PBDR		(*(volatile unsigned char *)0xA4000122)
#define	PCDR		(*(volatile unsigned char *)0xA4000124)
#define	PDDR		(*(volatile unsigned char *)0xA4000126)
#define	PEDR		(*(volatile unsigned char *)0xA4000128)
#define	PFDR		(*(volatile unsigned char *)0xA400012A)
#define	PGDR		(*(volatile unsigned char *)0xA400012C)
#define	PHDR		(*(volatile unsigned char *)0xA400012E)
#define	PJDR		(*(volatile unsigned char *)0xA4000130)
#define	PKDR		(*(volatile unsigned char *)0xA4000132)
#define	PLDR		(*(volatile unsigned char *)0xA4000134)
#define	SCPDR		(*(volatile unsigned char *)0xA4000136)

/**************************/
/*      21.A/D            */
/**************************/
#define	ADDRAH		(*(volatile unsigned char *)0xA4000080)
#define	ADDRAL		(*(volatile unsigned char *)0xA4000082)
#define	ADDRBH		(*(volatile unsigned char *)0xA4000084)
#define	ADDRBL		(*(volatile unsigned char *)0xA4000086)
#define	ADDRCH		(*(volatile unsigned char *)0xA4000088)
#define	ADDRCL		(*(volatile unsigned char *)0xA400008A)
#define	ADDRDH		(*(volatile unsigned char *)0xA400008C)
#define	ADDRDL		(*(volatile unsigned char *)0xA400008E)
#define	ADCSR		(*(volatile unsigned char *)0xA4000090)
#define	ADCR		(*(volatile unsigned char *)0xA4000092)

/**************************/
/*      22.D/A            */
/**************************/
#define	DADR0		(*(volatile unsigned char *)0xA40000A0)
#define	DADR1		(*(volatile unsigned char *)0xA40000A2)
#define	DACR		(*(volatile unsigned char *)0xA40000A4)

/**************************/
/*      23.HUDI           */
/**************************/
#define	SDBPR		()
#define	SDIR		(*(volatile unsigned short *)0xA4000200)
#define	SDBSR		()

#endif
