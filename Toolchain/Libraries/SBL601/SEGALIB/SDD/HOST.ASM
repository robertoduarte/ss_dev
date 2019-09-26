

	external	MIXER_wr,MIXER_wk_wr
	external	IMXL_all_off,EFSDL_all_off,MPRO_all_clr
	external	MADR_all_clr,COEF_all_clr,TEMP_all_clr
	external	xseq_allstop,all_RR_off,clr_sng_mdst


;************************************************************************
;�y �@ �\ �z �z�X�g����̃R�}���h����					*
; ready d7.w : Command data ( $0001�` )				 	*
;************************************************************************

	include	SCSP.LIB

	external	xseq_start,xseq_stop,xseq_pause
	external	xseq_cntnu,xseq_tempo
	external	xseq_vlan_st,xseq_vlan_ed
	external	MAP_chg	; xseq_map
	external	drv_87,seq_map

HOST_P1		equ	0
HOST_P2		equ	1
HOST_P3		equ	2
HOST_P4		equ	3
HOST_P5		equ	4
HOST_P6		equ	5
HOST_P7		equ	6
HOST_P8		equ	7
HOST_P9		equ	8
HOST_P10	equ	9
HOST_P11	equ	10
HOST_P12	equ	11

	global		HOST_IF
HOST_IF:
		andi.w	#$FF,d7			;
		move.b	d7,d7
		bpl.s	seq_Prg			; jump if d7:$01�`$09
		cmpi.w	#$90+1,d7
		bcc.s	CM_ret			; return if $0084�`$FFFF
		andi.w	#$0f,d7			;
		add.w	d7,d7			;
		add.w	d7,d7			;
		jsr	CM_DRV(pc,d7.w)		;
		bra.s	CM_ret			;
CM_DRV:		jmp	drv_CDlvl(pc)		; #80
		jmp	drv_CDpan(pc)		; #81
		jmp	drv_vol(pc)		; #82
		jmp	drv_effect(pc)		; #83
		jmp	drv_84(pc)		; #84
		jmp	drv_PCM_st(pc)		; #85 PCM stream play start
		jmp	drv_PCM_ed(pc)		; #86
		jmp	drv_87(pc)		; #87 Mixer change
		jmp	drv_88(pc)		; #88 Mixer Parameter change
		jmp	drv_89(pc)		; #89 Hard-Ware check
		jmp	drv_8A(pc)		; #8A PCM parameter change
	;------- edit by Y.Kashima 31/01/95 ----;
	;	for Q sound/YAMAHA 3D sound	;
	;---------------------------------------;
		jmp	CM_ret	;drv_8B(pc)	; #8B Q SOUND check
		jmp	CM_ret	;drv_8C(pc)	; #8C YAMAHA 3D check
		jmp	CM_ret	;drv_8D(pc)		; #8D YAMAHA 3D init.
;		jmp	CM_ret(pc)
;		jmp	CM_ret(pc)
	;---------------------------------------;
		jmp	CM_ret(pc)
		jmp	CM_ret(pc)
	;---------------------------------------;
seq_Prg:	cmpi.w	#$20,d7			;
		bcc.s	CM_ret

		add.w	d7,d7			;
		add.w	d7,d7			;
		movea.l	a6,a4			;
		movem.l	a5/a6,-(SP)	;+	; reg. push
;@		lea	SEQ_decode_ofst(a6),a6	; ready : a6 = seq work top
		jsr	CM_SEQ(pc,d7.w)		;
		movem.l	(SP)+,a5/a6	;+	; reg. pop
		rts

CM_ret:		clc
		rts
CM_SEQ:		rts				; #00
		nop
		jmp	seq_start(pc)		; #01
		jmp	seq_stop(pc)		; #02
		jmp	seq_pause(pc)		; #03
		jmp	seq_cntnu(pc)		; #04
		jmp	seq_fade(pc)		; #05
		jmp	seq_allstop(pc)		; #06
		jmp	seq_tempo(pc)		; #07
		jmp	seq_map(pc)		; #08
		jmp	HOST_MIDI(pc)		; #09 ready a4:work top
		jmp	seq_vlan_st(pc)		; #0A volume analize start
		jmp	seq_vlan_ed(pc)		; #0B volume analize stop
;@		jmp	seq_volume		; #0C �폜 94/10/31
		jmp	DSP_clear(pc)		; #0C
		jmp	all_off(pc)		; #0D
		jmp	SEQ_PAN(pc)		; #0E
		jmp	CM_ret(pc)		; #0F reserb
		jmp	drv_10(pc)		; #10 Sound initial
		jmp	drv_8C(pc)		; #11 3D Sound
		jmp	drv_8B(pc)		; #12 Q Sound
		jmp	drv_8D(pc)		; #13 YAMAHA 3D init.
		jmp	CM_ret(pc)		; #14
		jmp	CM_ret(pc)		; #15
		jmp	CM_ret(pc)		; #16
		jmp	CM_ret(pc)		; #17
		jmp	CM_ret(pc)		; #18
		jmp	CM_ret(pc)		; #19
		jmp	CM_ret(pc)		; #1A
		jmp	CM_ret(pc)		; #1B
		jmp	CM_ret(pc)		; #1C
		jmp	CM_ret(pc)		; #1D
		jmp	CM_ret(pc)		; #1E
		jmp	CM_ret(pc)		; #1F
		;===============================;
		; Command = $01   �a�f�l�J�n	;
		;===============================;
		global	seq_start
seq_start:	move.b	(a0)+,d0		; P1 ready d0 : ( 0 �` 7 )
		move.b	(a0)+,d2		; P2 SEQ Bank#
		move.b	(a0)+,d3		; P3 SEQ song#
		move.b	d3,d1			; push
		jsr	get_seq_top_adr(pc)	; return a2
		bcs.s	seq_start_1		; jump if error
		move.b	d1,d3			; pop song#
		move.b	(a0)+,d1		; P4 ready d1 : priority#
		move.b	(a0)+,d2		; P5 �������[�h
		movea.l	a2,a0			; ready a0
		jsr	xseq_start(pc)		; ready a0,a6,d0,d1
seq_start_1:	clc
		rts
		;===============================;
		; Command = $02   �a�f�l��~	;
		; P1 = �����Ǘ��ԍ� 0 �` 7	;
		; P2 = 80H : with release off	;
		;===============================;
		global	seq_stop
seq_stop:	move.b	(a0)+,d0		; P1 ready d0 : ( 0 �` 7 )
		move.b	(a0)+,d1		; P2 = 80H --> release off
		move.b	(a0)+,d3		; P3 ready d3 : song#
		movem.l	d0/d1/a6,-(SP)	;+	; reg. push
		jsr	xseq_stop(pc)		; ready a6,d0
		movem.l	(SP)+,d0/d1/a6	;+	; reg. pop
		move.b	d1,d1
		bpl.s	seq_stop_exit
