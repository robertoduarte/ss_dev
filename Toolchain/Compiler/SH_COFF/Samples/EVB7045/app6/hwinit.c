/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/

#include "7045map.h"

#ifdef __cplusplus
extern "C" {
#endif

void hw_initialise (void)
{
    SH2_704X_BSC_WCR1 = 0xff3f;
    SH2_704X_BSC_WCR2 = 0x000f;
    SH2_704X_BSC_BCR1 = 0x2020;
    SH2_704X_BSC_BCR2 = 0xf3dd;
    SH2_704X_BSC_DCR = 0;
    SH2_704X_BSC_RTCSR = 0;
    SH2_704X_BSC_RTCNT = 0;
    SH2_704X_BSC_RTCOR = 0;

    SH2_704X_PFC_PACRH = 0x5000;
    SH2_704X_PFC_PACRL1 = 0x1550;

    /* enable rx1, tx1, rx0 and tx0 lines */
    SH2_704X_PFC_PACRL2 = 0x0145;

    SH2_704X_PFC_PBCR2 = 0x2005;
    SH2_704X_PFC_PDCRH1 = 0x5555;
    SH2_704X_PFC_PDCRH2 = 0x5555;
    SH2_704X_PFC_PDCRL = 0xffff;
    SH2_704X_PFC_PCCR = 0xffff;
    SH2_704X_PFC_IFCR = 0;
    SH2_704X_PFC_PEIOR = 0x0020;
    SH2_704X_IO_PEDR = 0x0020;

    /* set CMI0 and ADI0 irq priorities to 1 */
    SH2_704X_INTC_IPRG = 0x1010;

    /* set DEI0 and DEI1 irq priorities to 1 */
    SH2_704X_INTC_IPRC = 0x1100;

    /* set up sci0 for 115k asynch,
       re + te, no interrupts */
    SH2_704X_SCI_SMR0 = 0;
    SH2_704X_SCI_BRR0 = 23;
    //*SH2_704X_SCI_BRR0 = 7; /* 7 == 115k, 23 == 38.4k */
    SH2_704X_SCI_SCR0 = 0x30;

    /* set up sci1 for 115k asynch,
       re + te, no interrupts */
    SH2_704X_SCI_SMR1 = 0;
    SH2_704X_SCI_BRR1 = 23; /* 7 == 115k, 23 == 38.4k */
    SH2_704X_SCI_SCR1 = 0x30;

 }

#ifdef __cplusplus
} // extern "C" {
#endif
