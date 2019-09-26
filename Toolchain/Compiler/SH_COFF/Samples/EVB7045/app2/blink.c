/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/

#include "7045map.h"

int __main(void) {}

void delay(unsigned long ul){if(ul) while(ul--);}

main(void)
{
    SH2_704X_PFC_PECR1 |= 0x0800;	/* Default general I/O EXCEPT PIN 13 = RESET */
    SH2_704X_PFC_PECR2 = 0x0000;	/* ALL I/O LINES BITS 7-0 */
    SH2_704X_PFC_PEIOR |= 0x0020;	/* Out -pin 5 */

    while (1)
    {

    	SH2_704X_IO_PEDR &= 0xFFDF;       /* set the LED on, lo to pin 13 */
    	delay(250000);
    	SH2_704X_IO_PEDR |= 0x0020;    /* Set green LED off */
    	delay(250000);
    }
}
