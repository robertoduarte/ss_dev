/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

__inline__ void set_interrupt_mask (unsigned char mask) __attribute__((always_inline));

void set_interrupt_mask (unsigned char mask)
{
  asm ("mov.l r0,@-r15");
  mask <<= 4;
  mask &= 0xf0;

  asm("stc   sr,r0");
  asm("mov   #0xff,r3");
  asm("shll8 r3");
  asm("add   #0x0f,r3");
  asm("and   r3,r0");
  asm("mov   %0,r3"::"r"(mask));
  asm("or    r3,r0");
  asm("ldc   r0,sr");
  asm("mov.l @r15+,r0");
}

#ifdef __cplusplus
} // extern "C"
#endif
