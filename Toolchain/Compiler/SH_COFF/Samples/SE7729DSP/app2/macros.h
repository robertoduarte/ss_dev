/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*****************************************************************/
#if !defined(_MACROS_)
#define _MACROS_
#if !defined(_VOL)
#define _VOL(x)	volatile x
#endif

#if !defined(_US)
#define _US(x)	unsigned x
#endif

#if !defined(_DREF)
#define _DREF(x) (* x)
#endif

#if !defined(_MPTR_)
#define _MPTR_(x)	(x *)
#endif

#endif

