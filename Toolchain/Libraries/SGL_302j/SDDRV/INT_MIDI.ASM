;=======================================================
;	Real time MIDI controller
;-------------------------------------------------------
;	in  d0 = Receive External MIDI data
;	in  a1 = MIDIA_RAM
;=======================================================

			.include	scsp.lib
			.extern		trgt_top
			.public		MIDI_d0_ctrl

MIDI_d0_ctrl:						; called from "trgt.asm"
		lea.l	MIDIA_wk(a6),a1
		tst.b	d0
		bmi.s	midi_message			; if( 0x80 =< d0 < 0xf0 )

		tst.b	EXCL_pt(a6)			; exclusive mode ?
		bne	EXCL_data			; yes

		move.l	MIDIJPBF(a1),d1			; ???
		beq	end_of_midi_in			; ???, jump if not ready jump addr

		movea.l	d1,a0
		jmp	(a0)

;=======================================================
;	end_of_midi_in:
;=======================================================
end_of_midi_in:
		rts

;=======================================================
;	midi_message:
;=======================================================
midi_message:
		move.w	d0,-(sp)
		andi.w	#$0070,d0
		lsr.w	#4-1,d0
		movea.w	?jump_table(pc,d0.w),a0
		move.w	(sp)+,d0
		jmp	(a0)

	?jump_table:
		dc.w	three_bytes_message		; 0x80 (note_off_data)
		dc.w	three_bytes_message		; 0x90 (note_on_data)
		dc.w	three_bytes_message		; 0xa0 (polyphonic_key_pressure)
		dc.w	three_bytes_message		; 0xb0 (control_change)
		dc.w	two_bytes_message		; 0xc0 (program_change)
		dc.w	two_bytes_message		; 0xd0 (channel_pressure)
		dc.w	three_bytes_message		; 0xe0 (pitch_wheel_change)
		dc.w	system_message			; 0xf0 (system_message)

three_bytes_message:
		move.b	d0,MIDI_1st(a1)
		pea	?2nd_byte(pc)
		bra.s	end_of_midi_message

	?2nd_byte:
		move.b	d0,MIDI_2nd(a1)
		pea	?3rd_byte(pc)
		bra.s	end_of_midi_message

	?3rd_byte:
		move.b	MIDI_1st(a1),d1
		move.b	MIDI_2nd(a1),d2
		move.b	d0,d3
		bsr	MIDI_stack
		pea	?2nd_byte(pc)
		bra.s	end_of_midi_message

two_bytes_message:
		move.b	d0,MIDI_1st(a1)
		pea	?2nd_byte(pc)
		bra.s	end_of_midi_message

	?2nd_byte:
		move.b	MIDI_1st(a1),d1
		move.b	d0,d2
		moveq	#0,d3
		bsr	MIDI_stack
		pea	?2nd_byte(pc)
;@		bra.s	end_of_midi_message

end_of_midi_message:
		move.l	(sp)+,MIDIJPBF(a1)
		rts

;=======================================================
;	Transfer FIFO to MIDI buffer
;-------------------------------------------------------
;	in  d1.b = 1st byte
;	in  d2.b = 2nd byte
;	in  d3.b = 3rd byte
;=======================================================

			.public		MIDI_stack

MIDI_stack:
		movea.l	#midi_receive_buffer,a0
		moveq	#0,d4

		move.w	sr,-(sp)			; 95/02/28
		ori.w	#$2700,sr			; disable interrupts.

		move.b	_MIDI_RCV_WRPT(a6),d4		; write offset pointer. this is indeed 1/4.
		addq.b	#1,d4				; bite access is masking for 0>d4>0xff.

			.if 0

		cmp.b	_MIDI_RCV_RDPT(a6),d4		; security of overload to read & write pointer.
		bne.s	?no_error

		bset.b	#ERRb27,Mem_err_bit+0		; receive buffer pointer error.
		bra.s	end_of_midi_stack

			.endif

	?no_error:
		move.b	d4,_MIDI_RCV_WRPT(a6)		; renewal writting offset pointer.
		subq.b	#1,d4
	;;	andi.w	#$00ff,d4			; the pointer must be, 0x100>d4.
		add.w	d4,d4
		add.w	d4,d4

		move.b	d2,$02(a0,d4.w)			; 2nd byte of midi data.
		move.b	d3,$03(a0,d4.w)			; 3rd byte of midi data.
		move.b	d1,d0				; 1st byte of midi data.
		andi.b	#$0f,d0				; d0 = midi channel number(0 to 15).

			.public		INT_MIDI_pt
	INT_MIDI_pt:
		dc.w	$0000,$00e0			; ori.b #$E0,d0
		move.b	d0,$01(a0,d4.w)			; bufferring
		lsr.b	#4,d1
		andi.b	#7,d1
		move.b	d1,$00(a0,d4.w)			; d1 = MIDI message, high 4bit.

	end_of_midi_stack:
		move.w	(sp)+,sr			; 95/02/28
		rts