;@		rts

;		; <<<< all ch key-off & release off : BnH,78H,40H >>>>

		andi.b	#7,d0
		lsl.b	#5,d0			; 20,40,60,...,E0H
		sr_push				; 95/02/28
		int_di				; 11/16
		moveq	#0,d1
		move.b	_MIDI_RCV_WRPT(a6),d1	; write offset pointer
		addq.b	#1,_MIDI_RCV_WRPT(a6)	; set new write offset pointer
		add.w	d1,d1			;
		add.w	d1,d1			;
		lea	_MIDI_RCV_BF(a6),a6	;
		adda.w	d1,a6			;
;@		move.b	#0,(a6)+		; 80H
;@		move.b	#0,(a6)+		; 00H
;@		move.b	#0,(a6)+		; Control change#
;@		move.b	#0,(a6)+		; with release off
		move.b	#3,(a6)+		; Priority = 0 , MIDI = BnH
		move.b	d0,(a6)+		; �Ǘ��ԍ� & MIDI ch#
		move.b	#$78,(a6)+		; Control change#
		move.b	#$40,(a6)+		; with release off
		sr_pop				; 95/02/28

seq_stop_exit:	rts
		;===============================;
		; Command = $03  �a�f�l�|�[�Y	;
		;===============================;
		global	seq_pause
seq_pause:	move.b	(a0)+,d0		; P1 ready d0 : ( 0 �` 7 )
		jsr	xseq_pause(pc)		; ready a6,d0
		clc
		rts
		;===============================;
		; Command = $04 �a�f�l�|�[�Y����;
		;===============================;
		global	seq_cntnu
seq_cntnu:	move.b	(a0)+,d0		; P1 ready d0 : ( 0 �` 7 )
		jsr	xseq_cntnu(pc)		; ready a6,d0
		clc
		rts
		;===============================;
		; Command = $05   		;
		;===============================;
		global	seq_fade
seq_fade:	move.b	(a0)+,d0		; P1 ready d0 : �Ǘ��ԍ�
		move.b	(a0)+,d1		; P2 ready d1 : SEQ volume
		move.b	(a0)+,d2		; P3 ready d2 : fade rate
	external	xseq_fade
		jsr	xseq_fade(pc)
		clc
		rts
		;===============================;
		; Command = $06   		;
		;===============================;
		global	seq_allstop
seq_allstop:	jsr	xseq_allstop(pc)
		jmp	all_RR_off(pc)		; jump and return
		;===============================;
		; Command = $07	  tempo change	;
		;===============================;
		global	seq_tempo
seq_tempo:	move.b	(a0)+,d0		; P1 ready d0 : ( 0 �` 7 )
		move.b	(a0)+,d1		; P2 dummy
		move.w	(a0)+,d2		; P3,P4 ready d2
		jsr	xseq_tempo(pc)		; ready a6,d0,d1
		clc
		rts
		;===============================;
		; Command = $09	  MIDI direct	; a4 = work top addr.
		;===============================;
		global	HOST_MIDI
HOST_MIDI:
		lea	bs_PWKTP,a6		; = 68k Program work top addr
		sr_push				; 95/02/28
		int_di				; 11/16

		clr.w	d0
		move.b	_MIDI_RCV_WRPT(a6),d0	; write offset pointer
		addq.b	#1,_MIDI_RCV_WRPT(a6)	; set new write offset pointer
		add.w	d0,d0
		add.w	d0,d0
;@		move.w	d0,d1
;@		andi.w	#$3FF,d0
;@		addq.w	#4,d1
;@		andi.w	#$3FF,d1
;@		move.w	d1,_MIDI_RCV_WRPT(a6)	; write offset pointer �X�V
		lea	_MIDI_RCV_BF(a6),a6	;
		adda.w	d0,a6			;
		move.l	(a0)+,(a6)+		; P1 : Priority & CMD
;@		move.b	(a0)+,(a6)+		; P2 : �����Ǘ��ԍ� & MIDI ch
;@		move.b	(a0)+,(a6)+		; P3 : MIDI ch Message 2nd
;@		move.b	(a0),(a6)		; P4 : MIDI ch Message 3rd
		sr_pop				; 95/02/28

		clc
		rts
;@HOST_MIDI_full:
;@		ei
;@		stc
;@		rts
		;===============================;
		; Command = $0A : volume analize start
		;===============================;
seq_vlan_st:	jsr	xseq_vlan_st(pc)	; ready nothing
		clc
		rts
		;===============================;
		; Command = $0B : volume analize stop
		;===============================;
seq_vlan_ed:	jsr	xseq_vlan_ed(pc)	; ready nothing
		clc
		rts
;-----------------------------------------------------------------------;
;									;
;	�c�r�o�̏������� [MPRO] <--- $000 (NOP) �Ƃ���			;
;			 �x���̈� <--- $6000				;
;									;
;-----------------------------------------------------------------------;
DSP_clear:	moveq	#128-1,d7		; loop size
		moveq	#0,d0
		lea	DSP_MPRO(a5),a0		; clear [MPRO]
DSP_clear_lp:	move.l	d0,(a0)+		; fill 0000 0000 0000 0000
		move.l	d0,(a0)+		;	�~ 128��
		dbra	d7,DSP_clear_lp		;
		rts
		;===============================;
		; Command = $0D : all off 	; 94/10/31
		;===============================;
	external	FH1005_init,wk_RAM_init
all_off:
		sr_push				; 95/02/28
		int_di				; 11/16

		lea	bs_PWKTP,a6		; 68K Prg work top address
		move.w	tmp_HIF_wk(a6),d0	;
		move.w	debug_offset(a6),d1	;
		move.w	debug_cnt(a6),d2	;
		movem.l	d0/d1/d2,-(SP)		; reg. push
		jsr	FH1005_init(pc)		;
		jsr	wk_RAM_init(pc)		; ���[�N�q�`�l������

		moveq	#0,d0			;++++++++ ver1.29 ++++++++
		lea	bs_HIFWK+80H,a0	; HOST I/F work top addr
		move.l	d0,(a0)+		;
		move.l	d0,(a0)+		;
		move.l	d0,(a0)+		;
		move.l	d0,(a0)+		;

		movem.l	(SP)+,d0/d1/d2		; reg. pop
		move.w	d0,tmp_HIF_wk(a6)	;
		move.w	d1,debug_offset(a6)	;
		move.w	d2,debug_cnt(a6)	;
		sr_pop				; 95/02/28

		rts
		;=======================================================;
		; Command = $0E : Sequence PAN control ; 94/11/19	;
		; k_MIDI_PAN : bit 7 = H --> layer PAN off		;
		;		   6 = H --> SEQ PAN on			;
		;=======================================================;
	global	SEQ_PAN
SEQ_PAN:	lea	bs_PWKTP,a6		; 68K Prg work top address
		move.w	(a0),d0			; P1 : �����Ǘ��ԍ� ( 0 �` 7 )
		andi.w	#$700,d0		;
		add.w	d0,d0			; 000,200,400,..,E00
		move.b	1(a0),d1		; P2 : PAN data
		bpl.s	SEQ_PAN_ON		;  bit 7 = H : SEQ PAN off
						;        = L : SEQ PAN on
						;  bit6�`0 : PAN data
		;-------------------------------;
SEQ_PAN_OFF:	lea	kanri_wk+k_MIDI_PAN(a6,d0.w),a0
		moveq	#16-1,d7		; loop size
		moveq	#$3F,d0			; clear SEQ PAN bit
SEQ_PAN_OFF_lp:	and.b	d0,(a0)			; set layer PAN (bit7) on
						; set SEQ PAN (bit6) off
		lea	$10(a0),a0		;
		dbra	d7,SEQ_PAN_OFF_lp	;
		rts
		;-------------------------------; 0(L) �` 40H(C) �` 7FH(R)
SEQ_PAN_ON:	lsr.b	#2,d1			;	    ��
		subi.b	#$10,d1			;	    ��
		bcc.s	SEQ_PAN_0		;	    ��
		not.b	d1			;	    ��
		addi.b	#$10,d1			; 1FH(L)�`10H(C)00H�`0FH(R)
SEQ_PAN_0:	;-------------------------------; d1.b = DIPAN ( 0 �` 1FH )
		move.b	d1,d3			;
		ori.b	#$C0,d1			; set layer PAN off 
						;    & SEQ PAN on
		lea	kanri_wk+k_MIDI_PAN(a6,d0.w),a1
		moveq	#16-1,d7		; loop size
SEQ_PAN_ON_lp1:	move.b	d1,(a1)			; set layer PAN off 
		lea	$10(a1),a1		;		& SEQ PAN on
		dbra	d7,SEQ_PAN_ON_lp1	;
		;-------------------------------;
		; �������X���b�g�̓���Ǘ��ԍ�	; d0 = 000,200,400,..,E00
		; �̑S�Ăɑ΂��� PAN ���X�V	;
		;-------------------------------;
		move.b	SND_OUT_ST(a6),d4	; MONO/STEREO status
		bpl.s	SEQ_PAN_3		; jump if STEREO mode
		moveq	#0,d3			; set center
SEQ_PAN_3:	lea	slot_work(a6),a0
		moveq	#slot_size-1,d7		; loop size
		moveq	#0,d4			; initial
SEQ_PAN_ON_lp2:	cmp.w	_sl_kanri(a0,d4.w),d0	; 
		bne.s	SEQ_PAN_ON_1		; jump if not equal ���ǔԍ�
		lsr.w	#1,d4			;
		move.b	SCSP_DISDLPN(a5,d4.w),d2	;
		andi.b	#$E0,d2			; [DISDL] mask
		or.b	d3,d2			; d3 = 0 �` 1F
		move.b	d2,SCSP_DISDLPN(a5,d4.w)	;
		add.w	d4,d4
		move.b	d2,sl_DISDLPAN(a0,d4.w)	;
SEQ_PAN_ON_1:	addi.w	#slot_wk_unit,d4
		dbra	d7,SEQ_PAN_ON_lp2	;
		rts
;************************************************************************
;		�g�n�r�s�R�}���h�i���W�O�j�b�c�|�c�` ���x��		*
; [EFSDL]#16,#17 control					94/07/27*
; P1 : CD-DA level Left  ( MSB 3bits )					*
; P2 : CD-DA level Right ( MSB 3bits )					*
;************************************************************************
		global	drv_CDlvl
drv_CDlvl:
		move.b	(a0)+,d0		; P1 : CD left $00,$20,...,$E0
		andi.b	#$E0,d0			;
		move.b	mixer_wk_SCSP+16(a6),d2	; Digital input L
		andi.b	#$1F,d2			;
		or.b	d2,d0			;
		move.b	SND_OUT_ST(a6),d1	; MONO/STEREO status
		bpl.s	drv_CDlvl_ST1		; jump if STEREO mode
		andi.b	#$E0,d0			; ---> Center
drv_CDlvl_ST1:	move.b	d0,mixer_wk_SCSP+16(a6)	;
		move.b	d0,$200+SCSP_EFSDLPN(a5)	;

		move.b	(a0)+,d0		; P2 : CD rght $00,$20,...,$E0
		andi.b	#$E0,d0			;
		move.b	mixer_wk_SCSP+17(a6),d2	; Digital input R
		andi.b	#$1F,d2			;
		or.b	d2,d0			;
		move.b	d1,d1			; MONO/STEREO status
		bpl.s	drv_CDlvl_ST2		; jump if STEREO mode
		andi.b	#$E0,d0			; ---> Center
drv_CDlvl_ST2:	move.b	d0,mixer_wk_SCSP+17(a6)	;
		move.b	d0,$220+SCSP_EFSDLPN(a5)
		rts
;************************************************************************
;		�g�n�r�s�R�}���h�i���W�P�j�b�c�|�c�` �o�`�m		*
; [EFPAN]#16,#17 control					94/07/27*
;************************************************************************
		global	drv_CDpan
drv_CDpan:
		move.b	(a0)+,d0		; P1 : CD left 0,1,2,...,$1F
		andi.b	#$1F,d0			;
		move.b	mixer_wk_SCSP+16(a6),d2	;
		andi.b	#$E0,d2			;
		or.b	d2,d0			;
		move.b	SND_OUT_ST(a6),d1	; MONO/STEREO status
		bpl.s	drv_CDpan_ST1		; jump if STEREO mode
		andi.b	#$E0,d0			; ---> Center
drv_CDpan_ST1:	move.b	d0,mixer_wk_SCSP+16(a6)	;
		move.b	d0,$200+SCSP_EFSDLPN(a5)	;

		move.b	(a0)+,d0		; P2 : CD rght 0,1,2,...,$1F
		andi.b	#$1F,d0			;
		move.b	mixer_wk_SCSP+17(a6),d2	;
		andi.b	#$E0,d2			;
		or.b	d2,d0			;
		move.b	d1,d1			; MONO/STEREO status
		bpl.s	drv_CDpan_ST2		; jump if STEREO mode
		andi.b	#$E0,d0			; ---> Center
drv_CDpan_ST2:	move.b	d0,mixer_wk_SCSP+17(a6)	;
		move.b	d0,$220+SCSP_EFSDLPN(a5)
		rts
;************************************************************************
;	�g�n�r�s�R�}���h�i���W�Q�j SCSP Master Volume [MVOL] ����	*
; P1 : 0(off) �` $0F(Max)				 		*
;************************************************************************
		global	drv_vol
drv_vol:	move.b	(a0)+,d0		; P1
		andi.b	#$0F,d0			;
		move.b	d0,RG_MVOL(a5)		;
		rts
;************************************************************************
;	�g�n�r�s�R�}���h�i���W�R�j DSP Micro Program change ����	*
; P1 : Effect#						 		*
;************************************************************************
		external	CTRL_5B
		global		drv_effect
drv_effect:	clr.w	d1			; ready d1:MIDI ch#
		move.b	(a0)+,d3		; ready d3:
		jmp	CTRL_5B(pc)		; jump and return
;************************************************************************
;	�g�n�r�s�R�}���h�i���W�S�j not deffinition			*
;************************************************************************
		global	drv_84
drv_84:		rts
;************************************************************************
;		HOST Command = $85 : PCM stream play start		*
; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
; P1 :   D7      = L ( mono ) / H ( stereo )				*
;        D6      = no care						*
;        D5      = no care						*
;        D4      = L ( 16bit PCM ) / H ( 8bit PCM )			*
;        D3      = no care						*
;        D2�`D0  = PCM Stream#						*
; P2   : D7�`D5  = Direct level [DISDL]					*
;        D4�`D0  = Direct Pan   [DIPAN]					*
; P3   : D7�`D0  = PCM start address bit19�`12				*
; P4   : D7�`D0  = 	//	     bit11�`4  ( bit3�`0 = always "0" )	*
; P5,6 : D15�`D0 = PCM data sample ( memory use ) size High byte per 1ch*
; P7,8 : D14�`D11 = [OCT]						*
;	 D9 �`D0  = [FNS]						*
; P9   : D6�`D3  = [ISEL] Rch ( stereo/mono )				*
; 	 D2�`D0  = [IMXL] Rch ( stereo/mono )				*
; P10  : D6�`D3  = [ISEL] Lch ( stereo )				*
; 	 D2�`D0  = [IMXL] Lch ( stereo )				*
; P11  : D7�`D0  = total level						*
;************************************************************************
		global	drv_PCM_st
drv_PCM_st:
		lea	Pcm_Strm(a6),a3		; PCM stream# stack area
		lea	slot_work(a6),a2	;
		move.b	HOST_P1(a0),d7		; P1 : bit7 H = stereo
		bpl	drv_85_mono
		;===============================;
drv_85_stereo:	andi.w	#7,d7			; = PCM Stream#
		add.w	d7,d7			;
		move.b	0(a3,d7.w),d4		; R : play flag & slot#
		bset.b	#7,0(a3,d7.w)		;
		andi.w	#$1F,d4			;
		lsl.w	#6,d4			;
		move.b	1(a3,d7.w),d5		; L : play flag & slot#
		bset.b	#7,1(a3,d7.w)		;
		andi.w	#$1F,d5			;
		lsl.w	#6,d5			;
		;-------------------------------;
		; d4.w = ( R ) slot work offset
		; d5.w = ( L ) slot work offset
drv_85_st1:	move.b	HOST_P1(a0),d0		; P1 : D4 = 8bit / D210 = #
		move.b	d0,d7			;
		andi.w	#7,d0			; = PCM Stream#
		ori.b	#$80,d0			;
		move.b	d0,PSPN(a2,d4.w)	; save PCM Stream# & play flag
		move.b	d0,PSPN(a2,d5.w)	; save PCM Stream# & play flag
		lsr.w	#1,d4			; $00,$20,..,$3E0
		lsr.w	#1,d5			;

		move.b	#$10,$00(a5,d4.w)	; <<< key off >>>
		move.b	#$10,$00(a5,d5.w)	; <<< key off >>>

		andi.w	#00010000B,d7		; 16/8 bit PCM ?
		ori.w	#00100000B,d7		; set Forward loop
		swap	d7			;
		clr.w	d7			;
		moveq	#0,d0			;
		move.w	d0,SCSP_PCM_LSA(a5,d4.w)	; [LSA] R = 0000
		move.w	d0,SCSP_PCM_LSA(a5,d5.w)	; [LSA] L = 0000
		move.w	HOST_P3(a0),d0		; P3,4 : [SA] ���16bits
		lsl.l	#4,d0			; *10H
		or.l	d7,d0			;
		move.l	d0,(a5,d4.w)		; <<< [SA] R >>>
		moveq	#0,d1			;
		move.b	d1,SCSP_SISD(a5,d4.w)	; <<< [TL/MDL] = 0 : R >>>

;@		move.b	HOST_P7(a0),SCSP_TLVL(a5,d4.w)	;
		move.b	HOST_P11(a0),SCSP_TLVL(a5,d4.w)	;
		move.w	d1,SCSP_MDLSL(a5,d4.w)	; <<< [TL/MDL] = 0 : R >>>
		move.b	d1,SCSP_SISD(a5,d5.w)	; <<< [TL/MDL] = 0 : L >>>
;@		move.b	HOST_P7(a0),SCSP_TLVL(a5,d5.w)	;
		move.b	HOST_P11(a0),SCSP_TLVL(a5,d5.w)	;
		move.w	d1,SCSP_MDLSL(a5,d5.w)	; <<< [TL/MDL] = 0 : L >>>
		move.w	d1,SCSP_RELFO(a5,d4.w)	; <<< [LFO] : R >>>
		move.w	d1,SCSP_RELFO(a5,d5.w)	; <<< [LFO] : L >>>
		move.w	HOST_P5(a0),d1		; = 1,2,...,FFFF,0000
		subq.w	#1,d1			; = 0,1,...,FFFE,FFFF
		move.w	d1,SCSP_PCM_LEA(a5,d4.w)	; <<< [LEA] R >>>
		move.w	d1,SCSP_PCM_LEA(a5,d5.w)	; <<< [LEA] L >>>
		addq.l	#1,d1			; = 1,2,...,FFFF,10000
		; d0.l = [LPCTL][8BIT][SA] R
		; d1.l = PCM sample size / slot
		add.l	d1,d0			; + data sample size
		btst.l	#20,d7			; 8/16 bit ?
		bne.s	drv_85_1		; jump if 8bit data
		add.l	d1,d0			; + data sample size
drv_85_1:	move.l	d0,(a5,d5.w)		; <<< [SA] L >>>

		move.b	HOST_P2(a0),d0		; P2   : [DISDL],[DIPAN]
		andi.b	#$E0,d0			;
		ori.b	#$0F,d0			; Pan = [R]
		; == for monoral TV �Ή� ==
		btst.b	#7,SND_OUT_ST(a6)	; MONO/STEREO status
		beq.s	drv_85_3ST		; jump if STEREO mode
		andi.b	#$F0,d0			; set Center
drv_85_3ST:	move.b	d0,SCSP_DISDLPN(a5,d4.w)	; <<< [DISDL],[DIPAN] R >>>
		add.w	d4,d4
		move.b	d0,sl_DISDLPAN(a2,d4.w)
		lsr.w	#1,d4
		ori.b	#$10,d0			; set Pan = [L]
		move.b	d0,SCSP_DISDLPN(a5,d5.w)	; <<< [DISDL],[DIPAN] L >>>
		add.w	d5,d5
		move.b	d0,sl_DISDLPAN(a2,d5.w)
		lsr.w	#1,d5

		move.l	#$001F001F,d0		; AR,RR = 1FH
		move.l	d0,$08(a5,d4.w)		; <<< [D2R][D1R][EHLD][AR] >>>
		move.l	d0,$08(a5,d5.w)		; <<< [D2R][D1R][EHLD][AR] >>>
		move.b	HOST_P9(a0),d0
		move.b	d0,$15(a5,d4.w)		; <<< [ISEL],[IMXL] : R >>>
		move.b	HOST_P10(a0),d0
		move.b	d0,$15(a5,d5.w)		; <<< [ISEL],[IMXL] : L >>>
		move.w	HOST_P7(a0),d0
		move.w	d0,$10(a5,d4.w)		; <<< [OCT],[FNS] : R >>>
		move.w	d0,$10(a5,d5.w)		; <<< [OCT],[FNS] : L >>>

		move.b	#$08,$00(a5,d4.w)	; <<< key on ���� >>>
		move.b	#$18,$00(a5,d5.w)	; <<< ���� key on >>>

		rts
		;===============================;
		; d7.b = P1:mode
drv_85_mono:	andi.w	#7,d7			; = PCM Stream#
		add.w	d7,d7			;
		move.b	0(a3,d7.w),d4		; R : play flag & slot#
		bset.b	#7,0(a3,d7.w)		;
		andi.w	#$1F,d4			;
		lsl.w	#6,d4			;
		move.b	1(a3,d7.w),d5		; L : play flag & slot#
		bclr.b	#7,1(a3,d7.w)		;
		andi.w	#$1F,d5			;
		lsl.w	#6,d5			;
		;-------------------------------;
		; d4.w = ( R ) slot work offset
		; d5.w = ( L ) slot work offset
drv_85_mn1:	move.b	HOST_P1(a0),d0		; P1 : D4 = 8bit / D210 = #
		move.b	d0,d7			;
		andi.w	#7,d0			; = PCM Stream#
		ori.b	#$80,d0			;
		move.b	d0,PSPN(a2,d4.w)	; save PCM Stream# & play flag
		move.b	PSPN(a2,d5.w),d0	; L ����
		move.b	#0,PSPN(a2,d5.w)	; clear PCM Stream# & play flag
		lsr.w	#1,d4			; $00,$20,..,$3E0
		lsr.w	#1,d5			;

		move.b	#$10,$00(a5,d4.w)	; <<< key off >>>
		move.b	#$10,$00(a5,d5.w)	; <<< key off >>>

		andi.w	#00010000B,d7		; 16/8 bit PCM ?
		ori.w	#00100000B,d7		; set forward loop
		swap	d7			;
		clr.w	d7			;
		moveq	#0,d0			;
		move.w	d0,SCSP_PCM_LSA(a5,d4.w)	; <<< [LSA] >>>
		move.w	HOST_P3(a0),d0		; P3,4 : [SA] ���16bits
		lsl.l	#4,d0			;
		or.l	d7,d0			;
		move.l	d0,(a5,d4.w)		; <<< [SA] >>>
;@		move.l	d0,d3			; stack d3.l = [SA] R
		moveq	#0,d1			;

		move.b	d1,SCSP_SISD(a5,d4.w)	; <<< [SI],[SD] = 0 >>>
		move.b	HOST_P11(a0),SCSP_TLVL(a5,d4.w)	;
		move.w	d1,SCSP_MDLSL(a5,d4.w)	; <<< [MSL],[MDXYSL] = 0 >>>
		move.w	d1,SCSP_RELFO(a5,d4.w)	; <<< [LFO] >>>
		move.w	HOST_P5(a0),d1		; = 1,2,...,FFFF,0000
		subq.w	#1,d1			; = 0,1,...,FFFE,FFFF
		move.w	d1,SCSP_PCM_LEA(a5,d4.w)	; <<< [LEA] >>>

		move.b	HOST_P2(a0),d0		; P2   : [DISDL],[DIPAN]
		btst.b	#7,SND_OUT_ST(a6)	; MONO/STEREO status
		beq.s	drv_85_6ST		; jump if STEREO mode
		andi.b	#$F0,d0			; set Center
drv_85_6ST:
	move.b	d0,SCSP_DISDLPN(a5,d4.w)		; <<< [DISDL],[DIPAN] >>>
	move.l	#$001F001F,SCSP_D2R1R(a5,d4.w)		; <<< AR,RR = 1FH >>>
	move.b	HOST_P9(a0),SCSP_ISEL+1(a5,d4.w)	; <<< [ISEL],[IMXL] >>>
	move.w	HOST_P7(a0),SCSP_OCTFNS(a5,d4.w)	; <<< [OCT],[FNS] >>>
	move.b	#$18,SCSP_KXKB(a5,d4.w)			; <<< key on >>>
	add.w	d4,d4
	move.b	d0,sl_DISDLPAN(a2,d4.w)
	rts
;************************************************************************
;		 #8A �o�b�l �X�g���[���Đ� parameter change		*
;  P1 = PCM stream play#						*
;  P2 = DISDL & DIPAN							*
;  P3 = OCT & FNS (H)							*
;  P4 = OCT & FNS (L)							*
;  P5 = Right ISEL & IMXL						*
;  P6 = Left  ISEL & IMXL						*
;  P7 = Total level		1995/03/20 �V�K				*
;************************************************************************
		global	drv_8A
drv_8A:
		lea	slot_work(a6),a2	;
		lea	Pcm_Strm(a6),a3		; PCM stream# stack area
		move.b	HOST_P1(a0),d7		;


		andi.w	#7,d7
		add.w	d7,d7
		move.b	(a3,d7.w),d4		;
		bpl.s	drv_8A_ret		; jump if non play
		move.b	1(a3,d7.w),d5		;
		bpl.s	drv_8A_1		; jump if mono play

		; <<<<< stereo >>>>>

		andi.w	#$1F,d4
		andi.w	#$1F,d5
		lsl.w	#5,d4
		lsl.w	#5,d5

		move.b	HOST_P2(a0),d0		; [DISDL],[DIPAN]
		andi.b	#$E0,d0			;
		btst.b	#7,SND_OUT_ST(a6)	; MONO/STEREO status
		bne.s	drv_8A_ST		; jump if MONO mode
		ori.b	#$0F,d0			; ---> Right
drv_8A_ST:	move.b	d0,SCSP_DISDLPN(a5,d4.w)	; [DISDL],[DIPAN] R
		move.b	HOST_P7(a0),SCSP_TLVL(a5,d4.w)	;
		add.w	d4,d4
		move.b	d0,sl_DISDLPAN(a2,d4.w)
		lsr.w	#1,d4
		ori.b	#$10,d0			;
		move.b	d0,SCSP_DISDLPN(a5,d5.w)	; [DISDL],[DIPAN] L
		move.b	HOST_P7(a0),SCSP_TLVL(a5,d5.w)	;
		add.w	d5,d5
		move.b	d0,sl_DISDLPAN(a2,d5.w)
		lsr.w	#1,d5

		move.w	HOST_P3(a0),d0		; [OCT],[FNS]
		move.w	d0,SCSP_OCTFNS(a5,d4.w)	; R
		move.w	d0,SCSP_OCTFNS(a5,d5.w)	; L
		move.b	HOST_P5(a0),SCSP_ISEL+1(a5,d4.w)	; [ISEL,IMXL] R
		move.b	HOST_P6(a0),SCSP_ISEL+1(a5,d5.w)	; [ISEL,IMXL] L

drv_8A_ret:	rts

drv_8A_1:	; <<<<< monoral >>>>>

		andi.w	#$1F,d4			;
		lsl.w	#5,d4			;
		move.b	HOST_P2(a0),d0		; [DISDL],[DIPAN]
		btst.b	#7,SND_OUT_ST(a6)	; MONO/STEREO status
		beq.s	drv_8A_1ST		; jump if STEREO mode
		andi.b	#$E0,d0			; --> Center
drv_8A_1ST:	move.b	d0,SCSP_DISDLPN(a5,d4.w)	; [DISDL],[DIPAN] R
		move.b	HOST_P7(a0),SCSP_TLVL(a5,d4.w)	;
		add.w	d4,d4
		move.b	d0,sl_DISDLPAN(a2,d4.w)
		lsr.w	#1,d4
		move.w	HOST_P3(a0),SCSP_OCTFNS(a5,d4.w)	; [OCT],[FNS] R
		move.b	HOST_P5(a0),SCSP_ISEL+1(a5,d4.w)	; [ISEL],[IMXL] R
		rts
;************************************************************************
;		HOST Command = $86 : PCM stream play stop		*
;************************************************************************
		global	drv_PCM_ed
drv_PCM_ed:	move.b	(a0),d7			; = P1 : PCM stream# ( 0 �` 7 )
		andi.w	#7,d7			;
		add.w	d7,d7			;
		; ready d7 : PCM stream#*2 ( 0,2,4,...0E )
PCM_strm_stop:
		lea	IO_SCSP,a5		; �Œ�
		lea	bs_PWKTP,a6		; 68K Prg work top address
		lea	Pcm_Strm(a6),a3		; PCM stream# stack area top
		lea	slot_work(a6),a2	;
		; <<< R ���� >>>
		move.b	(a3,d7.w),d4		; R
		bpl.s	drv_86_1		; jump if non play
		bclr.b	#7,(a3,d7.w)		;
		jsr	drv_86_sb(pc)
		; <<< L ���� >>>
drv_86_1:	move.b	1(a3,d7.w),d4		; L
		bpl.s	drv_86_2		; jump if non play
		bclr.b	#7,1(a3,d7.w)		;

drv_86_sb:	andi.w	#$1F,d4			;
		lsl.w	#6,d4			;
		move.b	#0,PSPN(a2,d4.w)	; clear
		lsr.w	#1,d4			;
		move.b	#$10,$00(a5,d4.w)	; <<< key off >>>
drv_86_2:	rts
;************************************************************************
;	    �g�n�r�s�R�}���h�i���W�W�j�lixer Parameter change		*
; P1 : EFREG#							94/07/27*
; P2 : [EFSDL],[EFPAN]							*
;************************************************************************
		global	drv_88
