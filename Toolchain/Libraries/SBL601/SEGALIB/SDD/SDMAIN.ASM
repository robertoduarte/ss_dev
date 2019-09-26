;===============================================================;
;	/////  SATURN Sound Driver  /////			;
;	[ Sequence Driver Main : SDMAIN.ASM ] Program Source	;
;	Author:							;
;		Ver. 0.00  94.12.13	K.Fujishima		;
;	Mod. history:						;
;		Ver. 0.00c 95.03.17	Y.Kashima		;
;===============================================================;
	include	SCSP.LIB	;sddrv.inc

	extern	com_get_workaddress	; sdcom

;===============================================================
;	Command Analyze & Jump process
;===============================================================
	public	xseq_start
	public	xseq_stop
	public	xseq_pause
	public	xseq_cntnu
	public	xseq_fade
	public	ret_normal
	public	ret_error

xseq_start:	lea.l	start_command,a2
		bra.w	command_jump
xseq_stop:	lea.l	stop_command,a2
		bra.w	command_jump
xseq_pause:	lea.l	pause_command,a2
		bra.w	command_jump
xseq_cntnu:	lea.l	continue_command,a2
		bra.w	command_jump
xseq_fade:	lea.l	fade_command,a2
		bra.w	command_jump

ret_normal:	clc	; CY=0: Normal end
		rts
ret_error:	stc	; CY=1: Error
		rts

;====================================================================
;	/////  command jump  [ command_jump ]  /////
;	set address register & control mode/command matrix
;
;	input	d0: play number(0-7)
;		d1-d3: input parameter
;		a0: sequence data top address
;		a2: jump table BASE address
;
;	output	d0: play number(0-7)
;		d1-d3: input parameter
;		a0: sequence data	top address
;		a4: Decoder work	top address
;		a5: Note ON Cue table	top address
;		a6: Sequence control block top address
;
;	break	a2,d7
;
;====================================================================
	public	command_jump

command_jump:
		bsr	com_get_workaddress	; set a4/a5/a6
		andi.w	#$0007,d0		; d0=Play Number(0-7)
		move.w	#SQ_SIZ,d7		; d7: seq control block size(=64)
		mulu.w	d0,d7			;
		adda.l	d7,a6			; a6: seq control block top adrs
		clr.l	d7			;
		move.b	CB_MODE(a6),d7		; d7=mode(0-7)
		andi.b	#$07,d7			; mask
		lsl.b	#2,d7			; x4
		adda.l	d7,a2			; a2=jump table address
		jmp	(a2)			; ///// Goto Handler /////

;====================================================================
;	COMMAND/MODE Jump table
;====================================================================
	extern	stop_to_start		; sddrv
	extern	stop_and_start		; sddrv
	extern	play_to_pause		; sddrv
	extern	fade_to_pause		; sddrv
	extern	play_to_stop		; sddrv
	extern	fade_to_stop		; sddrv
	extern	playpause_to_stop	; sddrv
	extern	fadepause_to_stop	; sddrv
	extern	playpause_to_play	; sddrv
	extern	fadepause_to_fade	; sddrv
	extern	stop_vol_fade		; sddrv
	extern	play_vol_fade		; sddrv
	extern	fade_vol_fade		; sddrv
	extern	playpause_vol_fade	; sddrv
	extern	fadepause_vol_fade	; sddrv

start_command
	bra.w	stop_to_start		; 0: STOP	mode
	bra.w	stop_and_start		; 1: PLAY	mode
	bra.w	stop_and_start		; 2: FADE	mode
	bra.w	stop_and_start		; 3: PLAY-PAUSE	mode
	bra.w	stop_and_start		; 4: FADE-PAUSE	mode

pause_command
	bra.w	ret_normal		; 0: STOP	mode
	bra.w	play_to_pause		; 1: PLAY	mode
	bra.w	fade_to_pause		; 2: FADE	mode
	bra.w	ret_normal		; 3: PLAY-PAUSE	mode
	bra.w	ret_normal		; 4: FADE-PAUSE	mode

stop_command
	bra.w	ret_normal		; 0: STOP	mode
	bra.w	play_to_stop		; 1: PLAY	mode
	bra.w	fade_to_stop		; 2: FADE	mode
	bra.w	playpause_to_stop	; 3: PLAY-PAUSE	mode
	bra.w	fadepause_to_stop	; 4: FADE-PAUSE	mode

