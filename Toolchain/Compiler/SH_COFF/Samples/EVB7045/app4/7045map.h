/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/

#if !defined( _7045_H )
#define _7045_H
#include "macros.h"

/* SCI */

#define SH2_704X_SCI_SSR_TDRE 0x80
#define SH2_704X_SCI_SSR_RDRF 0x40
#define SH2_704X_SCI_SSR_ORER 0x20
#define SH2_704X_SCI_SSR_FER  0x10
#define SH2_704X_SCI_SSR_PER  0x08
#define SH2_704X_SCI_SSR_TEND 0x04
#define SH2_704X_SCI_SSR_MPB  0x02
#define SH2_704X_SCI_SSR_MPBT 0x01

#define SH2_704X_SCI_SMR0 _DREF(_MPTR_(_VOL(char))0xffff81a0L)
#define SH2_704X_SCI_BRR0 _DREF(_MPTR_(_VOL(char))0xffff81a1L)
#define SH2_704X_SCI_SCR0 _DREF(_MPTR_(_VOL(char))0xffff81a2L)
#define SH2_704X_SCI_TDR0 _DREF(_MPTR_(_VOL(char))0xffff81a3L)
#define SH2_704X_SCI_SSR0 _DREF(_MPTR_(_VOL(char))0xffff81a4L)
#define SH2_704X_SCI_RDR0 _DREF(_MPTR_(_VOL(char))0xffff81a5L)

#define SH2_704X_SCI_SMR1 _DREF(_MPTR_(_VOL(char))0xffff81b0L)
#define SH2_704X_SCI_BRR1 _DREF(_MPTR_(_VOL(char))0xffff81b1L)
#define SH2_704X_SCI_SCR1 _DREF(_MPTR_(_VOL(char))0xffff81b2L)
#define SH2_704X_SCI_TDR1 _DREF(_MPTR_(_VOL(char))0xffff81b3L)
#define SH2_704X_SCI_SSR1 _DREF(_MPTR_(_VOL(char))0xffff81b4L)
#define SH2_704X_SCI_RDR1 _DREF(_MPTR_(_VOL(char))0xffff81b5L)


/* UBC */

#define SH2_704X_UBC_UBARH  _DREF(_MPTR_(_VOL(short))0xffff8600L)
#define SH2_704X_UBC_UBARL  _DREF(_MPTR_(_VOL(short))0xffff8602L)
#define SH2_704X_UBC_UBAMRH _DREF(_MPTR_(_VOL(short))0xffff8604L)
#define SH2_704X_UBC_UBAMRL _DREF(_MPTR_(_VOL(short))0xffff8606L)
#define SH2_704X_UBC_UBBR   _DREF(_MPTR_(_VOL(short))0xffff8608L)


/* BSC */

#define SH2_704X_BSC_WCR1   _DREF(_MPTR_(_VOL(short))0xffff8624L)
#define SH2_704X_BSC_WCR2   _DREF(_MPTR_(_VOL(short))0xffff8626L)
#define SH2_704X_BSC_BCR1   _DREF(_MPTR_(_VOL(short))0xffff8620L)
#define SH2_704X_BSC_BCR2   _DREF(_MPTR_(_VOL(short))0xffff8622L)
#define SH2_704X_BSC_DCR    _DREF(_MPTR_(_VOL(short))0xffff862aL)
#define SH2_704X_BSC_RTCSR  _DREF(_MPTR_(_VOL(short))0xffff862cL)
#define SH2_704X_BSC_RTCNT  _DREF(_MPTR_(_VOL(short))0xffff862eL)
#define SH2_704X_BSC_RTCOR  _DREF(_MPTR_(_VOL(short))0xffff8630L)


/* PFC */

#define SH2_704X_PFC_PAIORH _DREF(_MPTR_(_VOL(short)) 0xffff8384L)
#define SH2_704X_PFC_PAIORL _DREF(_MPTR_(_VOL(short)) 0xffff8386L)
#define SH2_704X_PFC_PACRH  _DREF(_MPTR_(_VOL(short)) 0xffff8388L)
#define SH2_704X_PFC_PACRL1 _DREF(_MPTR_(_VOL(short)) 0xffff838cL)
#define SH2_704X_PFC_PACRL2 _DREF(_MPTR_(_VOL(short)) 0xffff838eL)
#define SH2_704X_PFC_PBIOR  _DREF(_MPTR_(_VOL(short)) 0xffff8394L)
#define SH2_704X_PFC_PBCR1  _DREF(_MPTR_(_VOL(short)) 0xffff8398L)
#define SH2_704X_PFC_PBCR2  _DREF(_MPTR_(_VOL(short)) 0xffff839aL)
#define SH2_704X_PFC_PCIOR  _DREF(_MPTR_(_VOL(short)) 0xffff8396L)
#define SH2_704X_PFC_PCCR   _DREF(_MPTR_(_VOL(short)) 0xffff839cL)
#define SH2_704X_PFC_PDIORH _DREF(_MPTR_(_VOL(short)) 0xffff83a4L)
#define SH2_704X_PFC_PDIORL _DREF(_MPTR_(_VOL(short)) 0xffff83a6L)
#define SH2_704X_PFC_PDCRH1 _DREF(_MPTR_(_VOL(short)) 0xffff83a8L)
#define SH2_704X_PFC_PDCRH2 _DREF(_MPTR_(_VOL(short)) 0xffff83aaL)
#define SH2_704X_PFC_PDCRL  _DREF(_MPTR_(_VOL(short)) 0xffff83acL)
#define SH2_704X_PFC_PEIOR  _DREF(_MPTR_(_VOL(short)) 0xffff83b4L)
#define SH2_704X_PFC_PECR1  _DREF(_MPTR_(_VOL(short)) 0xffff83b8L)
#define SH2_704X_PFC_PECR2  _DREF(_MPTR_(_VOL(short)) 0xffff83baL)
#define SH2_704X_PFC_IFCR   _DREF(_MPTR_(_VOL(short)) 0xffff83c8L)

