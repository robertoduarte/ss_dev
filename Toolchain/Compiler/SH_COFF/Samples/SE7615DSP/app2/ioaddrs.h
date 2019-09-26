/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/
#if !defined(_IO_7615DSP_)
#define _IO_7615DSP_
#include "macros.h"

#define	MAP_IO		0xFFFFFE00
#define	TOP_UBC		MAP_IO+0x100
#define	TOP_INTC	MAP_IO+0x060
#define	TOP_DMAC	MAP_IO+0x070
#define	TOP_BSC		MAP_IO+0x1e0
#define	TOP_SCI1	MAP_IO+0x0
#define	TOP_CACHE	MAP_IO+0x92

/*************************/
/*		   BSC           */
/*************************/
#define BCR1 _MPTR_(_VOL(long))(TOP_BSC+0x00)			/* for SH7615	*//* FFFFFFE0 */
#define BCR2 _MPTR_(_VOL(long))(TOP_BSC+0x04)			/* for SH7615	*//* FFFFFFE4 */
#define BCR3 _MPTR_(_VOL(long))(TOP_BSC+0x1C)			/* for SH7615	*//* FFFFFFFC */
#define WCR1 _MPTR_(_VOL(long))(TOP_BSC+0x08)			/* for SH7615	*//* FFFFFFE8 */
#define WCR2 _MPTR_(_VOL(long))(TOP_BSC-0x20)			/* for SH7615	*//* FFFFFFC0 */
#define WCR3 _MPTR_(_VOL(long))(TOP_BSC-0x1C)			/* for SH7615	*//* FFFFFFC4 */
#define MCR  _MPTR_(_VOL(long))(TOP_BSC+0x0c)			/* for SH7615	*//* FFFFFFEC */
#define RTCSR _MPTR_(_VOL(long))(TOP_BSC+0x10)		/* for SH7615	*//* FFFFFFF0 */
#define RTCNT _MPTR_(_VOL(long))(TOP_BSC+0x14)		/* for SH7615	*//* FFFFFFF4 */
#define RTCOR _MPTR_(_VOL(long))(TOP_BSC+0x18)		/* for SH7615	*//* FFFFFFF8 */

/********************/
/*        SDMR		*/
/********************/
#define SDMR _DREF(_MPTR_(_VOL(_US(short)))0xffff0088)/* for SH7615 '99.10.17*/
/* Mode ---------> burst read/single write 	*/		/* for SH7615	*/
/* CAS Latency --> 3 					  	*/ 		/* for SH7615	*/
/* Burst Length -> 4 				      	*/		/* for SH7615	*/

/********************/
/*        UBC       */
/********************/
#define BARA   _MPTR_(_VOL(long))(TOP_UBC+0x00)
#define BAMRA  _MPTR_(_VOL(long))(TOP_UBC+0x04)
#define BARAH  _MPTR_(_VOL(long))(TOP_UBC+0x00)
#define BARAL  _MPTR_(_VOL(long))(TOP_UBC+0x02)
#define BAMRAH _MPTR_(_VOL(long))(TOP_UBC+0x04)
#define BAMRAL _MPTR_(_VOL(long))(TOP_UBC+0x06)
#define BBRA   _MPTR_(_VOL(long))(TOP_UBC+0x08)

#define BARB   _MPTR_(_VOL(long))(TOP_UBC+0x20)
#define BAMRB  _MPTR_(_VOL(long))(TOP_UBC+0x24)
#define BARBH  _MPTR_(_VOL(long))(TOP_UBC+0x20)
#define BARBL  _MPTR_(_VOL(long))(TOP_UBC+0x22)
#define BAMRBH _MPTR_(_VOL(long))(TOP_UBC+0x24)
#define BAMRBL _MPTR_(_VOL(long))(TOP_UBC+0x26)
#define BBRB   _MPTR_(_VOL(long))(TOP_UBC+0x28)

#define BARC   _MPTR_(_VOL(long))(TOP_UBC+0x40)
#define BAMRC  _MPTR_(_VOL(long))(TOP_UBC+0x44)
#define BBRC   _MPTR_(_VOL(long))(TOP_UBC+0x48)
#define BRCR   _MPTR_(_VOL(long))(TOP_UBC+0x30)

/********************/
/*       INTC       */
/********************/
#define IPRA _MPTR_(_VOL(long))(TOP_INTC+0x82)
#define IPRB _MPTR_(_VOL(long))(TOP_INTC+0x00)
#define IPRD _MPTR_(_VOL(long))(0xfffffe40)

#define VCRA _MPTR_(_VOL(long))(TOP_INTC+0x02)
#define VCRB _MPTR_(_VOL(long))(TOP_INTC+0x04)
#define VCRC _MPTR_(_VOL(long))(TOP_INTC+0x06)
#define VCRD _MPTR_(_VOL(long))(TOP_INTC+0x08)
#define VCRL _MPTR_(_VOL(long))(0xfffffe50)

