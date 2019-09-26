/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the
 implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/
#include "macros.h"

/* SCI1 Registers */
#define SCI1_SMR    _DREF(_MPTR_(_US(char))(0xffff81b0))  /* Serial mode     ch 1 */
#define SCI1_BRR    _DREF(_MPTR_(_US(char))(0xffff81b1))  /* Bit rate        ch 1 */
#define SCI1_SCR    _DREF(_MPTR_(_US(char))(0xffff81b2))  /* Serial control  ch 1 */
#define SCI1_TDR    _DREF(_MPTR_(_US(char))(0xffff81b3))  /* Transmit data   ch 1 */
#define SCI1_SSR    _DREF(_MPTR_(_US(char))(0xffff81b4))  /* Serial status   ch 1 */
#define SCI1_RDR    _DREF(_MPTR_(_US(char))(0xffff81b5))  /* Receive data    ch 1 */



#define PACRL2		_DREF(_MPTR_(_VOL(_US(short)))(0xFFFF838E))  	/* Port A Control Register L2 */

#define PEIOR       _DREF(_MPTR_(_VOL(short))(0xFFFF83B4))
#define PECR1       _DREF(_MPTR_(_VOL(short))(0xFFFF83B8))
#define PECR2       _DREF(_MPTR_(_VOL(short))(0xFFFF83BA))
#define PEDR        _DREF(_MPTR_(_VOL(short))(0xFFFF83B0))

/* Read to Watchdog timer registers */
#define WDT_R_TCSR    _DREF(_MPTR_(_US(char))(0xffff8610)) /* Timer control/status  */
#define WDT_R_TCNT    _DREF(_MPTR_(_US(char))(0xffff8611)) /* Timer counter         */
#define WDT_R_RSTCSR  _DREF(_MPTR_(_US(char))(0xffff8613)) /* Reset control/status  */

/* Write to Watchdog timer registers */
#define WDT_W_TCSR    _DREF(_MPTR_(_US(short))(0xffff8610)) /* Timer control/status  */
#define WDT_W_TCNT    _DREF(_MPTR_(_US(short))(0xffff8610)) /* Timer counter         */
#define WDT_W_RSTCSR  _DREF(_MPTR_(_US(short))(0xffff8612)) /* Reset control/status  */

#define INTC_IPRH     _DREF(_MPTR_(_US(short))(0xffff8356)) /* Reset control/status  */