drv_88:
		clr.w	d0
		move.b	(a0)+,d0		; P1 : [EFREG#] 0 �` 17
		cmpi.w	#18,d0			;
		bcc	drv_88_ret		;
		move.b	(a0),d1			; P2 : [EFSDL],[EFPAN]
		lea	mixer_wk_SCSP(a6),a1	;
		move.b	d1,(a1,d0.w)		; mixer_wk wr

		lsl.w	#5,d0			;
		move.b	d1,SCSP_EFSDLPN(a5,d0.w)	; SCSP wr
drv_88_ret:	rts
;===============================================================;
;	edit by Y.Kashima	31/01/95			;
;	for 3D sound debug					;
;===============================================================;
	;---------------------------------------;
	;	Q SOUND				;
	;---------------------------------------;
		global		drv_8B
		external	xseq_qsound
drv_8B:
		move.b	(a0)+,d0		; P1: channel
		move.b	(a0)+,d1		; P2: pan
		bsr	xseq_qsound
		rts
	;---------------------------------------;
	;	YAMAHA 3D			;
	;---------------------------------------;
		global	drv_8C,drv_8D
		extern	xseq_YMH3Dw,dsp3d_init
drv_8C:
		move.b	(a0)+,d0		; P1:distance
		move.b	(a0)+,d1		; P2:horizontal position
		move.b	(a0)+,d2		; P3:vertical position
		bsr	xseq_YMH3Dw
		rts
	;---------------------------------------;
drv_8D:
		bsr	dsp3d_init		;3D initialize
		rts				;
;===============================================================;

; ����������������������������������������������������������������
; ��	HOST Command = $10 : Sound initial			��95/02/20
; ����������������������������������������������������������������
		global	drv_10
drv_10:
		sr_push				; 95/02/28
		int_di				; 11/16

		move.b	(a0)+,d0		; P1 : Parameter
		subq.b	#1,d0			; all SEQ stop ?
		bne.s	drv_10_1		;
		movem.l	a0,-(SP)		; reg. push
		movem.l	a5,-(SP)		; reg. push
		jsr	xseq_allstop(pc)	;
		movem.l	(SP)+,a5		; reg. pop
		jsr	all_RR_off(pc)		;
		jsr	clr_sng_mdst(pc)	; clear song mode/status
		movem.l	(SP)+,a0		; reg. pop

drv_10_1:	move.b	(a0)+,d0		; P2 : Parameter
		subq.b	#1,d0			; all PCM stream stop ?
		bne.s	drv_10_2		;
		moveq	#8-1,d1			; loop size
		moveq	#0,d7			;
drv_10_1_lp:	jsr	PCM_strm_stop(pc)	; ready d7 : PCM stream#
		addq.w	#2,d7			;
		dbra	d1,drv_10_1_lp		;

drv_10_2:	move.b	(a0)+,d0		; P3 : Parameter
		subq.b	#1,d0			; CD-DA ��~ ?
		bne.s	drv_10_3		;
		moveq	#0,d0			;
		move.b	d0,SCSP_EFSDLPN+SCSP_slot_unit*16(a5)
		move.b	d0,SCSP_EFSDLPN+SCSP_slot_unit*17(a5)

drv_10_3:	lea	bs_PWKTP,a6		; 68K Prg work top address
		lea	IO_SCSP,a5		; �Œ�
		move.b	(a0)+,d0		; P4 : Parameter
		subq.b	#1,d0			; DSP ������ ?
		bne.s	drv_10_4		;
		jsr	IMXL_all_off(pc)	;
		jsr	EFSDL_all_off(pc)	;
		jsr	MPRO_all_clr(pc)	;
		jsr	MADR_all_clr(pc)	;
		jsr	COEF_all_clr(pc)	;
		jsr	TEMP_all_clr(pc)	;
		jsr	MIXER_wk_wr(pc)		; Mixer ���A

drv_10_4:	move.b	(a0)+,d0		; P5 : Parameter
		subq.b	#1,d0			; MIXER ������ ?
		bne.s	drv_10_5		;
		jsr	EFSDL_all_off(pc)	; [EFSDL],[EFPAN] <--- 00H

drv_10_5:	sr_pop
		rts
;@; ����������������������������������������������������������������
;@; ��	HOST Command = $12 : �p�T�E���h				��
;@; ����������������������������������������������������������������
;@		external	xseq_qsound
;@drv_12:		move.b	(a0)+,d0		; P1: channel
;@		move.b	(a0)+,d1		; P2: pan
;@		jmp	xseq_qsound(pc)
;@; ����������������������������������������������������������������
;@; ��	HOST Command = $11 : �R�c �T�E���h			��
;@; ����������������������������������������������������������������
;@		external	xseq_3dsound
;@drv_11:		move.b	(a0)+,d0		; P1:
;@		move.b	(a0)+,d1		; P2:
;@		move.b	(a0)+,d2		; P3:
;@		move.b	(a0)+,d3		; P3:
;@		move.b	(a0)+,d4		; P5:
;@		move.b	(a0)+,d5		; P6:
;@		jmp	xseq_3dsound(pc)
; ����������������������������������������������������������������
; ��	HOST Command = $89 : Hard ware check			��
; ����������������������������������������������������������������
		global	drv_89
drv_89:		move.b	(a0)+,d0		; P1 : Hard check#

		cmpi.b	#7+1,d0
		bcc	CM_ret			;
		andi.w	#$07,d0			;
		add.w	d0,d0			;
		add.w	d0,d0			;
		jsr	HD_CHK_TB(pc,d0.w)	;
		bra	CM_ret			;
HD_CHK_TB:	jmp	HD_CHK_00(pc)		; D-RAM 4Mbit R/W check
		jmp	HD_CHK_01(pc)		; D-RAM 8Mbit R/W check
		jmp	HD_CHK_02(pc)		; SCSP MIDI comunication check
		jmp	HD_CHK_03(pc)		; sound out L&R check
		jmp	HD_CHK_04(pc)		; sound out L only check
		jmp	HD_CHK_05(pc)		; sound out R only check
		jmp	CM_ret(pc)		; 
		jmp	CM_ret(pc)		; 

; ����������������������������������������������������������������
; ��	HOST Command = $89 : (#00) D-RAM 4Mbit R/W check	��
; ��	HOST Command = $89 : (#01) D-RAM 8Mbit R/W check	��
; ����������������������������������������������������������������
		global	HD_CHK_00,HD_CHK_01
HD_CHK_00:	moveq	#7-1,d6			; loop size ( $10000�`$7ffff)
		bra.s	HD_CHK_00_s
HD_CHK_01:	moveq	#15-1,d6		; loop size ( $10000�`$fffff)
		;-------------------------------;
HD_CHK_00_s:
		sr_push				; 95/02/28
		int_di				; 11/16

		lea	IO_SCSP,a5		;
		lea	DSP_COEF(a5),a0		;
		move.w	#$500/4-1,d7		; loop size
		moveq	#0,d0			;
DSP_clr:	move.l	d0,(a0)+		;
		dbra	d7,DSP_clr		;

		move.w	#$7FFF,d3		; error code
		lea	$10000,a0		;
HD_CHK_00_lp:	jsr	D_RAM_RW(pc)		; 10000H R/W check
		bcs.s	HD_CHK_00_er		;
		dbra	d6,HD_CHK_00_lp		;
		addq.w	#1,d3			; complete code
HD_CHK_00_er:	move.w	d3,Mem_HD_chk
		sr_pop
		rts
;- - - - - - - - - - - - - - - - - - - - - - - -;
		global	D_RAM_RW
D_RAM_RW:	move.l	#$55555555,d1
		move.l	#$aaaaaaaa,d2
		move.w	#$10000/4-1,d7		; loop size
D_RAM_RW_lp:	move.l	(a0),d0			; stack
		move.l	d1,(a0)
		cmp.l	(a0),d1
		bne.s	D_RAM_error
		move.l	d2,(a0)
		cmp.l	(a0),d2
		bne.s	D_RAM_error
		move.l	d0,(a0)+		; set original
		dbra	d7,D_RAM_RW_lp
		clc
		rts
D_RAM_error:	stc
		rts
; ����������������������������������������������������������������
; ��	HOST Command = $89 : (#02) SCSP MIDI comunication check	��
; ����������������������������������������������������������������
		global	HD_CHK_02
HD_CHK_02:	lea	IO_SCSP,a5
		move.w	#$7FFF,d3		; error code
		move.b	RG_MIBUF(a5),d0		;
		move.b	RG_MIBUF(a5),d0		;
		move.b	RG_MIBUF(a5),d0		; 4byte dammy read
		move.b	RG_MIBUF(a5),d0		;  Input buffer clear

		move.b	#$55,d0
		jsr	HD_CHK_02_sb(pc)
		bcs.s	HD_CHK_02_er
		move.b	#$aa,d0
		jsr	HD_CHK_02_sb(pc)
		bcs.s	HD_CHK_02_er

		addq.w	#1,d3			; complete code
HD_CHK_02_er:	move.w	d3,Mem_HD_chk
;@		ei
		rts
		;-------------------------------;
HD_CHK_02_sb:	move.b	d0,RG_MOBUF(a5)
		move.w	#$80,d7			; loop size
HD_CHK_02_1:	move.w	RG_SCIPD(a5),d1		;
		andi.b	#1000b,d1		; MIDI input ?
		bne.s	HD_CHK_02_2		; jump if input
		dbra	d7,HD_CHK_02_1
		bra.s	HD_CHK_02_3
HD_CHK_02_2:	move.b	RG_MIBUF(a5),d1		; get input MIDI data
		cmp.b	d0,d1			;
		bne.s	HD_CHK_02_3
		clc
		rts
HD_CHK_02_3:	stc
		rts
; ����������������������������������������������������������������
; ��	HOST Command = $89 : (#03) sound out L&R check		��
; ��			   : (#04) sound out L only check	��
; ��			   : (#05) sound out R only check	��
; ����������������������������������������������������������������
	global		HD_CHK_03,HD_CHK_04,HD_CHK_05

HD_CHK_03:	move.b	#$E0,d2			; L&R out
		bra.s	HD_CHK_0345
HD_CHK_04:	move.b	#$FF,d2			; L only out
		bra.s	HD_CHK_0345
HD_CHK_05:	move.b	#$EF,d2			; R only out
;		bra.s	HD_CHK_0345
		;-------------------------------;
HD_CHK_0345:	lea	TESTSDTB,a0
		movea.l	a5,a1
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		move.b	d2,SCSP_DISDLPN(a5)		; Direct vol & pan
		move.b	#$18,(a5)		; key on

		move.w	#$2000,d7		;
HD_CHK_03_2:	move.w	#$10,d6			;
HD_CHK_03_1:	lsl.l	#8,d0			;
		dbra	d6,HD_CHK_03_1		;
		dbra	d7,HD_CHK_03_2		;

		move.b	#$10,(a5)		; key off
		rts

TESTSDTB:	dc.l	SQRTB+$10200000		; key off & normal loop
		dc.w	0			; LSA
		dc.w	$10-1			; LEA
		dc.w	$001f
		dc.w	$001f
		dc.w	$0000			; TL
		dc.w	0
		dc.w	$7000
		dc.w	0
		dc.w	0
		dc.w	$E000
SQRTB:		dc.w	$c000,$c000,$c000,$c000,$c000,$c000,$c000,$c000
		dc.w	$4000,$4000,$4000,$4000,$4000,$4000,$4000,$4000
;***********************************************;
; �y���́zd2.b : SEQ Bank# ( 0 �` F )		;
;	  d3.b : SEQ Song# ( 0 �` FF )		;
; �y�o�́za2 : Seq. top address			;
;	  CY : error				;
; �y�j��zd2/d3/d4/d5/d6			;
;***********************************************;
		global	get_seq_top_adr
get_seq_top_adr:
		lea	bs_AMAPC,a2		; area map current work top
		moveq	#256/8-1,d5		; loop size
gsta_lp:	move.b	(a2),d4			; 
		bmi.s	seq_strt_er1		; jump if Area Map data end
		move.b	d4,d6			;
		andi.b	#$70,d4			; = Data ID * $10
		cmpi.b	#SEQ_ID*$10,d4		;
		bne.s	gsta_1			;
		andi.b	#$0F,d6			;
		cmp.b	d6,d2			; SEQ Bank# equal ?
		beq.s	gsta_pass		; jump if yes
gsta_1:		lea	8(a2),a2		;
		dbra	d5,gsta_lp		;
seq_strt_er1:	bset.b	#6,Mem_err_bit+3	; cannot find �Y�� SEQ Bank#
		stc
		rts
seq_strt_er2:	bset.b	#7,Mem_err_bit+3	; cannot find �Y�� SEQ Song#
		stc
		rts
seq_strt_er3:	bset.b	#0,Mem_err_bit+2	; cannot down load SEQ data
		stc
		rts

gsta_pass:	bclr.b	#6,Mem_err_bit+3	; find �Y�� SEQ Bank#
		move.l	(a2)+,d2		;
		move.b	(a2)+,d4		; Seq.Bank down load �� ?
		bpl.s	seq_strt_er3		; jump if Seq.data not ready
		bclr.b	#0,Mem_err_bit+2	; SEQ data down load ��
		andi.l	#$FFFFF,d2		;
		movea.l	d2,a2			; = seq. Bank top
		andi.w	#$ff,d3			;
		cmp.w	(a2),d3			; �a�f�l�� > d3 ?
		bcc.s	seq_strt_er2		; jump if Seq song# too large
		bclr.b	#7,Mem_err_bit+3	; cannot find �Y�� SEQ Song#
		add.w	d3,d3			;
		add.w	d3,d3			;
		adda.l	2(a2,d3.w),a2		; = dest. seq# data top
		clc
		rts