#define VCRWDT _MPTR_(_VOL(long))(TOP_INTC+0x84)
#define ICR _MPTR_(_VOL(long))(TOP_INTC+0x80)

/********************/
/*       DMAC       */
/********************/
#define SAR0 _MPTR_(_VOL(long))(TOP_DMAC+0x110)
#define DAR0 _MPTR_(_VOL(long))(TOP_DMAC+0x114)
#define DMAOR _MPTR_(_VOL(long))(TOP_DMAC+0x140)
#define TCR0 _MPTR_(_VOL(long))(TOP_DMAC+0x118)
#define CHCR0 _MPTR_(_VOL(long))(TOP_DMAC+0x11c)
#define SAR1 _MPTR_(_VOL(long))(TOP_DMAC+0x120)
#define DAR1 _MPTR_(_VOL(long))(TOP_DMAC+0x124)
#define TCR1 _MPTR_(_VOL(long))(TOP_DMAC+0x128)
#define CHCR1 _MPTR_(_VOL(long))(TOP_DMAC+0x12c)
#define VCRDMA0 _MPTR_(_VOL(long))(TOP_DMAC+0x130)
#define VCRDMA1 _MPTR_(_VOL(long))(TOP_DMAC+0x138)
#define DRCR0 _MPTR_(_VOL(char))(TOP_DMAC+0x1)
#define DRCR1 _MPTR_(_VOL(char))(TOP_DMAC+0x2)

/*******************/
/*       SCI       */
/*******************/
/*#define SMR _MPTR_(_VOL(char))(TOP_SCI1+0x00)
#define BRR _MPTR_(_VOL(char))(TOP_SCI1+0x01)
#define SCR _MPTR_(_VOL(char))(TOP_SCI1+0x02)
#define TDR _MPTR_(_VOL(char))(TOP_SCI1+0x03)
#define SSR _MPTR_(_VOL(char))(TOP_SCI1+0x04)
#define RDR _MPTR_(_VOL(char))(TOP_SCI1+0x05)*/

/******************/
/*      SCIF1     */
/******************/
#define SCSMR1 _MPTR_(_VOL(char))(0xFFFFFCC0)
#define SCBRR1 _MPTR_(_VOL(char))(0xFFFFFCC2)
#define SCSCR1 _MPTR_(_VOL(char))(0xFFFFFCC4)
#define SCFTDR1 _MPTR_(_VOL(char))(0xFFFFFCC6)
#define SCSSR1 _MPTR_(_VOL(long))(0xFFFFFCC8)
#define SCFRDR1 _MPTR_(_VOL(char))(0xFFFFFCCC)
#define SCFCR1 _MPTR_(_VOL(char))(0xFFFFFCCE)
#define SCFDR1 _MPTR_(_VOL(long))(0xFFFFFCD0)

/******************/
/*       PFC      */
/******************/
#define PBCR _MPTR_(_VOL(long))(0xFFFFFC88)

/******************/
/*      CACHE     */
/******************/
#define	CCR _MPTR_(_VOL(char))(TOP_CACHE+0x00)

/********************/
/* TL16C552 SCI CH0 */
/********************/
#define TL0RBR _MPTR_(_VOL(char))(0x700000)
#define TL0THR _MPTR_(_VOL(char))(0x700000)
#define TL0IER _MPTR_(_VOL(char))(0x700002)
#define TL0IIR _MPTR_(_VOL(char))(0x700004)
#define TL0FCR _MPTR_(_VOL(char))(0x700004)
#define TL0LCR _MPTR_(_VOL(char))(0x700006)
#define TL0MCR _MPTR_(_VOL(char))(0x700008)
#define TL0LSR _MPTR_(_VOL(char))(0x70000a)
#define TL0MSR _MPTR_(_VOL(char))(0x70000c)
#define TL0SCR _MPTR_(_VOL(char))(0x70000e)
#define TL0DLL _MPTR_(_VOL(char))(0x700000)
#define TL0DLM _MPTR_(_VOL(char))(0x700002)

/**************************/
/* TL16C552 Parallel port */
/**************************/
#define Para_r_data _MPTR_(_VOL(char))(0x22900000)
#define Para_w_data _MPTR_(_VOL(char))(0x22900000)
#define Para_r_status _MPTR_(_VOL(char))(0x22900002)
#define Para_r_cont _MPTR_(_VOL(char))(0x22900004)
#define Para_w_cont _MPTR_(_VOL(char))(0x22900004)

/*******************/
/*       SW0       */
/*******************/
#define SW0 _MPTR_(_VOL(long))(0x22100000)

/*******************/
/*     SDRAM       */
/*******************/
#define SD_BANK1 _MPTR_(_VOL(long))(0x06000000)
#define SD_BANK2 _MPTR_(_VOL(long))(0x06800000)
#define SD_BANK3 _MPTR_(_VOL(long))(0x07000000)
#define SD_BANK4 _MPTR_(_VOL(long))(0x07800000)

#endif