continue_command
	bra.w	ret_normal		; 0: STOP	mode
	bra.w	ret_normal		; 1: PLAY	mode
	bra.w	ret_normal		; 2: FADE	mode
	bra.w	playpause_to_play	; 3: PLAY-PAUSE	mode
	bra.w	fadepause_to_fade	; 4: FADE-PAUSE	mode

fade_command
	bra.w	stop_vol_fade		; 0: STOP	mode
	bra.w	play_vol_fade		; 1: PLAY	mode
	bra.w	fade_vol_fade		; 2: FADE	mode
	bra.w	playpause_vol_fade	; 3: PLAY-PAUSE	mode
	bra.w	fadepause_vol_fade	; 4: FADE-PAUSE	mode

;====================================================================
;	SEQ_QSOUND
;	input	d0: channel(0-3:4ch or 0-7:8ch)
;		d1: pan data(0-30)
;	output	nothing
;	break	d0-d7,a0-a6,flag
;====================================================================
	;	edit by Y.Kashima 31/01/95	;
	public	xseq_qsound
xseq_qsound:
	movea.l	Mem_IFWK_PTR,a0		; a0: system IFwork top(480h)
	move.b	OF_3D_FLAG(a0),d2	;
	andi.b	#$30,d2			;Q SOUND use check
	beq.w	ret_normal		;
	movea.l	#SCSP_DSP_COEF,a1	; SCSP COEF address(100700h)
	lea	qsound_pan_table,a2	; Q-sound PAN data table address
	moveq	#$07,d3			;mask(0-7)
	btst	#4,d2			;8ch?
	beq.s	?skip			;
	move.b	#$03,d3			;mask(0-3)
?skip:
	and.l	d3,d0			; mask channel
	lsl.l	#3,d0			; ch x 8 (get CH offset)
	andi.l	#$1f,d1			; mask PAN data(0-30)
	lsl.l	#3,d1			; pan x 8 (get PAN offset)
	clr.l	d7			;
	move.b	OF_QCOEF_PTR(a0),d7	;
	adda.l	d7,a1			; 100700 + COEF offset
	adda.l	d0,a1			;	+ CH	offset
	adda.l	d1,a2			; get PAN data	offset

	move.w	(a2)+,(a1)+		; >>>>> set L filter data
	move.w	(a2)+,(a1)+		; >>>>> set L direct data
	move.w	(a2)+,(a1)+		; >>>>> set R direct data
	move.w	(a2)+,(a1)+		; >>>>> set R filter data

	bra.w	ret_normal

	extern	dsp3d_init,dsp3d_op
;====================================================================
;	SEQ_YMH3Dw
;	input		d0:channel(0-1:2channel)
;			d1:distance(00-7F)
;			d2:horizontal position(00-7F)
;			d3:vertical position(00-7F)
;	output	nothing
;	break	d0-d7,a0-a6,flag
;====================================================================
	;	edit by Y.Kashima 31/01/95	;
	public	xseq_YMH3Dw
xseq_YMH3Dw:
	movea.l	Mem_IFWK_PTR,a0			; a0: system IFwork top(480h)
	move.b	OF_3D_FLAG(a0),d4		;
	andi.b	#$c0,d4				;YAMAHA 3D use check
	beq.w	ret_normal			;
	adda.l	#OF_3DCH1_WORK,a0		;
;@	btst	#6,d4				;ch2 use check		17/03/95 Y.K.
;@	beq.s	?skip				;no use			17/03
;@	andi.l	#$01,d0				;mask channel(0-1)	17/03
;@	lsl.l	#4,d0				;ch x 16		17/03
;@	adda.l	d0,a0				;YAMAHA 3D work address	17/03
;@?skip:								17/03
	move.b	d0,(a0)+			;distance		17/03
	move.b	d1,(a0)+			;horizontal pos.	17/03
	move.b	d2,(a0)+			;vertical pos.		17/03
	move.b	#$80,(a0)+			;flag
	move.b	#$7f,(a0)+			;dist.
	move.b	#$7f,(a0)+			;hori.
	move.b	#$7f,(a0)			;vert.
	bsr	dsp3d_op			;
	bra.w	ret_normal
