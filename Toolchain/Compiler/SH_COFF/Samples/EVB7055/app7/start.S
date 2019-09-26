!/****************************************************************
!KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.
!
!This program is distributed in the hope that it will be useful,
!but WITHOUT ANY WARRANTY; without even the implied warranty of
!MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
!
!*****************************************************************/

	! Start.s

	.list
	.section .text
	.global _start  ! global Start routine 

	.extern _hw_initialise ! external Sub-routine to initialise Hardware
	.extern start !  start in CRT0
	.extern _data
	.extern _mdata
	.extern _edata
	.extern _stack
	.extern _vects

_start:
	! initialise the SP for non-vectored code
	mov.l	stack,r15

! call the hardware initialiser
	mov.l	hw_initialise,r1
	jsr     @r1
	nop  
!load data section from ROM to RAM only if  ROMSTART is defined

#if ROMSTART
	
	! initialise sections
	mov.l	edata,r1		! edata in r1
	mov.l	mdata,r2		! mdata in r2
	mov.l	data,r0		        ! data in r0
	cmp/eq  r0,r1
	bt	start_1
	nop
start_l:
	mov.b   @r2,r3  !get from src
	mov.b   r3,@r0  !place in dest
	add   	#1,r2    !inc src
	add   	#1,r0    !inc dest
	cmp/eq  r0,r1   !dest == edata?
	bf	start_l
	nop
start_1:
#endif //ROMSTART

#if defined (__SH2E__) || defined (__SH3E__) || defined(__SH4_SINGLE__) ||defined(__SH4__) || defined(__SH4_SINGLE_ONLY__)
		mov.l set_fpscr_k, r1
		jsr @r1
		mov #0,r4
		lds r3,fpscr
#endif /*  defined (__SH2E__) || defined (__SH3E__) ||
	defined(__SH4_SINGLE__) || defined(__SH4__) ||
	defined(__SH4_SINGLE_ONLY__) */

	mov.l	crt_start,r1
	jsr     @r1
	nop
	
	.align 2
#if defined (__SH2E__) || defined (__SH3E__) || defined(__SH4_SINGLE__) ||defined(__SH4__) || defined(__SH4_SINGLE_ONLY__)
set_fpscr_k:
	.long	___set_fpscr
#endif /*  defined (__SH2E__) || defined (__SH3E__) ||
defined(__SH4_SINGLE__) || defined(__SH4__) ||
defined(__SH4_SINGLE_ONLY__) */
	

	.ALIGN 4
hw_initialise:	
		.long	_hw_initialise
stack:
		.long	_stack
crt_start:
		.long	start
data:
		.long	_data
mdata:
		.long	_mdata
edata:
		.long	_edata