;=======================================================
; 	System message ( 0xf0 to 0xff )
;=======================================================
system_message:
		andi.w	#$000f,d0
		add.w	d0,d0
		movea.w	?jump_table(pc,d0.w),a0
		jmp	(a0)

	?jump_table:
		dc.w	exclusive_message		; 0xf0
		dc.w	quater_flame_message		; 0xf1
		dc.w	song_position_pointer		; 0xf2
		dc.w	song_select			; 0xf3
		dc.w	not_assigned			; 0xf4
		dc.w	not_assigned			; 0xf5
		dc.w	tune_request			; 0xf6
		dc.w	end_of_exclusive		; 0xf7
		dc.w	timing_clock			; 0xf8
		dc.w	not_assigned			; 0xf9
		dc.w	start				; 0xfa
		dc.w	continue			; 0xfb
		dc.w	stop				; 0xfc
		dc.w	not_assigned			; 0xfd
		dc.w	active_sensing			; 0xfe
		dc.w	system_reset

			.public		system_reset


;-------------------------------------------------------
	quater_flame_message:
	song_position_pointer:
	song_select:
	tune_request:
	timing_clock:
	start:
	continue:
	stop:
	active_sensing:
	not_assigned:
;-------------------------------------------------------

		rts

;-------------------------------------------------------
;	system_reset:
;-------------------------------------------------------
system_reset:
		bra	trgt_top


;-------------------------------------------------------
;	end_of_exclusive:
;-------------------------------------------------------

			.public		end_of_exclusive
end_of_exclusive:
		move.b	#0,EXCL_pt(a6)			; clear
		movea.l	#exclusive_receive_buf,a0

		move.b	(a0)+,d0			; dammy
		move.b	(a0)+,d0
		cmpi.b	#$43,d0				; = YAMAHA ID ?
		bne.s	?error_of_exclusive

		move.b	(a0)+,d0
		cmpi.b	#$79,d0				; = DIV ?
		bne.s	?error_of_exclusive

		move.b	(a0)+,d0
		cmpi.b	#$00,d0				; = Device ID ?
		bne.s	?error_of_exclusive

		move.b	(a0)+,d0
		cmpi.b	#$01,d0				; = SATURN ID ?
		bne.s	?error_of_exclusive

		move.b	(a0)+,d0			; = Command Code
		cmpi.b	#8,d0
		bcc.s	?error_of_exclusive

		andi.w	#$0007,d0
		add.w	d0,d0
		movea.w	?jump_table(pc,d0.w),a0
		jmp	(a0)

	?error_of_exclusive:
		bset.b	#ERRb26,Mem_err_bit+0		; �s�� MIDI EXCL Receive
		bsr	NACK_OUT
		rts

	?jump_table:
		dc.w	EXCL_00				; 0x00 data Dump Request
		dc.w	EXCL_01				; 0x01 data set
		dc.w	NACK_OUT			; 0x02
		dc.w	NACK_OUT			; 0x03
		dc.w	trgt_top			; 0x04 RESET
		dc.w	NACK_OUT			; 0x05
		dc.w	NACK_OUT			; 0x06
		dc.w	EXCL_07				; 0x07 HOST Command set

;-------------------------------------------------------
;	exclusive_message:
;-------------------------------------------------------

			.public		exclusive_message
exclusive_message:
		move.b	#1,EXCL_pt(a6)		; clear
		rts

