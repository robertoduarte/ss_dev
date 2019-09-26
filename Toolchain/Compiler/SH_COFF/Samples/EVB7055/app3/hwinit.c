/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/
#include "ioaddrs.h"
#include "inlines.h"

#ifdef __cplusplus
extern "C" {
#endif

int main(void);

void hw_initialise()
{
   /* set up port pin registers
      PORT E provides either general I/O or address outputs A0 - A15
      For LSI mode EXPANDED MODE / EXTERNAL ROM control register PECR settings
      are not used; also PEIOR direction control register is not used.

      PORT F provides either general I/O or address outputs A16 - A21;
      CS0 - CS3; and RD,WAIT, WR.
      For LSI mode EXPANDED MODE / EXTERNAL ROM control register PFCR settings
      are not required for address settings.
      Initial power up value for PFCRH IS 0x0015, which selects CS0, RD and WAIT.
      Use 0x0055 to select CS1.
      Initial power up value for PFCRL IS 0x5000, which selects WR and A16 - A21.
      Also, PFIOR direction control register not used.

      Port H provides either general I/O or data outputs D0 - D15
      For LSI mode EXPANDED MODE / EXTERNAL ROM control register PHCR settings
      are not required for data as power up default selects D0 - D15 as active.
      Also, PHIOR direction control register not used.   */

		/* Initialise Port A registers */

 		PACRL = 0x0000; /* Port A Control Register Low = 0x0000 */
 		PADR = 0xFFFF;  /* Port A Data Register = 0xFFFF */

 		PAIOR = 0x7FFF;    /* Sets TxD0 as an ouput and RxD0 as an input */
 		PACRH = 0x5000;    /* Enables TxD0 and RxD0 */
        PHIOR = 0xFFFF;

		// Something for TxD1 RxD1
 		PCIOR = 0xFFFD;    /* Sets TxD1 as an ouput and RxD1 as an input */
 		PCCR  = 0x0005;    /* Enables TxD1 and RxD1 */


#ifndef EPROM
	PECR= 0xFFFF;
	PFCRL = 0x5555;   /* A16 - A21 for flash */
#endif
		/* This is valid for Mode 2, perhaps CHECK */

		/* PFIOR| = 0x003F;  Enables A16 - A21 */
		/* PEIOR| = 0xFFFF;  changed Enables A0 - A15 */


        /* BCR1 defaults to 16 bit RAM access*/

        PHCR =  0xFFFF;    /* for data lines D8- D15 */


#ifdef EPROM

        PFCRH |= 0x0040;   /*  EPROM; make CS1 active  changed */
#else
        PFCRH |= 0x0055;   /*  flash;make CS1 active  changed */
#endif

        BCR1 |= 0x0002;    /* CS1 RAM is 16bit access  */
		BCR2 = 0xF3DD;   	/* No CS1 idle cycles and asserts for RAM access */
		WCR  = 0xFF23;   	/* CS1 2wt; CS0 3wt   */


   /* serial  port interrupt vector set by micro at position 126 in table (offset 504) */
        IPRLK =0xF000;  	/* interrupt level set to 15 */

        UBC_BRCR = 0;		/* set UBC to SH7000 series compatible mode */
							/* for 7055 value 0 enables interrupt and sets
								break pin pulse to 1 x clock */

        MSTCR =0x3C00;  	/* enable UBC from power save start up mode  */

       	PCCR = 0x0100;  	/* Set port pin to be IRQ0 */
		IPRA = 0x3000;  	/* Set IRQ0 priority to 3 */
		ICR  = 0x8080; 		/* edge detect */
		set_interrupt_mask(2);   /* for IRQ0, Always use -O2 flag while compiling, otherwise this
											function call will not be inlined*/

 }

#ifdef __cplusplus
} // extern "C" {
#endif