/* INTC */

#define SH2_704X_INTC_IPRG_ADI  0xf000
#define SH2_704X_INTC_IPRG_DTC  0x0f00
#define SH2_704X_INTC_IPRG_CMI0 0x00f0
#define SH2_704X_INTC_IPRG_CMI1 0x000f

#define SH2_704X_INTC_IPRA _DREF(_MPTR_(_VOL(short)) 0xffff8348L)
#define SH2_704X_INTC_IPRB _DREF(_MPTR_(_VOL(short)) 0xffff834aL)
#define SH2_704X_INTC_IPRC _DREF(_MPTR_(_VOL(short)) 0xffff834cL)
#define SH2_704X_INTC_IPRD _DREF(_MPTR_(_VOL(short)) 0xffff834eL)
#define SH2_704X_INTC_IPRE _DREF(_MPTR_(_VOL(short)) 0xffff8350L)
#define SH2_704X_INTC_IPRF _DREF(_MPTR_(_VOL(short)) 0xffff8352L)
#define SH2_704X_INTC_IPRG _DREF(_MPTR_(_VOL(short)) 0xffff8354L)
#define SH2_704X_INTC_IPRH _DREF(_MPTR_(_VOL(short)) 0xffff8356L)
#define SH2_704X_INTC_ICR  _DREF(_MPTR_(_VOL(short)) 0xffff8358L)
#define SH2_704X_INTC_ISR  _DREF(_MPTR_(_VOL(short)) 0xffff835aL)


/* IO */

#define SH2_704X_IO_PDDRH_PD27DR 0x0800

#define SH2_704X_IO_PADRH _DREF(_MPTR_(_VOL(short)) 0xffff8380L)
#define SH2_704X_IO_PADRL _DREF(_MPTR_(_VOL(short)) 0xffff8382L)
#define SH2_704X_IO_PBDR  _DREF(_MPTR_(_VOL(short)) 0xffff8390L)
#define SH2_704X_IO_PCDR  _DREF(_MPTR_(_VOL(short)) 0xffff8392L)
#define SH2_704X_IO_PDDRH _DREF(_MPTR_(_VOL(short)) 0xffff83a0L)
#define SH2_704X_IO_PDDRL _DREF(_MPTR_(_VOL(short)) 0xffff83a2L)
#define SH2_704X_IO_PEDR  _DREF(_MPTR_(_VOL(short)) 0xffff83b0L)
#define SH2_704X_IO_PFDR  _DREF(_MPTR_(_VOL(char)) 0xffff83b3L)


/* SCI1 Registers */
#define SCI1_SMR    _DREF(_MPTR_(_US(char))(0xffff81b0))  /* Serial mode     ch 1 */
#define SCI1_BRR    _DREF(_MPTR_(_US(char))(0xffff81b1))  /* Bit rate        ch 1 */
#define SCI1_SCR    _DREF(_MPTR_(_US(char))(0xffff81b2))  /* Serial control  ch 1 */
#define SCI1_TDR    _DREF(_MPTR_(_US(char))(0xffff81b3))  /* Transmit data   ch 1 */
#define SCI1_SSR    _DREF(_MPTR_(_US(char))(0xffff81b4))  /* Serial status   ch 1 */
#define SCI1_RDR    _DREF(_MPTR_(_US(char))(0xffff81b5))  /* Receive data    ch 1 */


#define PACRL2	    _DREF(_MPTR_(_VOL(_US(short)))(0xFFFF838E))  	/* Port A Control Register L2 */

//#define PEIOR       _DREF(_MPTR_(_VOL(short))(0xFFFF83B4))
//#define PECR1       _DREF(_MPTR_(_VOL(short))(0xFFFF83B8))
//#define PECR2       _DREF(_MPTR_(_VOL(short))(0xFFFF83BA))
//#define PEDR        _DREF(_MPTR_(_VOL(short))(0xFFFF83B0))

/* Read to Watchdog timer registers */
#define WDT_R_TCSR    _DREF(_MPTR_(_US(char))(0xffff8610)) /* Timer control/status  */
#define WDT_R_TCNT    _DREF(_MPTR_(_US(char))(0xffff8611)) /* Timer counter         */
#define WDT_R_RSTCSR  _DREF(_MPTR_(_US(char))(0xffff8613)) /* Reset control/status  */
/* Write to Watchdog timer registers */
#define WDT_W_TCSR    _DREF(_MPTR_(_US(short))(0xffff8610)) /* Timer control/status  */
#define WDT_W_TCNT    _DREF(_MPTR_(_US(short))(0xffff8610)) /* Timer counter         */
#define WDT_W_RSTCSR  _DREF(_MPTR_(_US(short))(0xffff8612)) /* Reset control/status  */

#define INTC_IPRH     _DREF(_MPTR_(_US(short))(0xffff8356)) /* Reset control/status  */



#endif  // _7045_H