EXCL_data:	clr.w	d1
		move.b	EXCL_pt(a6),d1
		movea.l	#exclusive_receive_buf,a0
		move.b	d0,(a0,d1.w)
		addq.b	#1,EXCL_pt(a6)
		rts




;======== forword source program is written by Mr.Yamamoto ========




;@EXCL_RCV_BF:	BLKB	180H,0

;-----------------------------------------------;
;		data Dump Request		;
; ready a0					;
; F0,43,79,00,01,00,xx,xx,zz,zz,zz,zz,zz,zz,F7	;
;-----------------------------------------------;
			.public		EXCL_00
EXCL_00:	bsr.w	get_size		; return d7.w ( 00 �` 7FH )
		bsr.w	get_st_addr		; return a1(=d2)
		bcs.w	NACK_OUT
; <<<< EXCL Headder set >>>>
		move.w	_MIDI_OUT_WRPT(a6),d4	; byte
		lea	_MIDI_OUT_BF(a6),a2	;
		moveq	#5-1,d0			; loop size
		lea	EXCL_HD_TB(pc),a0	;
EXCL_00_1:	move.b	(a0)+,(a2,d4.w)		; MIDI bufferring
		addq.w	#1,d4			;
		andi.w	#$3FF,d4		;
		dbra	d0,EXCL_00_1		;
; <<<< EXCL Command set >>>>
		move.b	#$41,(a2,d4.w)		; MIDI bufferring (DATA DUMP)
		addq.w	#1,d4			;
		andi.w	#$3FF,d4		;
; <<<< data size set >>>>
		move.b	d7,d0			; size - 1 ( 0�`7F )
		lsr.b	#4,d0			;
		move.b	d0,(a2,d4.w)		; MIDI bufferring
		addq.w	#1,d4			;
		andi.w	#$3FF,d4		;
		move.b	d7,d0			;
		andi.b	#$0F,d0			;
		move.b	d0,(a2,d4.w)		; MIDI bufferring
		addq.w	#1,d4			;
		andi.w	#$3FF,d4		;
; <<<< start address set >>>>
		move.l	d2,d0			; start address ( = a1 )
		lsl.l	#8,d0			;
		moveq	#6-1,d1			; loop size
EXCL_00_2:	rol.l	#4,d0			;
		andi.b	#$0F,d0			;
		move.b	d0,(a2,d4.w)		;
		addq.w	#1,d4			;
		andi.w	#$3FF,d4		;
		dbra	d1,EXCL_00_2		;
; <<<< nible data set >>>>
; 		ready d7 : loop size
; 		      a1 : desti addr.
EXCL_00_lp:	move.b	(a1)+,d0
		move.b	d0,d2
		lsr.b	#4,d0
		move.b	d0,(a2,d4.w)		; MSB 4bits
		addq.w	#1,d4			;
		andi.w	#$3FF,d4		;
		andi.b	#$0F,d2
		move.b	d2,(a2,d4.w)		; LSB 4bits
		addq.w	#1,d4			;
		andi.w	#$3FF,d4		;
		dbra	d7,EXCL_00_lp
; <<<< EOX set >>>>
		move.b	#$F7,(a2,d4.w)		; LSB 4bits
		addq.w	#1,d4			;
		andi.w	#$3FF,d4		;
		move.w	d4,_MIDI_OUT_WRPT(a6)	; + 1 �X�V
		clc
		rts

EXCL_HD_TB:	dc.b	$F0,$43,$79,$00,$01
;-----------------------------------------------;
;		data set			;
; ready a0					;
;-----------------------------------------------;
	public	EXCL_01
EXCL_01:	bsr.w	get_size		; return d7.w ( 00 �` 7FH )
		bsr.w	get_st_addr		; return a1(=d2)
		bcs.w	NACK_OUT

EXCL_01_lp:	move.b	(a0)+,d0		; ----xxxx : data D7�`D4
		move.b	(a0)+,d1		; ----zzzz : data D3�`D0
		lsl.b	#4,d0			; xxxx0000
		andi.b	#$0F,d1			; 0000zzzz
		or.b	d1,d0			; d0.b ( D7�`D0 )
		move.b	d0,(a1)+
		dbra	d7,EXCL_01_lp

