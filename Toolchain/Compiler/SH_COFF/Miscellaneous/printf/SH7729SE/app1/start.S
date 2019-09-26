!/****************************************************************
!KPIT Infosystems Ltd, Pune, India. - 16-Dec-2002.
!
!This program is distributed in the hope that it will be useful,
!but WITHOUT ANY WARRANTY; without even the implied warranty of
!MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
!
!*****************************************************************/

	! Start.s

	.list
#if __ELF__
	.section ".vects","ax"
#else	
	.section .vects
#endif
	.global _start  ! global Start routine 
	.global _exit   ! global exit routine 

	.extern _hw_initialise ! external Sub-routine to initialise Hardware
	.extern _main 
	.extern _data
	.extern _mdata
	.extern _edata
	.extern _ebss
	.extern _bss
	.extern _stack
	.extern _vects

_start:
	! initialise the SP for non-vectored code
	mov.l	stack,r15

! Call hw_initialise  and load data section from ROM to RAM only if 
! ROMSTART is defined

	! call the hardware initialiser
	mov.l	hw_initialise,r1
	jsr     @r1
	nop
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

	! zero out bss
	mov.l	ebss,r1
	mov.l	bss,r0
	cmp/eq	r0,r1         
	bt	start_3
	sub	r2,r2
start_2:
	mov.b	r2,@r0
	add	#1,r0
	cmp/eq	r0,r1
	bf     	start_2
	nop
start_3:
	mov.l	main,r1
	jsr     @r1
	nop
	
	! jump to exit
	mov	r0,r4
	mova	_exit,r0
	jmp	@r0
	nop

_exit:  nop
	bra _exit
	nop


	.ALIGN 4
hw_initialise:	
		.long	_hw_initialise
stack:
		.long	_stack
main:
		.long	_main
data:
		.long	_data
mdata:
		.long	_mdata
edata:
		.long	_edata
bss:
		.long	_bss
ebss:
		.long	_ebss