;@;====================================================================
;@;	SEQ_YMH3Dr
;@;	input	d0:channel(0-1:2channel)
;@;	output	d0:distance(00-7F)
;@;		d1:horizontal position(00-7F)
;@;		d2:vertical position(00-7F)
;@;	break	a0
;@;====================================================================
;@	;	edit by Y.Kashima 31/01/95	;
;@	public	xseq_YMH3Dr
;@xseq_YMH3Dr:
;@	movea.l	Mem_IFWK_PTR,a0			; a0: system IFwork top(480h)
;@	move.b	OF_3D_FLAG(a0),d1		;
;@	andi.b	#$c0,d1				;YAMAHA 3D use check
;@	beq.w	ret_normal			;
;@	adda.l	#OF_3DCH1_WORK,a0		;
;@	btst	#6,d1				;ch2 use check
;@	beq.s	?skip				;no use
;@	andi.l	#$01,d0				;mask channel(0-1)
;@	lsl.l	#4,d0				;ch x 16
;@	addi.l	#$08,d0				;+8
;@	adda.l	d0,a0				;YAMAHA 3D work address
;@?skip:
;@	move.b	(a0)+,d0			;distance
;@	move.b	(a0)+,d1			;horizontal pos.
;@	move.b	(a0),d2				;vertical pos.
;@	bra.w	ret_normal			;
;====================================================================
;	SEQ_YMH3Di
;	input	nothing
;	output	nothing
;	break	
;====================================================================
	;	edit by Y.Kashima 31/01/95	;
	public	xseq_YMH3Di
xseq_YMH3Di:
	bsr	dsp3d_init			;
	bra.w	ret_normal			;
;====================================================================
;	Q-sound control PAN data table
;====================================================================
	public	qsound_pan_table

qsound_pan_table:

	dc.w	$0000*8,$0FFF*8,$0000*8,$0FFF*8	; 00: L15
	dc.w	$0000*8,$0FFF*8,$0000*8,$0E5E*8	; 01: L14
	dc.w	$0000*8,$0FFF*8,$0000*8,$0BF2*8	; 02: L13
	dc.w	$0000*8,$0FFF*8,$0000*8,$09F9*8	; 03: L12
	dc.w	$0000*8,$0FFF*8,$0000*8,$0770*8	; 04: L11
	dc.w	$0000*8,$0FFF*8,$0000*8,$042E*8	; 05: L10
	dc.w	$0000*8,$0FFF*8,$0000*8,$0244*8	; 06: L9
	dc.w	$0000*8,$0FFF*8,$0199*8,$007F*8	; 07: L8
	dc.w	$0000*8,$0F33*8,$0333*8,$002C*8	; 08: L7
	dc.w	$0000*8,$0F33*8,$0333*8,$0015*8	; 09: L6
	dc.w	$0000*8,$0E66*8,$0666*8,$000A*8	; 10: L5
	dc.w	$0000*8,$0E66*8,$0800*8,$0005*8	; 11: L4
	dc.w	$0000*8,$0D99*8,$08CC*8,$0002*8	; 12: L3
	dc.w	$0000*8,$0D99*8,$0999*8,$0001*8	; 13: L2
	dc.w	$0000*8,$0CCC*8,$0A66*8,$0000*8	; 14: L1
	dc.w	$0000*8,$0B33*8,$0B33*8,$0000*8	; 15: center
	dc.w	$0000*8,$0A66*8,$0CCC*8,$0000*8	; 16: R1
	dc.w	$0001*8,$0999*8,$0D99*8,$0000*8	; 17: R2
	dc.w	$0002*8,$08CC*8,$0D99*8,$0000*8	; 18: R3
	dc.w	$0005*8,$0800*8,$0E66*8,$0000*8	; 19: R4
	dc.w	$000B*8,$0666*8,$0E66*8,$0000*8	; 20: R5
	dc.w	$0016*8,$04CC*8,$0F33*8,$0000*8	; 21: R6
	dc.w	$002C*8,$0333*8,$0F33*8,$0000*8	; 22: R7
	dc.w	$007F*8,$0199*8,$0FFF*8,$0000*8	; 23: R8
	dc.w	$0251*8,$0000*8,$0FFF*8,$0000*8	; 24: R9
	dc.w	$047B*8,$0000*8,$0FFF*8,$0000*8	; 25: R10
	dc.w	$0770*8,$0000*8,$0FFF*8,$0000*8	; 26: R11
	dc.w	$09F9*8,$0000*8,$0FFF*8,$0000*8	; 27: R12
	dc.w	$0BF2*8,$0000*8,$0FFF*8,$0000*8	; 28: R13
	dc.w	$0DE6*8,$0000*8,$0FFF*8,$0000*8	; 29: R14
	dc.w	$0FFF*8,$0000*8,$0FFF*8,$0000*8	; 30: R15

;=======[ end of SDMAIN.ASM ]========================================