EXCL_01_pass:	lea	EXCL_ACK_TB(pc),a3	;
		bra.w	EXCL_trans
NACK_OUT:	lea	EXCL_NACK_TB(pc),a3	;
;@		bra.w	EXCL_trans
;-----------------------------------------------;
EXCL_trans:	move.w	(a3)+,d7		; loop size
		move.w	_MIDI_OUT_WRPT(a6),d4	;
		lea	_MIDI_OUT_BF(a6),a2	;
EXCL_trans_lp:	move.b	(a3)+,(a2,d4.w)		; MIDI bufferring
		addq.w	#1,d4			;
		andi.w	#$3FF,d4		;
		dbra	d7,EXCL_trans_lp	;
		move.w	d4,_MIDI_OUT_WRPT(a6)	; + 1 �X�V
		rts

EXCL_ACK_TB:	dc.w	7-1
		dc.b	$F0,$43,$79,$00,$01,$02,$F7
EXCL_NACK_TB:	dc.w	7-1
		dc.b	$F0,$43,$79,$00,$01,$03,$F7
;-----------------------------------------------;
;-----------------------------------------------;
get_size:	clr.w	d7			; clear
		move.b	(a0)+,d7		; ----xxxx : size - 1 : MSB
		move.b	(a0)+,d1		; ----zzzz : 	 //   : LSB
		lsl.b	#4,d7			; xxxx0000
		andi.b	#$0F,d1			; 0000zzzz
		or.b	d1,d7			; d7.w(size-1) = 00H �` FFH
		rts
;-----------------------------------------------;
get_st_addr:	moveq	#0,d2			;
		moveq	#$0F,d0			;
		moveq	#6-1,d3			; loop size
get_st_adr_lp:	lsl.l	#4,d2			;
		move.b	(a0)+,d1		;
		and.b	d0,d1			;
		or.b	d1,d2			;
		dbra	d3,get_st_adr_lp	;
		movea.l	d2,a1			; a1 = Dest. address
		cmpi.l	#$101000,d2
		bcc.w	st_addr_er		; jump if d2.l �� 101000H



;;	if	sw_MODEL_M
;;	else
;;		cmpi.l	#$80000,d2
;;		bcs.w	st_addr_pass		; jump if d2.l < 80000H
;;		cmpi.l	#$100000,d2
;;		bcs.w	st_addr_er		; jp if 80000H �� d2 < 100000H
;;	endif


	if	sw_MODEL_M
	else
		cmpi.l	#$80000,d2
		bls.w	st_addr_er		; jump if d2.l < 80000H
		cmpi.l	#$100000,d2
		bcs.w	st_addr_er		; jp if 80000H �� d2 < 100000H
	endif







st_addr_pass:	clc
		rts
st_addr_er:	stc
		rts
;-----------------------------------------------;
;		HOST Command set		;
;-----------------------------------------------;
	public	EXCL_07
EXCL_07:	lea	EXCL_HOST_COM(pc),a1
		move.b	(a0)+,d0		; ----xxxx : P1 MSB
		move.b	(a0)+,d1		; ----zzzz : P1 LSB
		lsl.b	#4,d0			; xxxx0000
		andi.b	#$0F,d1			; 0000zzzz
		or.b	d1,d0			; xxxxzzzz
		move.b	d0,(a1)+
		move.b	#0,(a1)+
		moveq	#14-1,d7		; loop size
EXCL_07_lp:	move.b	(a0)+,d0		; ----xxxx : P1 MSB
		move.b	(a0)+,d1		; ----zzzz : P1 LSB
		lsl.b	#4,d0			; xxxx0000
		andi.b	#$0F,d1			; 0000zzzz
		or.b	d1,d0			; xxxxzzzz
		move.b	d0,(a1)+
		dbra	d7,EXCL_07_lp
		lea	EXCL_HOST_COM(pc),a0	; ready
		move.b	(a0),d0			; ready
	external	HOST_IF
		bra.w	HOST_IF		; ready a0 , d0.b

EXCL_HOST_COM:	BLKB	16,0

;===============================================;

