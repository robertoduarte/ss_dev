/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/

extern void start (void);  /* Startup code (in start.s)  */
extern void stack (void);  /* stack from linker script   */
/*
**-----------------------------------------------------------------------
**  Typedef for the function pointer
**-----------------------------------------------------------------------
*/

typedef void (*fp) (void);

/*
**-----------------------------------------------------------------------
**  Place the interrupt service routine symbols in the table
**  to create the vector entry
**-----------------------------------------------------------------------
*/

#define VECT_SECT          __attribute__ ((section (".vects")))

const fp HardwareVectors[] VECT_SECT = {
  start,                    /* 0  Power-on reset, Program counter (PC) */
  (fp) stack,             /* 1  Power-on reset, Stack pointer (SP)   */
};




