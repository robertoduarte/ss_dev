
	include	SCSP.LIB

	external	trgt_top

	global	      er_01
	global	er_09,er_0A,er_0B,er_0C,er_0D,er_0E,er_0F
	global	er_10,er_11,er_12,er_13,er_14,er_15,er_16,er_17
	global	er_18,er_19,er_1F
	global	er_27
;@	global	er_26,er_22,er_25

	if	sw_MODEL_M

	global	LED_CTRL
;-----------------------------------------------------------------------;
;			�k�d�c ���� �i �J�����[�h only �j		;
;-----------------------------------------------------------------------;
; LED8(�E�[) -- MIDI-A ( MCS-1 ) ��M					;
; LED7       -- MIDI-B ( MCS-2 ) ��M					;
; LED6       -- 							;
; LED5       -- 							;
; LED4       -- 							;
; LED3       -- �r�d�p�𓀂org���퓮��					;
; LED2       -- CPU over heat						;
; LED1(���[) -- CPU ���퓮��						;
;-----------------------------------------------------------------------;
LED_CTRL:
		move.w	CPU_CAPA_cnt(a6),d1
		subq.w	#1,d1
		bne.s	MIDI_capa_1
		ori.b	#%00000010,LED_status(a6)
		move.b	#CTO_LED_speed,CTO_LED_cnt(a6)
		addi.l	#1,OVER_count(a6)
MIDI_capa_1:	;jsr	MIDI_capa_out(pc)
		move.w	#0,CPU_CAPA_cnt(a6)

		move.b	BIG_timer(a6),d0
		andi.b	#3,d0			; 8msec ?
		beq.s	LED_ctrl_10		; jump if yes
		rts

LED_ctrl_10:	move.b	LED_status(a6),d2	;
		;-------------------------------;
		addi.w	#CPU_LED_speed,LED8_cnt_wk(a6)	; <<< CPU ���� >>>
		bcc.s	LED_ctrl_01		;
		bchg	#0,d2			; CPU LED bit0
		;-------------------------------;
LED_ctrl_01:	move.b	RCVA_LED_cnt(a6),d0	; <<< �l�h�c�h-�`��M >>>
		subq.b	#1,d0
		bcs.s	LED_ctrl_3
		bne.s	LED_ctrl_2
		andi.b	#01111111B,d2
LED_ctrl_2:	move.b	d0,RCVA_LED_cnt(a6)
		;-------------------------------;
LED_ctrl_3:	move.b	RCVB_LED_cnt(a6),d0	; <<< �l�h�c�h-�a��M >>>
		subq.b	#1,d0
		bcs.s	LED_ctrl_5
		bne.s	LED_ctrl_4
		andi.b	#10111111B,d2
LED_ctrl_4:	move.b	d0,RCVB_LED_cnt(a6)
		;-------------------------------;
		global	LED_ctrl_5
LED_ctrl_5:	move.b	CTO_LED_cnt(a6),d0	; <<< CPU ���� over heat >>>
		subq.b	#8,d0
		bcs.s	LED_ctrl_7
		bne.s	LED_ctrl_6
		andi.b	#11111101B,d2
LED_ctrl_6:	move.b	d0,CTO_LED_cnt(a6)
		;-------------------------------;
LED_ctrl_7:	move.b	SEQ_LED_cnt(a6),d0	; <<< �r�d�p�𓀊��� >>>
		bpl.s	LED_ctrl_8
		ori.b	#00000100B,d2
		bra.s	LED_ctrl_9
LED_ctrl_8:	andi.b	#11111011B,d2
		;-------------------------------;
LED_ctrl_9:
;@		move.b	SCSI_LED_cnt(a6),d0
;@		subq.b	#1,d0
;@		bcs.s	LED_ctrl_B
;@		bne.s	LED_ctrl_A
;@		andi.b	#11011111B,d2
;@LED_ctrl_A:	move.b	d0,SCSI_LED_cnt(a6)
		;-------------------------------;
LED_ctrl_B:	move.b	d2,LED_status(a6)
		move.b	d2,IO_LED+1
		rts
;-----------------------------------------------------------------------;
;									;
;			�l�h�c�h�o�́i�J�����[�h�j			;
;									;
;-----------------------------------------------------------------------;
		global	MIDI_er_out
MIDI_er_out:	move.w	d1,Mem_DRVERR_FLG
		move.b	#$81,d0
		jmp	MIDI_out(pc)
		;-----------------------;
		global	MIDI_capa_out
MIDI_capa_out:

		rts
		move.b	#$80,d0
;@		jmp	MIDI_out(pc)
;---------------------------------------;
; MIDI OUT ( $8n,$xx,$xx )		;
; ready d1 : $0000�`$3FFF		;
;---------------------------------------;
		global	MIDI_out
MIDI_out:
		sr_push				; 95/02/28
		int_di				; 11/16

		move.l	d1,d2
		moveq	#$7F,d3

		lea	IO_MIDI1+1,a0
		move.b	#5,R01(a0)		;
		move.b	Rx4(a0),d1		; FIFO-Tx status <--- R54
		andi.b	#01000000B,d1		; TxRDY ?
		beq	MIDIA_SEND_er		;
		move.b	d0,Rx6(a0)		; R56 <-- d0.b

		move.b	d2,d0			;
		and.b	d3,d0			;
		move.b	#5,R01(a0)		;
		move.b	Rx4(a0),d1		; FIFO-Tx status <--- R54
		andi.b	#01000000B,d1		; TxRDY ?
		beq	MIDIA_SEND_er
		move.b	d0,Rx6(a0)		; R56 <-- d0.b

		lsr.l	#7,d2			;
		move.b	d2,d0			;
		and.b	d3,d0			;
		move.b	#5,R01(a0)		;
		move.b	Rx4(a0),d1		; FIFO-Tx status <--- R54
		andi.b	#01000000B,d1		; TxRDY ?
		beq	MIDIA_SEND_er		;
		move.b	d0,Rx6(a0)		; R56 <-- d0.b
MIDIA_SEND_er:
		sr_pop				; 95/02/28
		rts

	endif

;-----------------------------------------------------------------------;
;									;
;			�G���[����					;
;									;
;-----------------------------------------------------------------------;
er_1F:	bset.b	#ERRb30,Mem_err_bit+0		; CPU buss error
	jmp	trgt_top(pc)	;
er_01:	bset.b	#ERRb28,Mem_err_bit+0		; CPU address error
	jmp	trgt_top(pc)	;

er_09:	bset.b	#ERRa_0,Mem_DRVERR_FLG+1	; SDDRVS : ����` Level#5 ����
	rts				; SDDRV  : MIDI-1 Level#5 MIDI error
er_0A:	bset.b	#ERRa_1,Mem_DRVERR_FLG+1	; SDDRVS : ����` Level#6 ����
	rts				; SDDRV  : MIDI-2 Level#6 MIDI error

er_27:				; Level#1 event on
er_0B:				; Level#3 event on
er_0C:				; Level#4 event on
er_0D:				; Level#7 event on
er_0E:				; CPU vector#04 �s������
er_0F:				; CPU vector#05 �O���Z
er_10:				; CPU vector#06 CHK����
er_11:				; CPU vector#07 TRAPV����
er_12:				; CPU vector#08 �����ᔽ
er_13:				; CPU vector#09 TRACE��O����
er_14:				; CPU vector#10,11 ����������
er_15:				; CPU vector#12,13,14,16�`23,48�`63 reserved
er_16:				; CPU vector#15 uninitialized
er_17:				; CPU vector#24 �X�v���A�X
er_18:				; CPU vector#32�`47  TRAP����
er_19:				; CPU vector#64�`255 user vector
	bset.b	#ERRb29,Mem_err_bit+0	; CPU ��O����
	jmp	trgt_top(pc)	;

;@er_1A:	moveq	#$1A,d1		; Map change : not find the Map
;@	bra	er_return
;@er_02:	moveq	#$02,d1		; Mixer change : not find No
;@	bra	er_return
;@er_04:	moveq	#$04,d1		; Effect change : not find DSP-Prg-ID
;@	bra	er_return
;@er_05:	moveq	#$05,d1		; Effect change : not find DSP-RAM-ID
;@	bra	er_return
;@er_06:	moveq	#$06,d1		; Effect change : unit is 2000H
;@	bra	er_return
;@er_07:	moveq	#$07,d1		; Effect change : Memory �s��
;@	bra	er_return
;@er_08:	moveq	#$08,d1		; Program change : not find the Voice
;@	bra	er_return
;@er_1B:	moveq	#$1B,d1		; BANK change : not find the ���FBANK
;@	bra	er_return
;@er_20:	moveq	#$20,d1
;@	bra	er_return
;@er_23:	moveq	#$23,d1
;@	bra	er_return
;@er_24:	moveq	#$24,d1
;@
;@er_return:
;@		move.w	d1,Mem_DRVERR_FLG
;@		rts
	;-----------------------;


