;
; SCSP 68k program
;
; Target : 171-6638A SATURN SOUND BOARD
;
;						SEGA R&D AM2 Sound yamamoto
; rewrite 95/05/02�`  Ver1.30a�Ȍ�  MIDI ch 0 �` 0FH only
;

	include	SCSP.LIB

	external	MIDI_ctrl,HOST_IF
	external	FH1005_init,wk_RAM_init,MONITOR_init
	external	HOST_IF_init,BANK_init,sl_layer_adr_init
	external	vector_tb_init,SEGA_MARK
	external	xseq_2msec
	external	send_ACK,send_NACK,get_MIDI
	external	MIXER_wr,MIXER_wk_wr,MAP_chg
	external	YM3802_init,get_BANK_addr

	if	sw_MODEL_M
		org	bs_PRGTP
        else

	external		er_09,er_0A
	external		er_1F,er_01,er_0B,er_0C,er_0D
	external		er_0E,er_0F,er_10,er_11,er_12,er_13
	external		er_14,er_15,er_16,er_17,er_18,er_19,er_27
;------------------------------------------------------------------------;
	dc.l	bs_SSP		; CPU vector#00 stack pointer
	dc.l	trgt_top	; CPU vector#01 program counter
	dc.l	er_1F		; CPU vector#02 buss error
	dc.l	er_01		; CPU vector#03 address error
	dc.l	er_0E,er_0F,er_10,er_11
	dc.l	er_12,er_13,er_14,er_14,er_15,er_15,er_15,er_16
	dc.l	er_15,er_15,er_15,er_15,er_15,er_15,er_15,er_15
	dc.l	er_17
	dc.l	er_27		;	    #26 Level#1 (SCSI)
	dc.l	level_2		; 	    #26 Level#2 Timer-B
	dc.l	er_0B		; 	    #27 Level#3 ..........
	dc.l	er_0C		; 	    #28 Level#4 ..........
	dc.l	level_5		; 	    #29 Level#5 MIDI #1
	dc.l	er_0A		; 	    #30 Level#6 MIDI #2
	dc.l	er_0D		;	    #31 Level#7 debugger
	dc.l	er_18,er_18,er_18,er_18,er_18,er_18,er_18,er_18
	dc.l	er_18,er_18,er_18,er_18,er_18,er_18,er_18,er_18
	dc.l	er_15,er_15,er_15,er_15,er_15,er_15,er_15,er_15
	dc.l	er_15,er_15,er_15,er_15,er_15,er_15,er_15,er_15
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #064-#071
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #072-#079
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #080-#087
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #088-#095
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #096-#103
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #104-#111
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #112-#119
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #120-#127
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #128-#135
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #136-#143
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #144-#151
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #152-#159
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #160-#167
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #168-#175
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #176-#183
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #184-#191
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #192-#199
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #200-#207
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #208-#215
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #216-#223
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #224-#231
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #232-#239
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #240-#247
	dc.l	er_19,er_19,er_19,er_19,er_19,er_19,er_19,er_19	; #248-#255
;
; system interface table ( 400H �` )
;
	long	bs_SINFO		; System information table pointer
	long	bs_HIFWK		; Host interface work pointer
	long	bs_AMAPC		; Area Map Current Work top
	long	bs_TIFWK		; Sound Tool Interface Work Pointer
	byte	0			; DSP program load fiag(BootROM work)
	byte	0			; Sound driver load flag
	long	bs_SIFWK	; System interface work pointer
	BLKB	42,0		; reserved
;
;system information table ( 440H �` )
;
	dc.l	trgt_top		; 68K Program top
	dc.l	sz_PRG			;	//    size
	dc.l	bs_AREAM		; Area Map top
	dc.l	sz_AREAM		;    //    size
	dc.l	bs_PWKTP		; 68K Program work top
	dc.l	sz_Prg_wk		; 	//	   size
	BLKB	40,0			; reserved
;
	BLKB	128,0		; dummy blank	System I/F work  ( 480H �` )
	BLKB	128,0FFH	; dammy data	Area Map Current ( 500H �` )
	BLKW	64,0d0aH	; dammy data		//	 ( 580H �` )
	BLKB	256,0		; dammy blank	TOOL I/F work    ( 600H �` )
	BLKB	256,0		; dummy blank	HOST I/F work    ( 700H �` )
	BLKB	2048,0		; dammy blank	HOST Command ����( 800H �` )

	endif

;===============================================;
		global	trgt_top
trgt_top:	int_di				; �����֎~
		moveq	#0,d0
		move.l	d0,d1
		move.l	d0,d2
		move.l	d0,d3
		move.l	d0,d4
		move.l	d0,d5
		move.l	d0,d6
		move.l	d0,d7
		movea.l	d0,a0
		movea.l	d0,a1
		movea.l	d0,a2
		movea.l	d0,a3
		movea.l	d0,a4
		bra.s	trgt_top_1

;=======================================================
; Credit of sond driver version
;=======================================================

		if ENGN
		dc.b	"Ver2.04 <Engine>"

		else
		dc.b	"Ver2.04 96/01/25"		; �������ł���.

		endif
			
;=======================================================
; Credit of Hardware Type ( on Middle box )
;=======================================================

		if sw_MODEL_M
		dc.b	"SATURN(M) CHECK "


       		else
			if SW_TITAN
			dc.b	"<< STV Master >>"

			else
			dc.b	"<< SEGASATURN >>"

			endif
		endif

;=======================================================
; �J����
;=======================================================

;		dc.b	"Nao V0.07  08/01"
;		dc.b	"Kas V0.00c      "
		dc.b	"A.Miyazawa 16:54"

		dc.b	"(C) SEGA Enterprises / Digital Media R&D ,Sound Division "

;=======================================================
;	start program
;=======================================================
trgt_top_1:
		movea.l	d0,a5
		movea.l	d0,a6
		;
		; �r�o�istack pointer�j�̐ݒ�
		;
		lea	bs_PWKTP,a6		; 68K Prg work top address
		lea	bs_SSP,a7		; initial stack pointer
		;
		;
		;
		lea	IO_SCSP,a5		; �Œ�
		jsr	FH1005_init(pc)		;
		jsr	wk_RAM_init(pc)		; ���[�N�q�`�l������
		cmpi.l	#$53454741,SEGA_MARK
		bne.w	trgt_top
	if	ENGN
		move.l	#$7F000,$7C0+ENGN_TB_addr
		move.l	#$800,$7C0+ENGN_TB_sz
	endif
	if	sw_MODEL_M
		jsr	MONITOR_init(pc)	; for Tone Editor
		jsr	HOST_IF_init(pc)	;
		moveq	#0,d0			; ready �l�`�o�ԍ�
		jsr	MAP_chg(pc)		;
		jsr	vector_tb_init(pc)	; Vector table initial
		jsr	BANK_init(pc)		; [scspbin_adr_bf] initial
		jsr	sl_layer_adr_init(pc)

		move.w	#0,_activ_MIX_NO(a6)	; ready Mixer# ( default )
		movea.l	knr_BANK_adr(a6),_activ_MIX_BNK(a6)
			; = �Ǘ��ԍ����O�AMIDI ch#0 �̉��FBANK#0
		jsr	MIXER_wr(pc)		;
		jsr	YM3802_init(pc)		;
	endif
		move.b	#2,RG_M4D8(a5)		; SCSP 4Mbit D-RAM mode

	if	SCSI_vctr_chg
		jsr	TRGTSCSI_trans(pc)
	endif
		bsr.w	OF_HNDSHK_flg_chk	; HOST I/F H-shake flag check
		bsr.w	CR_check		; Carriage Return code check

		move.w	#$2000,SR		; Enable Interupt
;=======================================================================;
;									;
;				main routine				;
;									;
;=======================================================================;
		global	main_st
main_st:

	if	sw_MODEL_M
		addq.w	#2,CPU_CAPA_cnt(a6)
	endif
		addq.w	#1,FUKA_cnt(a6)
		;-------------------------------;
		; SEGA format MIDI buffer read	;
		;<<< SCSP MIDI IN & SEQ �� >>>;
		;-------------------------------;
main_7:		moveq	#0,d0			;
		move.b	_MIDI_RCV_RDPT(a6),d0	; 11/16
		cmp.b	_MIDI_RCV_WRPT(a6),d0	; 0 �` ff
		beq.w	main_8_pt		; jump if input MIDI nothing
		jsr	MIDI_ctrl(pc)		; ready d0.w
		bra.w	main_7
main_8_pt:	bsr.w	MIDI_OUT_BF_ex		; MIDI ���� data SCSP OUT
		;-------------------------------;
		;  HOST Command Req code check	;
		;-------------------------------;
main_2_pt:	move.b	bs_SIFWK+OF_HNDSHK_EN,d0
		bpl.w	main_2C			; jump if HOST ������

		lea	bs_HIFWK,a0		; HOST I/F work top addr
		moveq	#8-1,d7			; loop size
main_2B:	move.b	(a0),d0			; Command �� 0 ?
		beq.w	main_2A			; jump if Command nothing

		move.l	(a0),-(sp)
		movem.l	a0/d7,-(sp)	;+	; reg. push

		bsr	HOST_IF

		movem.l	(sp)+,a0/d7	;+	; reg. pop
		move.l	(sp)+,d0
		
		cmp.l	(a0),d0
		beq.s	?100

		bset.b	#ERRa11,Mem_DRVERR_FLG
		bra.s	?next_request_comes
	?100:
		move.w	#0,(a0)			; clear Command sign
	?next_request_comes:

main_2A:	lea	$10(a0),a0		; = 00,10,20,30,..,70H + 10H
		dbra	d7,main_2B		;
		andi.b	#$7F,bs_SIFWK+OF_HNDSHK_EN	; set Sound ��������
main_2C:
		;-------------------------------;
		;  �^�C�}�[�` (�|�[�����O�Ď�)	;
		;-------------------------------;
		global	Timebase
Timebase:	move.w	RG_SCIPD(a5),d0		; �`12
		add.b	d0,d0			; �`8 Timer-A over flow ?
		bpl.w	main_st			; jump if < 2msec
		;-------------------------------;
	if	sw_MODEL_M
		external	LED_CTRL
		jsr	LED_CTRL(pc)		; for LED(�J���{�[�h)
	endif
		;-------------------------------;
		;     �^�C�}�[�` ���Z�b�g	;
		;-------------------------------;
Timebase_rst:	move.w	#TIMA_base,RG_TACTL(a5)	; 2msec [TACTL] & [TIMA]
		move.w	#$40,RG_SCIRE(a5)		; timer-A reset
		addq.b	#1,BIG_timer(a6)	;
		;-------------------------------;
	if	sw_MODEL_M
		jsr	MAC_MIXER(pc)		; for MAC TONE-Editor
	endif

; A.Miyazawa	{
			.extern		fade_control_from_timer

		bsr	fade_control_from_timer

;		}

		jsr	slot_ctrl(pc)

		move.b	BIG_timer(a6),d0	;
		andi.b	#1,d0			; 8msec(2*4msec) ?
		bne.w	main_4
		jsr	DSP_CPU(pc)
		move.b	bs_SIFWK+OF_MONO_FLAG,d0	; STEREO/MONO ?
		cmp.b	SND_OUT_ST(a6),d0	;
		beq.w	main_4			; jump if bit7 no change
		bsr	TV_mono_chg
main_4:
		;-------------------------------;
;@		move.b	RG_DEBUG+1(a5),d1	; �O��ݒ�EG�l
;@		not.b	d1			; d1.b = 0(off) �` 1F(max)
;@;@		andi.b	#$1f,d1			;
;@		andi.w	#$10,d1			;
;@		move.b	BIG_timer(a6),d0	;
;@		andi.w	#$1F,d0			;
;@		lsl.w	#6,d0			; = $00,$40,$80,...,$7C0
;@		addi.w	#slot_work,d0		;
;@		move.b	d1,EG_Value(a6,d0.w)	;
;@		move.b	BIG_timer(a6),d0	;
;@		addq	#1,d0			;
;@		andi.w	#$1F,d0			;
;@		lsl.w	#3,d0			; = $00,$08,$10,..,$F8
;@		move.b	d0,RG_DEBUG(a5)		; set [MSLC/CA/SGC/EG]

	if	sw_MODEL_M
		jsr	DSP_LINK(pc)
	endif

	if	ENGN
		bsr.w	ENGN_CTRL
	endif

FE_out_pt:	bsr.w	FE_out			; MIDI [FEH] 
F0_out_pt:	bsr.w	F0_out			; MIDI error ��� EXCL 
F4_out_pt:	bsr.w	F4_out			; MIDI [F4H] 
		bra	main_st
;-----------------------------------------------------------------------;
;		MIDI ���� data SCSP OUT or YM-3802 OUT			;
;-----------------------------------------------------------------------;
MIDI_OUT_BF_ex:	lea	_MIDI_OUT_BF(a6),a2	;
		move.w	_MIDI_OUT_RDPT(a6),d4	; word 0�`3FF
MIDI_OUT_lp:	cmp.w	_MIDI_OUT_WRPT(a6),d4	; word
		beq.w	ret_33

	if	sw_MODEL_M
		lea	IO_MIDI1+1,a0		;<<<<<< for YM3802-A >>>>>>
		move.b	#5,R01(a0)		; MIDI port-A
		move.b	Rx4(a0),d0		; FIFO-Tx status <--- R54
		andi.b	#01000000B,d0		; TxRDY ?
		beq.w	ret_33			; jump if FI-FO full
		move.b	(a2,d4.w),Rx6(a0)	; R56 <-- d0.b
	else
		move.b	RG_MSTS(a5),d0		;<<<<<< for SCSP MIDI >>>>>>
		andi.b	#$10,d0			;
		bne.w	ret_33			; jump if FI-FO full
		move.b	(a2,d4.w),RG_MOBUF(a5)	;
	endif
		addq.w	#1,d4			;
		andi.w	#$3ff,d4		;
		move.w	d4,_MIDI_OUT_RDPT(a6)	; �X�V
		bra.w	MIDI_OUT_lp
ret_33:		rts
;-----------------------------------------------------------------------;
; 	Active Sensing [FEH] �� [MIDI_OUT_BF] �ւ� buffering		;
;-----------------------------------------------------------------------;
		global	FE_out
FE_out:		move.w	FE_count(a6),d0		;
		bpl.w	ret_12			; 300msec ?
		move.w	#0,FE_count(a6)		; clear
		move.w	_MIDI_OUT_WRPT(a6),d0	; byte
		lea	_MIDI_OUT_BF(a6),a2	;
		move.b	#$FE,(a2,d0.w)		; MIDI bufferring
		addq.w	#1,d0			;
		andi.w	#$3FF,d0		;
		move.w	d0,_MIDI_OUT_WRPT(a6)	; + 1 �X�V
ret_12:		rts
;-----------------------------------------------------------------------;
; 	CPU ���� over [F4H] �� [MIDI_OUT_BF] �ւ� buffering		;
;-----------------------------------------------------------------------;
		global	F4_out
F4_out:		move.w	FUKA_cnt(a6),d0
		subq.w	#1,d0
		bne.w	F4_OUT_1
		clr.w	d1			;
		move.w	_MIDI_OUT_WRPT(a6),d1	;
		andi.w	#$3FF,d1		;
		lea	_MIDI_OUT_BF(a6),a2	;
		move.b	#$F4,(a2,d1.w)		; MIDI bufferring
		addq.w	#1,d1
		andi.w	#$3FF,d1		;
		move.w	d1,_MIDI_OUT_WRPT(a6)	; + 1 �X�V
F4_OUT_1:	move.w	#0,FUKA_cnt(a6)		; clear
		rts
;-----------------------------------------------------------------------;
;		Error ���� [_MIDI_OUT_BF] �ւ� buffering		;
;-----------------------------------------------------------------------;
		global	F0_out
F0_out:		move.l	Mem_err_bit,d1		; long
		move.w	Mem_DRVERR_FLG,d0	; word
		bne.w	F0_out_a
		move.l	d1,d1
		beq.w	ret_11
F0_out_a:	lea	EXCL_TB(pc),a0		;
		move.b	#$42,5(a0)		; Command Code
		andi.w	#$7F7F,d0		;
		move.w	d0,6(a0)		;
		andi.l	#$7F7F7F7F,d1		;
		move.l	d1,8(a0)		;
		move.b	#MIDI_EOX,12(a0)	; EOX

		moveq	#0,d0			; clear
		move.w	d0,Mem_DRVERR_FLG	; word
		move.l	d0,Mem_err_bit		; long

		moveq	#13-1,d7		; ready F0�`F7 size
;-----------------------------------------------;
;	Exclusive SCSP MIDI out Bufferring	;
; ready d7 : loop size				;
;-----------------------------------------------;
	global	EXCL_OUT
EXCL_OUT:	lea	EXCL_TB(pc),a1		;
		lea	_MIDI_OUT_BF(a6),a2	; MIDI OUT buffer
		move.w	_MIDI_OUT_WRPT(a6),d0	; word
		andi.w	#$3FF,d0		;
EXCL_OUT_lp:	move.b	(a1)+,(a2,d0.w)		; MIDI bufferring
		addq.w	#1,d0			;
		andi.w	#$3FF,d0		;
		dbra	d7,EXCL_OUT_lp		;
		move.w	d0,_MIDI_OUT_WRPT(a6)	; �X�V
ret_11:		rts

	global	EXCL_TB
EXCL_TB:	dc.l	$F0437900	; Excl status,YAMAHA ID,DIV,Target Dev ID
		dc.w	$0142		; Saturn ID,Command Code
		dc.l	0,0,0,0,0,0,0,0
		dc.l	0,0,0,0,0,0,0,0
;-----------------------------------------------------------------------;
;		�s�u�p �T�E���h�o�͂̃��m�����^�X�e���I�Ή�		;
; ready d0.b								;
;-----------------------------------------------------------------------;
		global	TV_mono_chg
TV_mono_chg:	move.b	d0,SND_OUT_ST(a6)	; save 
		bmi.w	to_mono
		;
		; <<<< MONO ---> STEREO >>>>
		;
to_stereo:	jsr	MIXER_wk_wr(pc)		; <<< Mixer data wr on SCSP >>>
		moveq	#0,d4			; clear
		lea	slot_work(a6),a4	;
		moveq	#slot_size-1,d7		; loop ( slot ) size
to_st_lp:	move.w	_sl_kanri(a4,d4.w),d5	; = �����Ǘ��ԍ��~200H
		move.b	sl_DISDLPAN(a4,d4.w),d6	; = original [DISDL],[DIPAN]
		lsr.w	#1,d4			;
		move.b	d6,SCSP_DISDLPN(a5,d4.w)	;
		add.w	d4,d4			;
		addi.w	#slot_wk_unit,d4
		dbra	d7,to_st_lp
		rts
		;
		; <<<< STEREO ---> MONO >>>>
		;
to_mono:	moveq	#0,d4
		moveq	#18-1,d7
to_mono_lp1:	andi.w	#$E0E0,SCSP_DISDLPN(a5,d4.w)	; DIPAN & EFPAN --> Center
		addi.w	#SCSP_slot_unit,d4
		dbra	d7,to_mono_lp1
		moveq	#14-1,d7
to_mono_lp2:	andi.b	#$E0,SCSP_DISDLPN(a5,d4.w)	; DIPAN ---> Center
		addi.w	#SCSP_slot_unit,d4
		dbra	d7,to_mono_lp2
		rts
;-----------------------------------------------------------------------;
;			�c�r�o�����J �cirect down load			;
;-----------------------------------------------------------------------;
	if	sw_MODEL_M
		global	DSP_LINK
DSP_LINK:
		move.b	Mem_DSP_FLG,d0	; DSP linker direct down load ?
		bpl.s	ret_00		; jump if no !
		andi.b	#$7F,d0		;
		move.b	d0,Mem_DSP_FLG	;
		lea	bs_TIFWK,a1	; Tool I/F work top addr

		move.w	OF_DSP_RBL(a1),d1	; = current RBL/RBP
		move.w	d1,d0		;
		andi.w	#$7F,d0		; = RBP
		swap	d0		;
		clr.w	d0		;
		lsr.l	#3,d0		; $0000,$2000,...,$FE000
		lsr.w	#7,d1
		andi.w	#3,d1		; = RBL 0,1,2,3
		addq.w	#1,d1		;       1,2,3,4
		move.l	#$2000,d2
		lsl.l	d1,d2		; = $4000,$8000,$10000,$20000
		add.l	d2,d0
		move.l	d0,DFL_INST_addr(a6)	; = �u���l�����G���A top addr.

		movea.l	d0,a0		; = �u���l�����G���A top addr.
		moveq	#32/2-1,d7	;
		moveq	#0,d0		;
DSP_LNK_CHK:	move.l	d0,(a0)+	;
		dbra	d7,DSP_LNK_CHK	;
ret_00:		rts
	endif
;-----------------------------------------------------------------------;
;			�c-�eilter �b�o�t ����				;
;-----------------------------------------------------------------------;
		global	DSP_CPU
DSP_CPU:	move.b	BIG_timer(a6),d0
		andi.b	#3,d0			; 8msec ?
		bne	DSP_CPU_exit
		clr.w	d7			;
		move.b	DFL_ELMNT_NO(a6),d7	; D-Filter Element�̐� 0�`32
; A.Miyazawa	{
;		beq.s	main_A			; jump if Element nothing
;	external	DSPEGLFO
;		jsr	DSPEGLFO(pc)		; ready d7.w
;}
main_A:
		move.b	EFCT_CHG_CNT(a6),d0	; Effect change exe mode ?
		beq	DSP_CPU_exit
		;-------------------------------;
		;     Effect change ������	;
		;-------------------------------;
		moveq	#16-1,d7		; loop size
		lea	SCSP_EFSDLPN(a5),a0	;
EFCT_CHG_lp:	move.b	(a0),d1
		andi.b	#$E0,d1
		beq.s	EFCT_CHG_0		; [EFSDL]��p���� Fade out
		subi.b	#$20,(a0)		;   ( CD-DA���͏��� )
EFCT_CHG_0:	lea	SCSP_slot_unit(a0),a0	;
		dbra	d7,EFCT_CHG_lp
		subq.b	#1,EFCT_CHG_CNT(a6)	;
		bne	DSP_CPU_exit		
		;-------------------------------;
		;    [MPRO] & [TEMP] fill NOP	;94/12/18
		;-------------------------------;
		lea	DSP_MPRO(a5),a0		;
		moveq	#0,d0
		move.w	#$600/4-1,d7		; loop size
DSP_nop_lp:	move.l	d0,(a0)+
		dbra	d7,DSP_nop_lp
		;-------------------------------;
		; ready a0,a2,d3		;
		;-------------------------------;
		movea.l	DSP_PRG_top(a6),a0	; = DSP Micro Prg top addr
		movea.l	DSP_RW_top(a6),a2	; = DSP Memory access area top
		move.l	DSP_RW_sz(a6),d3	; = DSP Memory access size
		;-------------------------------;
		; fill "$6000" 			;
		;	  into DSP access area 	;94/12/18
		;-------------------------------;
		move.l	#$60006000,d0		;95/02/03
		lsr.l	#4,d3
		beq.s	CTRL_5B_A
		subq	#1,d3
		move.w	d3,d3
		beq.s	CTRL_5B_A
FILL_6000:	move.l	d0,(a2)+
		move.l	d0,(a2)+
		move.l	d0,(a2)+
		move.l	d0,(a2)+
		dbra	d3,FILL_6000
CTRL_5B_A:
		;---------------------------------------;
		;	3D flag set			;
		;	edit by Y.Kashima 01/02/95	;
		;---------------------------------------;
		move.b	DL_3DM(a0),d0
		move.b	d0,bs_SIFWK+OF_3D_FLAG
		cmp.b	#$40,d0			;YAMAHA 3D use check
		bcs.s	?skip			;($40>d0)
		move.b	DL_3DC(a0),bs_SIFWK+OF_3DCOEF_PTR
		move.b	DL_3DA(a0),bs_SIFWK+OF_3DADRS_PTR
		bra.s	?skip2			;
?skip:
		move.b	DL_3DC(a0),bs_SIFWK+OF_QCOEF_PTR
?skip2:
		;-------------------------------;
		;  [EFREG] clear 95/03/17	;
		;-------------------------------;
		lea	DSP_EFREG(a5),a2	;
		moveq	#0,d0			;
		move.l	d0,(a2)+		;
		move.l	d0,(a2)+		;
		move.l	d0,(a2)+		;
		move.l	d0,(a2)+		;
		move.l	d0,(a2)+		;
		move.l	d0,(a2)+		;
		move.l	d0,(a2)+		;
		move.l	d0,(a2)+		;
		; ������������������������������������������������
		; ��	[RBL,COEF,MADRS,MPRO] write to SCSP	��
		; ������������������������������������������������
		move.l	DSP_RW_top(a6),d5	; 0000,2000,4000,....,FE000
		move.b	DL_RBL(a0),d0		; = RBLEN ( 0,1,2,3 )
		andi.w	#3,d0			; 0000 0000 0000 00xx
		lsl.w	#7,d0			; 0000 000x x000 0000
		lsl.l	#3,d5			; 
		swap	d5			; 0000 0000 0nnn nnnn
		or.w	d5,d0			; 0000 000x xnnn nnnn
		move.w	d0,RG_RBP(a5)		; write new current RBL/RBP
		move.w	d0,stack_RBLP(a6)	;
		lea	DL_CEF(a0),a2		; = dest. EXB COEF�` data top
		lea	DSP_COEF(a5),a3		;
		move.w	#$500/4-1,d7		; loop size
CTRL_5B_EX_1:	move.l	(a2)+,(a3)+		; wr [COEF],[MADR],[MPRO]
		dbra	d7,CTRL_5B_EX_1		;
		; ������������������������������������������������
		; ���W���e�[�u���]�� write to DSP R/W area	��
		; �� d5:RBP data				��
		; �� d4:COEF table size : �m�~A00H		��
		; �� a2:EXB COEF table top			��
		; ������������������������������������������������
		moveq	#0,d4			;
		move.b	DL_NCT(a0),d4		; = COEF table��
		move.b	DL_RBL(a0),d2		; = RBLEN ( 0,1,2,3 )
		move.l	#$4000,d3		;
		andi.w	#3,d2			;
		beq.s	CTRL_5B_5		; jump if R/W area = $4000
		lsl.l	d2,d3
CTRL_5B_5:	; d3.l = $4000,$8000,$10000,$20000 ( �x���̈�T�C�Y )
		swap	d5			; = 007F0000
		lsr.l	#3,d5			; = 000FE000 ( RBP�~2000H )
		add.l	d3,d5			; = �u���l�����G���A top addr.
		move.l	d5,DFL_INST_addr(a6)	;
		movea.l	d5,a0			; = �W���e�[�u�� top addr.
		moveq	#0,d0			;
		moveq	#40H/4-1,d7		;
CTRL_5B_EX_6:	move.l	d0,(a0)+		; = �u���l�����G���A clear
		dbra	d7,CTRL_5B_EX_6		;
		lsr.l	#2,d4			;
		beq.s	CTRL_5B_EX_5		; jump if COEF table nothing
		subq.w	#1,d4			; loop size
CTRL_5B_EX_4:	move.l	(a2)+,(a0)+		; COEF table �]��
		dbra	d4,CTRL_5B_EX_4		;
CTRL_5B_EX_5:	move.l	a2,DFL_ELMNT_addr(a6)	; EXB ELEMENT table top

		jmp	MIXER_wk_wr(pc)


DSP_CPU_exit:	rts
;-----------------------------------------------------------------------;
;			����(PLFO/PEG/BEND)����				;
; �ŏ��̂Smsec : slot#00 �` #07 ������					;
;   ���̂Smsec : slot#08 �` #15 ������					;
;   ���̂Smsec : slot#16 �` #23 ������					;
; �@���̂Smsec : slot#23 �` #31 ������					;
; �P���̃X���b�g�͂P�Umsec���Ƃɐ��䂳���				;
;-----------------------------------------------------------------------;
		global	slot_ctrl
slot_ctrl:	; <<< don't destroy a4/d4.w/d7.w >>>

slot_ctrl_pt:	move.w	#$300,d4		;
		addi.w	#$100,d4
		andi.w	#$300,d4
		move.w	d4,slot_ctrl_pt+2	; d4 = $000,$100,$200,$300
		lea	slot_work(a6),a4	;
		moveq	#slot_size/4-1,d7	; loop ( slot ) size
slot_ctrl_lp:	move.w	d4,d5			; = 0,$20,$40,..,$3E0
		add.w	d5,d5			; = 0,$40,$80,..,$7C0
		move.b	PSPN(a4,d5.w),d0	; PCM_flg : PCM �Đ� mode ?
		bmi.w	next_slot		; jump if Yes
	if	ENGN
		move.b	sl_flag1(a4,d5.w),d0	; Engine busy ?
		bmi.w	next_slot		; jump if Yes
	endif
;@@		move.b	EG_Value(a4,d5.w),d0	; EG�l = off ?
;@@		beq.w	next_slot		; jump if Yes

		add.w	d4,d4			; = 0,$40,..,$7C0

		; pitch bend

		clr.w	d5			;
		move.b	sl_MIDI(a4,d4.w),d5	; = MIDI ch# (0�`0FH)
		lsl.w	#5,d5			; *20H
		or.w	_sl_kanri(a4,d4.w),d5	; + �����Ǘ��ԍ��~200H
		move.w	knr_PBend_BF(a6,d5.w),d5	; = Pitch Bend data

		move.l	sl_layer_adr(a4,d4.w),d0	;ready
		bne.w	slot_ctrl_3
		lsr.w	#1,d4			; = 0,$20,$40,..,$3E0
		bra.w	next_slot

slot_ctrl_3:	movea.l	d0,a2
		btst.b	#PLON_flg,sl_flag1(a4,d4.w)
		beq.w	slot_ctrl_1		; jump if if PLFO off
		jsr	add_PLFO(pc)		; return d5.w = + PLFO freq
slot_ctrl_1:
		btst.b	#PEON_flg,sl_flag1(a4,d4.w)
		beq.w	slot_ctrl_2		; jump if if PEG off
		jsr	add_PEG(pc)		; return d5.w = + PEG freq
slot_ctrl_2:
		cmp.w	sl_tune(a4,d4.w),d5	;
		bne.w	slot_ctrl_4
		lsr.w	#1,d4			; = 0,$20,$40,..,$3E0
		bra.w	next_slot		;
slot_ctrl_4:	move.w	d5,sl_tune(a4,d4.w)	;

; A.Miyazawa	{

	;;	jsr	tune_wr2(pc)		; ready d5.w

			.extern		OCT_TB
			.extern		FNSTB

		clr.w	d6
		clr.w	d2
		move.b	LY_fine_tune(a2),d6
		ext.w	d6			; = FF80 �` 0000 �` 007E
		add.w	d6,d5			;
		move.w	d5,d6			; d5.b = �Z���g��
		lsr.w	#8,d6			; d6.b = ����
		move.b	sl_note(a4,d4.w),d2	;
		add.w	d2,d6			; + MIDI note#
		addi.w	#96,d6			; + 12*8 ( 8octav )
		move.b	LY_base_note(a2),d2	;
		sub.w	d2,d6			; - base note

		andi.w	#$FF,d6			; = ����
		lea	OCT_TB(pc),a2		;
		move.b	(a2,d6.w),d6		; �c7�`�c4 : Octv 0 �` F
						; �c3�`�c0 : ���� 0 �` B
		lsl.w	#7,d6			; 0xxx xxxx x000 0000
		move.w	d6,d2
		andi.w	#$7800,d6		; = [OCT] : 0xxx x000 0000 0000
		add.w	d2,d2			; = xxxx xxxx 0000 0000
		move.b	d5,d2			; = xxxx xxxx ssss ssss
		andi.w	#$0ffe,d2		; = $000 �` $BFE
		lsr.w	#1,d2			; = $000 �` $5FF
		lea	FNSTB(pc),a2		;
		clr.w	d5			;
		move.b	(a2,d2.w),d5		; = [FNS] low 8bits only
		cmpi.w	#$3de/2,d2
		bcs.s	?100

		cmpi.w	#$706/2,d2
		bcs.s	?200

		cmpi.w	#$9b2/2,d2
		bcs.s	?300

		addi.w	#$100,d5		; $9B2 �` $BFE cent = +$300
	?300:	addi.w	#$100,d5		; $706 �` $9B0 cent = +$200
	?200:	addi.w	#$100,d5		; $3DE �` $704 cent = +$100
	?100:					; $000 �` $3DC cent = +$000

	; d5.w = SCSP [FNS] data $000 �` $3FF

		or.w	d5,d6			; [OCT]+[FNS]
		lsr.w	#1,d4			; = $00,$20,....,$3E0
		move.w	d6,SCSP_OCTFNS(a5,d4.w)

;		}

next_slot:	addi.w	#SCSP_slot_unit,d4	; = 0,$20,$40,..,$3E0
		dbra	d7,slot_ctrl_lp		;
		rts
; ������������������������������������������������������������������������
; ��	     <<<< �P�ʎ��ԓ�����̂o�k�e�n�ɂ������Z�o >>>>		��
; �ņ���������������������������������������������������������������������
; ���� 	       d0.l [----] : work					��
; ��  	       d1.l [----]						��
; ���� 	       d2.l [----] : work					��
; ��  	       d3.l [----]						��
; �����y���́z d4.w [hold] : slot offset ( $00,$20 �` $7C0 )		��
; ����	       d5.l [dstr] : work					��
; ���� 	       d6.l [----] : work		 			��
; ��  	       d7   [Hold] : loop count					��
; ��	       a0   [----]						��
; ��  	       a1   [----]						��
; ��	       a2   [hold]						��
; ��	       a3   [----]						��
; �����y���́z a4   [hold] : slot work top 				��
; ��	       a5   [----]		 				��
; ��	       a6   [----]						��
; �����y�o�́z d5.w [dstr] : ���� XXYYH ( XXH = ���� , YYH = �Z���g )	��
; ������������������������������������������������������������������������

			.public		add_PLFO
add_PLFO:
		move.w	PLFO_Delay(a4,d4.w),d0	; 1msec counter
		beq.w	plfo_action
		
		subi.w	#$48,d0
		bcc.s	?200
		
		clr.w	d0
	?200:	move.w	d0,PLFO_Delay(a4,d4.w)
		rts

plfo_action:
		move.l	a4,-(sp)
		adda.w	d4,a4

		move.w	PLFO_HTCNT_wk(a4),d0
		subi.w	#_4msec*4,d0
		bpl.w	?300

		move.w	d0,d2
		add.w	PLFO_HTCNT_bs(a4),d0
		move.w	d0,PLFO_HTCNT_wk(a4)
		moveq	#0,d1				; = FRQR_bs
		move.w	PLFO_FDCNT(a4),d0
		beq.s	?400				; jump if FDCT = 0

		subq.w	#1,d0
		move.w	d0,PLFO_FDCNT(a4)
		move.l	PLFO_FRQR_bs(a4),d1
		neg.l	d1
		move.l	d1,PLFO_FRQR_bs(a4)
		neg.l	d1
	?400:
		move.l	PLFO_FRQR_wk(a4),d6
		move.l	PLFO_cent(a4),d0		; (1msec)
		addi.w	#_4msec*4,d2			; = 0,1,2,3
		andi.w	#0003,d2
		add.w	d2,d2
		jmp	?jump_table(pc,d2.w)

	?jump_table:
		bra.s	?d2_0
		bra.s	?d2_1
		bra.s	?d2_2
	;	bra.s	?d2_3

	?d2_3:
		add.l	d6,d0				; Cent    <-- �{ FRQR_wk
		add.l	d6,d0				; Cent    <-- �{ FRQR_wk
		add.l	d6,d0				; Cent    <-- �{ FRQR_wk
		add.l	d1,d6				; FRQR_wk <-- �{ FRQR_bs
		neg.l	d6
		bra.s	?d2_0C
	?d2_2:
		add.l	d6,d0				; Cent    <-- �{ FRQR_wk
		add.l	d6,d0				; Cent    <-- �{ FRQR_wk
		add.l	d1,d6				; FRQR_wk <-- �{ FRQR_bs
		neg.l	d6
		bra.s	?d2_0B
	?d2_1:
		add.l	d6,d0				; Cent    <-- �{ FRQR_wk
		add.l	d1,d6				; FRQR_wk <-- �{ FRQR_bs
		neg.l	d6
		bra.s	?d2_0A
	?d2_0:
		add.l	d1,d6				; FRQR_wk <-- �{ FRQR_bs
		neg.l	d6
		add.l	d6,d0				; Cent    <-- �{ FRQR_wk
	?d2_0A:	add.l	d6,d0				; Cent    <-- �{ FRQR_wk
	?d2_0B:	add.l	d6,d0				; Cent    <-- �{ FRQR_wk
	?d2_0C:	add.l	d6,d0				; Cent    <-- �{ FRQR_wk
		move.l	d6,PLFO_FRQR_wk(a4)
		move.l	d0,PLFO_cent(a4)
		bra.s	?500
	?300:
		move.w	d0,PLFO_HTCNT_wk(a4)
		move.l	PLFO_FRQR_wk(a4),d6
		add.l	d6,d6				; (16msec)
		add.l	d6,d6				; (8msec)
		add.l	d6,d6				; (4msec)
		add.l	d6,d6				; (2msec)
		add.l	d6,PLFO_cent(a4)		; (1msec)
	?500:
		add.w	PLFO_cent(a4),d5
		move.l	(sp)+,a4
		rts


;===============================================;
;  �o�d�f����i������,Key off����܂ށj		;
;	Pitch EG ���� addition			;
;�y���́z a2 : destination layer data top addr	;
;         a4 : slot work address                ;
;	  d4.w : slot work offset
;	  d5.w : Pitch
;�y�o�́z d5.w : Pitch
;�y�j��z d0.l/d1.l/d3.w/a1			;
;===============================================;
		external	EXPTB,get_LEVEL,get_EXPTB2
		global	PEG_1,PEG_2,PEG_3,PEG_4,PEG_5,PEG_6,PEG_7
		global	add_PEG
add_PEG:
		move.l	a4,-(sp)
		adda.w	d4,a4

		moveq	#0,d0
		move.b	PEG_SEG(a4),d0			; get d0.b = SEG number
		andi.w	#0007,d0
		add.w	d0,d0
		add.w	d0,d0
		lea	?jump_table(pc),a1
		movea.l	(a1,d0.w),a1
		jmp	(a1)

	?jump_table:
		dc.l	end_of_function
		dc.l	PEG_1				; DLY
		dc.l	PEG_2				; AR
		dc.l	PEG_3				; DR
		dc.l	PEG_4				; SR
		dc.l	PEG_5				; SR key off �҂�
		dc.l	PEG_6				; RR
		dc.l	PEG_7


		;===============================;
		; ready d0.w = 0,4,8,C,..	;
		;===============================;
PEG_1:		; <<<<<<<<<<< Delay >>>>>>>>>>>>;
PEG_2:		; <<<<<<<<<<< Attack >>>>>>>>>>>;
PEG_3:		; <<<<<<<<<<< Decay >>>>>>>>>>>>;
PEG_4:		; <<<<<<<<<<< Sustain >>>>>>>>>>;
PEG_6:		; <<<<<<<<<<< Release >>>>>>>>>>;

		move.l	PEG_RATE(a4),d3			; set �X���^16msec
		bne	ADD_RATE

		subi.w	#$48,PEG_dly_cnt(a4)		; 95/08/31
		bcs	NEXT_SEG
		bra	end_of_function

			.global		NEXT_SEG
NEXT_SEG:
		movea.l	PEG_addr(a4),a1
		lsr.w	#1,d0				; = 0,2,4,6,8,..
		adda.w	d0,a1				; ready a1 : for [DR]
		cmpi.w	#8,d0				; Sustain ?
		bne	PEG_123
		addq.b	#1,PEG_SEG(a4)			; set next Segment
		move.l	PEG_level(a4),d0
		move.l	d0,PEG_cent(a4)

PEG_5:		; <<<<<<<<<<< Sustain �I�� Key-off �҂� >>>>>>>>>>;
PEG_7:		; <<<<<<<<<<< Release �I�� >>>>>>>>>>;
		add.w	PEG_cent(a4),d5
		bra	end_of_function

PEG_123:	addq.b	#1,PEG_SEG(a4)			; set next Segment
		clr.w	d0
		move.b	(a1)+,d0			; = [DR]
		beq	RATE_00				; jump if [DR]�� 0

		cmpi.b	#$80,d0
		beq	RATE_80				; jump if [DR]=80H

	; [AR/DR]��0 & ��80H �X���L��

			.global		RATE_ON
RATE_ON:
		bsr	get_EXPTB2			; return d0.w:-$3F01�`+$3F01

		swap	d0
		clr.w	d0
		asr.l	#2,d0				; for 16msec
		move.l	d0,PEG_RATE(a4)			; set �X���^16msec
		move.l	PEG_level(a4),d0
		move.l	d0,PEG_cent(a4)

		move.b	(a1),d0				; = [DL]
		bsr	get_LEVEL			; return d0.w:-$5FFF�`+$5FFF

		move.w	d0,PEG_level(a4)		; set ���B Level high word
		moveq	#0,d0
		move.w	d0,PEG_level+2(a4)		; set ���B Level low word
		move.w	d0,PEG_dly_cnt(a4)		; = counter (1msec)
		add.w	PEG_cent(a4),d5
		bra	end_of_function


	; <<<< [DR]��80H >>>>
RATE_80:
		move.b	(a1),d0				; [DL] = Level
		bsr	get_LEVEL			; return d0.w:-$5FFF�`+$5FFF

		move.w	d0,PEG_cent(a4)			; set Level high word
		move.w	d0,PEG_level(a4)		; set Level high word
		moveq	#0,d0
		move.w	d0,PEG_cent+2(a4)		; set Level low word
		move.w	d0,PEG_level+2(a4)		; set Level low word
		move.w	d0,PEG_dly_cnt(a4)		; = counter (1msec)
		move.l	d0,PEG_RATE(a4)
		bra	add_PEG

	; <<<< [DR]��00H >>>>

			.global		RATE_00
RATE_00:
		move.b	(a1)+,d0			; [DL] = counter
		add.w	d0,d0				; [AL]�̌p��
		lea	EXPTB(pc),a0			;
		move.w	(a0,d0.w),d0			;
		move.w	d0,PEG_dly_cnt(a4)		; = counter (1msec)

		moveq	#0,d0
		move.l	d0,PEG_RATE(a4)
		move.l	PEG_level(a4),d0
		move.l	d0,PEG_cent(a4)

		add.w	PEG_cent(a4),d5
		bra	end_of_function

			.global		ADD_RATE
ADD_RATE:						; d3.l = PEG_RATE
		move.l	PEG_cent(a4),d0
		add.l	d3,d0
		move.l	d0,PEG_cent(a4)
		move.l	PEG_level(a4),d1
		moveq	#0,d2
		move.w	#$8000,d2
		swap	d2
		add.l	d2,d0				; new [PEG_cent]
		add.l	d2,d1				;     [PEG_level]
		move.l	d3,d3
		bpl	RATE_pls
RATE_mi:
		cmp.l	d0,d1
		bcc	RATE_NEXT

		add.w	PEG_cent(a4),d5
		bra	end_of_function
RATE_pls:
		cmp.l	d1,d0
		bcc	RATE_NEXT

		add.w	PEG_cent(a4),d5
		bra	end_of_function
RATE_NEXT:
		move.w	PEG_level(a4),d0		; set Level high word
		move.w	d0,PEG_cent(a4)			; set Level high word
		moveq	#0,d0
		move.w	d0,PEG_cent+2(a4)		; set Level low word
		move.w	d0,PEG_dly_cnt(a4)		; = counter (1msec)
		move.l	d0,PEG_RATE(a4)
		move.b	PEG_SEG(a4),d0			; set next Segment
		andi.w	#7,d0
		add.w	d0,d0
		add.w	d0,d0
		bra	NEXT_SEG

end_of_function:
		move.l	(sp)+,a4
		rts

;************************************************************************
;									*
;		YM-8302 ( MCS-1 & 2 ) ��������				*
;									*
;************************************************************************

; A.Miyazawa	{

;	external	MIDI_d0_ctrl
;	if	sw_MODEL_M
;		global	level_5,level_6
;level_6:	movem.l	d0-d7/a0-a6,-(SP)		; reg. push
;		lea	bs_PWKTP,a6			; 68K Prg work top address
;		ori.b	#%10000000,LED_status(a6)
;		move.b	#MIDI_LED_speed,RCVA_LED_cnt(a6)
;		lea	IO_MIDI1+1,a0			; ready a0 @005
;		move.b	R00(a0),d0		; IRQ vector read
;		andi.w	#00011110B,d0		; = 0,2,4,6,8,A,C,E
;		cmpi.b	#$0A,d0			; FIFO-Rx ready ?
;		bne.w	level_6_exit		; jump if no
;		move.b	#3,R01(a0)
;		move.b	Rx4(a0),d0		; R34 = (RD) FIFO-Rx status
;		andi.b	#01110000B,d0		; error ?
;		beq.w	MIDI_RX_pass
;		move.b	Rx6(a0),d0		; dammy read
;		bra.w	level_6_exit
;MIDI_RX_pass:	move.b	Rx6(a0),d0		; R36 = (RD) FIFO-Rx data
;		bsr.w	MIDI_d0_ctrl			;
;level_6_exit:	movem.l	(SP)+,d0-d7/a0-a6		; reg. pop
;		rte
;		;---------------------------------------;
;level_5:	movem.l	d0/a0/a6,-(SP)			; reg. push
;		lea	bs_PWKTP,a6			; 68K Prg work top address
;		ori.b	#%01000000,LED_status(a6)
;		move.b	#MIDI_LED_speed,RCVB_LED_cnt(a6)
;		lea	IO_MIDI2+1,a0			; ready a0 @005
;		move.b	R00(a0),d0			; IRQ vector read
;		move.b	#3,R01(a0)			;
;		move.b	Rx6(a0),d0			; dammy read
;		movem.l	(SP)+,d0/a0/a6			; reg. pop
;		rte
;	else
;		global		level_5
;level_5:	movem.l	d0-d7/a0-a6,-(SP)		; reg. push
;		lea	IO_SCSP,a5			; �Œ�
;		lea	bs_PWKTP,a6			; 68K Prg work top address
;		move.b	RG_MIBUF(a5),d0
;		bsr.w	MIDI_d0_ctrl			;
;		movem.l	(SP)+,d0-d7/a0-a6		; reg. pop
;		rte
;	endif
;

			.if sw_MODEL_M

			.extern		MIDI_d0_ctrl
			.public		level_5
			.public		level_6

level_6:	movem.l	d0-d7/a0-a6,-(sp)
		lea	bs_PWKTP,a6			; 68K Prg work top address
		ori.b	#%10000000,LED_status(a6)
		move.b	#MIDI_LED_speed,RCVA_LED_cnt(a6)

		movea.l	#IO_MIDI1+1,a0			; ready a0 @005

		move.b	R00(a0),d0			; IRQ vector read
		andi.w	#00011110B,d0			; = 0,2,4,6,8,A,C,E
		cmpi.b	#$0A,d0				; FIFO-Rx ready ?
		bne.s	?level_6_exit			; jump if no

		move.b	#3,R01(a0)
		move.b	Rx4(a0),d0			; R34 = (RD) FIFO-Rx status
		andi.b	#01110000B,d0			; error ?
		beq.s	?MIDI_RX_pass

		move.b	Rx6(a0),d0			; dammy read
		bra.s	?level_6_exit

	?MIDI_RX_pass:
		move.b	Rx6(a0),d0			; R36 = (RD) FIFO-Rx data
		bsr.w	MIDI_d0_ctrl			;

	?level_6_exit:
		movem.l	(sp)+,d0-d7/a0-a6
		rte
level_5:
		movem.l	d0/a0/a6,-(sp)
		lea	bs_PWKTP,a6			; 68K Prg work top address
		ori.b	#%01000000,LED_status(a6)
		move.b	#MIDI_LED_speed,RCVB_LED_cnt(a6)
		lea	IO_MIDI2+1,a0			; ready a0 @005
		move.b	R00(a0),d0			; IRQ vector read
		move.b	#3,R01(a0)			;
		move.b	Rx6(a0),d0			; dammy read
		movem.l	(sp)+,d0/a0/a6
		rte

			.else

			.extern		MIDI_d0_ctrl
			.public		level_5

level_5:	movem.l	d0-d7/a0-a6,-(SP)		; reg. push
	;;	lea	IO_SCSP,a5			; �Œ�
	;;	lea	bs_PWKTP,a6			; 68K Prg work top address
	;;	move.b	RG_MIBUF(a5),d0
	;;	bsr.w	MIDI_d0_ctrl			;
		movem.l	(SP)+,d0-d7/a0-a6		; reg. pop
		rte

			.endif

;		}



; ������������������������������������������������
; �� �r�b�r�o�^�C�}�[�a�i�r�d�p�f�R�[�h�j������	��
; ������������������������������������������������
		global	level_2
level_2:
		movem.l	d0-d7/a0-a6,-(SP)	; reg. push

	if	SCSI_RAM_chk
		moveq	#$80/4-1,d7		; loop size
		lea	$800,a0			;
		lea	$F3FE00,a1		;
SCSI_chk_lp:	move.l	(a1)+,d0
		move.l	(a0),d1
		move.l	d0,(a0)+
		cmp.l	d0,d1
		beq.s	SCSI_chk_1
		nop
SCSI_chk_1:	dbra	d7,SCSI_chk_lp
	endif

		lea	IO_SCSP,a5		; �Œ�
		lea	bs_PWKTP,a6		; 68K Prg work top address

;xxxx		move.w	RG_SCIPD(a5),d0		;
;xxxx		andi.w	#$80,d0			;
;xxxx		beq.s	level_2_exit		;

		move.w	#TIMB_VL,RG_TBCTL(a5)	; timer-B counter set
		move.w	#INT_RST,RG_SCIRE(a5)	; timer-B reset
		addi.w	#SEQ_LED_speed,SEQ_LED_cnt(a6)	; <<< �r�d�p�𓀊��� >>>
		addi.w	#$DA,FE_count(a6)	;
		addq.w	#1,COMHIS_cnt(a6)	; HOST Command HISTORY counter
		jsr	xseq_2msec(pc)		; [SDDRV.ASM]

	if	SW_TITAN
		jsr	titan_coin(pc)
	endif
level_2_exit:	movem.l	(SP)+,d0-d7/a0-a6		; reg. pop
		rte

	if	SW_TITAN
titan_coin:	lea	TTN_SW_DRAM,a0
		tst.b	TTN_flag68(a0)
		beq.s	sw_ex
		
		move.b	#1,TTN_execute(a0)
		move.w	IO_PORT_C,d0		; 94/12/28

		move.b	d0,d1
		and.b	#$3F,d0
		tst.b	TTN_coin_sw_md(a0)
		beq.s	sw_01
		
		lsl.b	#4,d1
		bra.s	sw_00
sw_01:
		lsl.b	#2,d1
sw_00:
		and.b	#$C0,d1
		or.b	d1,d0
		
		move.b	TTN_old_sw(a0),d1
		move.b	d0,TTN_old_sw(a0)
		eor.b	d0,d1
		and.b	d0,d1
		rol.b	#4,d1
		and.w	#$3C,d1
		move.l	swcnttbl(pc,d1.w),d1
		add.l	d1,TTN_off_count(a0)
		move.b	#0,TTN_execute(a0)		; 94/12/28
sw_ex:
		rts

swcnttbl:	dc.l	00000000H,00000100H,00000001H,00000101H
		dc.l	01000000H,01000100H,01000001H,01000101H
		dc.l	00010000H,00010100H,00010001H,00010101H	; 94/12/28
		dc.l	01010000H,01010100H,01010001H,01010101H
		
		
	endif

;--------------------------------------------------------------------------
;--------------------------------------------------------------------------
	if	sw_MODEL_M
MAC_MIXER:
		move.b	BIG_timer(a6),d0	; ���P�Omsec�̊Ԋu�������Ȃ���
		andi.b	#$0F,d0			; MONO �̂Ƃ��m�C�Y���o��B
		bne	MAC_MIXER_pass		; 

		move.l	_activ_MIX_BNK(a6),d0	; = SCSPBIN top address
		beq	MAC_MIXER_pass		; 
		movea.l	d0,a0			;
		move.w	_activ_MIX_NO(a6),d0	; ready Mixer#
		movea.l	a0,a1			;
		adda.w	BIN_VL(a0),a1		; = V-L data top
		adda.w	BIN_MIX(a0),a0		; = MIXER top addr
		add.w	d0,d0			;
		adda.w	d0,a0			; +  2*Mixer#
		lsl.w	#3,d0			; + 16*Mixer#
		adda.w	d0,a0			; = desti. Mixer addr
		cmpa.l	a1,a0			;
		bcc	MAC_MIXER_pass		;
		movea.l	a0,a1			;
		lea	mixer_wk_edt(a6),a2	;
		cmpm.l	(a1)+,(a2)+		;
		bne.s	MAC_MIXER_wr		;
		cmpm.l	(a1)+,(a2)+		;
		bne.s	MAC_MIXER_wr		;
		cmpm.l	(a1)+,(a2)+		;
		bne.s	MAC_MIXER_wr		;
		cmpm.l	(a1)+,(a2)+		;
		bne.s	MAC_MIXER_wr		;
		cmpm.w	(a1)+,(a2)+		;
		beq	MAC_MIXER_pass		;
MAC_MIXER_wr:	lea	mixer_wk_edt(a6),a2	;
		lea	mixer_wk_SCSP(a6),a1
		move.l	(a0),(a1)+
		move.l	(a0)+,(a2)+
		move.l	(a0),(a1)+
		move.l	(a0)+,(a2)+
		move.l	(a0),(a1)+
		move.l	(a0)+,(a2)+
		move.l	(a0),(a1)+
		move.l	(a0)+,(a2)+
		move.w	(a0),(a1)+
		move.w	(a0)+,(a2)+

		lea	mixer_wk_SCSP(a6),a0
		moveq	#18-1,d7		; loop size
		move.b	SND_OUT_ST(a6),d4	; MONO/STEREO status
		bmi.s	MAC_MIXER_MONO
		; <<<< Stereo mode >>>>
		clr.w	d4
MAC_MIX_wr1_lp:	move.b	(a0)+,SCSP_EFSDLPN(a5,d4.w)
		addi.w	#SCSP_slot_unit,d4
		dbra	d7,MAC_MIX_wr1_lp
		rts
		; <<<< MONO mode >>>>
MAC_MIXER_MONO:	clr.w	d4
MAC_MIX_wr2_lp:	move.b	(a0)+,d0
		andi.b	#$E0,d0			; --> Center
		move.b	d0,SCSP_EFSDLPN(a5,d4.w)
		addi.w	#SCSP_slot_unit,d4
		dbra	d7,MAC_MIX_wr2_lp
MAC_MIXER_pass:	rts
	endif
;--------------------------------------------------------------------------
	if	SCSI_vctr_chg

TRGTSCSI_trans:
		move.w	#(SCSI_e-SCSI_s)/4-1,d7	; loop size
		lea	SCSI_s(pc),a0
		lea	SCSI_top,a1
TRGTSCSI_TRLP:	move.l	(a0)+,(a1)+
		dbra	d7,TRGTSCSI_TRLP
		rts
	include	TRGTSCSI.ASM
	endif
;-----------------------------------------------------------------------;
;			SCSP MIDI on/off check				;
;-----------------------------------------------------------------------;
	external	MIDI_ctrl_pt
	external	Rireki_pt
OF_HNDSHK_flg_chk:
		move.l	#$4E714E71,d1		; = NOP
		;-------------------------------------------------------;
		;   HOST I/F Hand Shake check				;
		;-------------------------------------------------------;
		move.b	bs_SIFWK+OF_HNDSHK_flg,d0	; HOST I/F H-shake flag
		bmi.s	init_01			; bit 7 = H ?
		lea	main_2_pt(pc),a0	;
		move.l	d1,(a0)+		; = nop,nop
		move.l	d1,(a0)+		; = nop,nop
		;-------------------------------------------------------;
		;  68k ���� MIDI �� [MIDI_OUT_BF] �ւ� buffering 	;
		;-------------------------------------------------------;
init_01:	add.b	d0,d0			; bit 6 = H ?
		bmi.s	init_02			; jump if "H"
		lea	MIDI_ctrl_pt(pc),a0	;
		move.l	d1,(a0)			; = nop,nop
		;-------------------------------------------------------;
		; 68k ���� HOST Command �� [MIDI_OUT_BF] �ւ� buffering;
		;-------------------------------------------------------;
init_02:	add.b	d0,d0			; bit 5 = H ?
		bmi.s	init_04			; jump if "H"
		lea	Rireki_pt(pc),a0	;
		move.l	d1,(a0)			; = nop,nop
		;-------------------------------------------------------;
		;	Error ���� [MIDI_OUT_BF] �ւ� buffering	;
		;-------------------------------------------------------;
init_04:	add.b	d0,d0			; bit 4 = H ?
		bmi.s	init_03			; jump if "H"
		lea	F0_out_pt(pc),a0	;
		move.l	d1,(a0)			; = nop,nop
		;-------------------------------------------------------;
		; Active Sensing [FEH] �� [MIDI_OUT_BF] �ւ� buffering	;
		;-------------------------------------------------------;
init_03:	add.b	d0,d0			; bit 3 = H ?
		bmi.s	init_05			; jump if "H"
		lea	FE_out_pt(pc),a0	;
		move.l	d1,(a0)			; = nop,nop
		;-------------------------------------------------------;
init_05:	add.b	d0,d0			; bit 2 = H ?
		;-------------------------------------------------------;
		; CPU ���� over [F4H] �� [MIDI_OUT_BF] �ւ� buffering	;
		;-------------------------------------------------------;
init_06:	add.b	d0,d0			; bit 1 = H ?
		bmi.s	init_07			; jump if "H"
		lea	F4_out_pt(pc),a0	;
		move.l	d1,(a0)			; = nop,nop
		;-------------------------------------------------------;
		;	MIDI OUT Enable ?				;
		;  68k �ɂ�� [MIDI_OUT_BF] �� �O���ւ� MIDI out	;
		;-------------------------------------------------------;
init_07:	move.b	bs_SIFWK+OF_HNDSHK_flg,d0	; HOST I/F H-shake flag
		andi.b	#$7F,d0

; A.Miyazawa	{

	;;	bne.s	ret_13
	;;	lea	main_8_pt(pc),a0	;
	;;	move.l	d1,(a0)			; = nop,nop

;		}

ret_13:		rts
;-----------------------------------------------------------------------;
;   Carriage Return code check ( SDDRVS.TSK �� Media�Ԃł̕���check )	;
;-----------------------------------------------------------------------;
	global	CR_check
CR_check:	lea	CR_TB(pc),a0
		cmpi.b	#$0D,(a0)+
		bne.w	CR_chk_er
		cmpi.b	#$0A,(a0)+
		bne.w	CR_chk_er
		cmpi.b	#$0D,(a0)+
		bne.w	CR_chk_er
		cmpi.b	#$0A,(a0)+
		beq.w	ret_34
CR_chk_er:	bset.b	#ERRb20,Mem_err_bit+1	; SDDRVS.TSK �� Media�Ԃł̕����~�X
ret_34:
		; <<<<< �O�� SCSP-MIDI ���͂̔����Ǘ��ԍ��̐ݒ� >>>>>

	external	INT_MIDI_pt
		move.b	bs_SIFWK+OF_EXTMIDI_No,d0	; 4E2H:0,1,2,3,4,5,6,7
		subq.b	#1,d0				;
		andi.w	#7,d0				;      7,0,1,2,3,4,5,6
		lsl.b	#5,d0				; = E0,00,20,40,..,C0

; A.Miyazawa	{

	;;	lea	INT_MIDI_pt(pc),a0
	;;	move.b	d0,3(a0)

		move.b	d0,(INT_MIDI_pt+3)		; ##"int_midi.asm"

;		}

		rts

CR_TB:		dc.b	$0D,$0A,$0D,$0A
;--------------------------------------------------------------------------
;	�G���W������ ---> see ENGINE.DOC
;--------------------------------------------------------------------------
	if	ENGN

ENGN_P1_rpm	equ	0+2	; P1 ��]��	
ENGN_P1_accl	equ	4	; P1 �A�N�Z���J�x
ENGN_P1_spd	equ	8	; P1 ���x	
ENGN_P1_gear	equ	$0C	; P1 �M�A	
ENGN_P1_SH	equ	$10	; P1 SH-Status	
ENGN_P1_68K	equ	$14	; P1 68k-Status	
ENGN_TB_addr	equ	$18	; Engine Table addr
ENGN_TB_sz	equ	$1C	; Engine Table size
ENGN_P2_rpm	equ	$20+2	; P2 ��]��	
ENGN_P2_accl	equ	$24	; P2 �A�N�Z���J�x
ENGN_P2_spd	equ	$28	; P2 ���x	
ENGN_P2_gear	equ	$2C	; P2 �M�A	
ENGN_P2_SH	equ	$30	; P2 SH-Status	
ENGN_P2_68K	equ	$34	; P2 68k-Status	
;		equ	$38
;		equ	$3C

;-----------------------------------------------;<<< a3 >>>
ENGN_wk_rpm	equ	0	; byte 0 �` $22
ENGN_wk_accl	equ	1	; byte 0 �` $0F
;		equ	2	; byte
;		equ	3	; byte
ENGN_wk_TONE	equ	4	; long	Engine SCSPBIN top
;-----------------------------------------------;<<< a2 >>>
ENGN_TB_unit	equ	$80
ENGN_TB_Layer	equ	0	; long
ENGN_TB_d4	equ	4	; word	$300,$320,...,$3E0
ENGN_TB_flg	equ	6	; byte 	bit7 = PCM on
ENGN_TB_rpm	equ	7	;
ENGN_TB_accl	equ	$2A	;
ENGN_TB_pitch	equ	$3A	;
;-----------------------------------------------;<<< a0 >>>
ENGN_slot_TB:	dc.w	$300	; bit15=H : ����(�g�p)��
		dc.w	$320
		dc.w	$340
		dc.w	$360
		dc.w	$380
		dc.w	$3A0
		dc.w	$3C0
		dc.w	$3E0
		;-------------------------------;
	global	ENGN_CTRL
ENGN_CTRL:	lea	ENGN_wk_cmn(a6),a3	;
		move.l	ENGN_wk_TONE(a3),d0	; = Engine SCSPBIN top
		beq.w	ret_21			; jump if not ready

		lea	ENGN_slot_TB(pc),a0	;
		lea	$7C0,a1			;
		movea.l	bs_HIFWK+$D8,a2		; = Engine table top address
		move.w	ENGN_P1_rpm(a1),d0	; = 0000H�`2260H
		beq.w	ENGN_all_off_s		; jump if RPM = 0000H
		move.b	ENGN_P1_rpm(a1),d0	; rpm = 00H �` 22H
		cmp.b	ENGN_wk_rpm(a3),d0	; 
		bne.w	ENGN_CTRL_3		; jump if rpm �ω�
		move.l	ENGN_P1_accl(a1),d1	;
		cmp.b	ENGN_wk_accl(a3),d1	; 
		beq.w	ret_21			; jump if rpm & accel �ω��Ȃ�
		andi.b	#$0F,d1
ENGN_CTRL_3:	cmpi.b	#$23,d0			;
		bcs.w	ENGN_CTRL_2		; jump if 00H �` 22H
		move.b	#$22,d0			
ENGN_CTRL_2:	move.b	d0,ENGN_wk_rpm(a3)	; �X�V
		move.b	d1,ENGN_wk_accl(a3)	; �X�V
		moveq	#16-1,d7		; loop size
ENGN_CTRL_lp:	bsr.w	ENGN_exe
		lea	ENGN_TB_unit(a2),a2	; + $80 : next ENGIN PCM
		dbra	d7,ENGN_CTRL_lp
ret_21:		rts
;===============================================;
;    	    <<<< ��]�� = �ω� >>>>		;
; ready a2 : $7F000,$7F080,$7F100,...		;
;	a3 : work $8860
; Hold	a0 : $300,$320,...,$3E0
;	a1 : $7C0
;	a2 : $7F000,$7F080,$7F100,...		;
;	a3 : work $8860
;===============================================;
	global	ENGN_exe
ENGN_exe:
		clr.w	d2				;
		move.l	ENGN_P1_accl(a1),d0		;
		andi.w	#$F,d0				;
		move.b	ENGN_TB_accl(a2,d0.w),d2	; d2.w = ����shift

		clr.w	d0				;
		move.b	ENGN_wk_rpm(a3),d0		; = $00 �` $22
		moveq	#0,d1				;
		move.b	ENGN_TB_rpm(a2,d0.w),d1		; d1.w = TL
		add.w	d0,d0				;
		move.w	ENGN_TB_pitch(a2,d0.w),d3	; d3.w = Pitch
		beq.w	Pitch_00			; jp if Pitch = 0 (off)
		;-------------------------------;
		;    <<<< Pitch �� 0 >>>>	;
		;-------------------------------;
		move.b	ENGN_TB_flg(a2),d0	; bit7 = PCM on
		bmi.w	ENGN_running		; jump if ������
		clr.w	d0			;
		moveq	#8-1,d6			; loop size
ENGN_slt_srch:	move.w	(a0,d0.w),d4		; = $300,$320,...,$3E0
		bpl.w	ENGN_slt_find		; jump if off ��
		addq	#2,d0
		dbra	d6,ENGN_slt_srch
		rts				; return if off slot nothing

		; <<<< ready d4 : $300,$320,...,$3E0 >>>>

	global	ENGN_slt_find
ENGN_slt_find:	bset.b	#7,(a0,d0.w)		;
		move.w	d4,ENGN_TB_d4(a2)	;
		;-------------------------------;
		;    <<<< ���F ���� >>>>	;
		;-------------------------------;
		movea.l	ENGN_TB_Layer(a2),a4	;
		addq.l	#2,a4
		move.l	(a4)+,d0
		andi.l	#$07FFFFFF,d0
		add.l	ENGN_wk_TONE(a3),d0	; + SCSPBIN top
		move.l	d0,SCSP_KXKB(a5,d4.w)		; [SA]
		move.l	(a4)+,SCSP_PCM_LSA(a5,d4.w)	; [LSA]�`
		move.l	(a4)+,SCSP_D2R1R(a5,d4.w)	; [D2R]�`
		addq.l	#6,a4
		move.b	#$FF,SCSP_TLVL(a5,d4.w)
		move.l	(a4)+,SCSP_RELFO(a5,d4.w)
		move.w	(a4)+,SCSP_DISDLPN(a5,d4.w)

		bsr.w	ENGN_PITCH_WR		; <<<< �������� >>>>
		bsr.w	ENGN_TL_WR		; <<<< ���ʏ��� >>>>
	global	ENGN_on
ENGN_on:	bset.b	#7,ENGN_TB_flg(a2)	; set ������
		move.b	(a5,d4.w),d0		;
		ori.b	#%00011000,d0		; set   KYONB ( key-on ���� )
		move.b	d0,(a5,d4.w)		; write KYONEX & KYONB
		rts
		;-------------------------------;
		; ready d1.w : TL($FF:Min�`0:Max);
		; 	d2.w : ����shift(80H,81H,82H,�`,7EH,7FH);
		; 	d3.w : Pitch(0�`3FFFH)	;
		;-------------------------------;
	global	ENGN_running
ENGN_running:	bsr.w	ENGN_PITCH_WR
;@		bra.w	ENGN_TL_WR
		;-------------------------------;
		;-------------------------------;
ENGN_TL_WR:	moveq	#0,d0
		ext.w	d2	; FF80,FF81,....,FFFF,0000,0001,....,007E,007F
		not.w	d2	; 007F,007E,....,0000,FFFF,FFFE,....,FF81,FF80
		add.w	d2,d1	;
		bmi.w	TL_max
		not.b	d0
		cmpi.w	#$100,d1
		bcc.w	TL_max
		move.b	d1,d0
TL_max:		move.b	d0,SCSP_TLVL(a5,d4.w)	; <<<< ���ʏ��� >>>>
		rts
		;-------------------------------;
		;-------------------------------;
ENGN_PITCH_WR:	move.w	ENGN_TB_d4(a2),d4	;
		andi.w	#$3FFF,d3		; 0000H�`2000H(44.1kHz)�`3FFFH
		subi.w	#$2000,d3		; -2000H�`0�`+1FFFH
		move.w	d3,d0			;
		andi.w	#$3FF,d0		; = FNS
		andi.w	#$3C00,d3		; = OCT
		add.w	d3,d3			;
		or.w	d0,d3			;
		move.w	d3,SCSP_OCTFNS(a5,d4.w)	; <<<< �������� >>>>
		rts
		;-------------------------------;
		;     <<<< Pitch = 00 >>>>	;
		;-------------------------------;
	global	Pitch_00
Pitch_00:	bclr.b	#7,ENGN_TB_flg(a2)	; bit7 = PCM on
		beq.w	Pitch_00_ret		; jump if off ��
		; <<<< key off >>>>
	global	ENGN_off
ENGN_off:	move.w	ENGN_TB_d4(a2),d4	; = $300,$320,...,$3E0
		move.b	SCSP_KXKB(a5,d4.w),d0	;
		andi.b	#%00000111,d0		; clear KYONB
		ori.b	#%00010000,d0		; set   KYONEX
		move.b	d0,SCSP_KXKB(a5,d4.w)	;
		andi.w	#$0E0,d4		; 0,$20,$40,...,$E0
		lsr.w	#8,d4			; 0,2,4,...,$E
		bclr.b	#7,(a0,d4.w)		; a0 = ENGN_slot_TB
Pitch_00_ret:	rts
;===============================================;
;    ��]�� = 0 : all Activ Engine slots off	;
;===============================================;
	global	ENGN_all_off_s

		; <<<< work clear >>>>

ENGN_all_off_s:	move.b	#$FF,ENGN_wk_rpm(a3)	; clear
		moveq	#0,d1			;
		moveq	#16-1,d7		; loop size
ENGN_all_off_3:	move.b	d1,ENGN_TB_flg(a2)	;
		move.w	d1,ENGN_TB_d4(a2)	;
		lea	ENGN_TB_unit(a2),a2	; + $80 : next ENGIN PCM
		dbra	d7,ENGN_all_off_3	;

		; <<<< Engine slots all key-off >>>>

		moveq	#8-1,d7			; loop size
ENGN_all_off_2:	bclr.b	#7,(a0)			; ������ ?
		beq.w	ENGN_all_off_1		; jump if no
		move.w	(a0),d4			; = $300,$320,...
		move.b	SCSP_KXKB(a5,d4.w),d0	;
		andi.b	#%00000111,d0		; clear KYONB
		ori.b	#%00010000,d0		; set   KYONEX
		move.b	d0,SCSP_KXKB(a5,d4.w)	;
ENGN_all_off_1:	addq.l	#2,a0
		dbra	d7,ENGN_all_off_2

		; <<<< slot-work Engine flag clear >>>>

;		lea	slot_work(a6),a3	;
;		lea	slot_wk_unit*24(a3),a3	;
;		moveq	#8-1,d7			;
;ENGN_all_off_4:	bclr.b	#ENGN_flg,sl_flag1(a3)	;
;		lea	slot_wk_unit(a3),a3	;
;		dbra	d7,ENGN_all_off_4

		rts
;===============================================;
;	   �G���W��������( MAP change )		;
;===============================================;
	global	ENGN_init
ENGN_init:
		; <<<< ENGN_wk_cmn initial >>>>

		moveq	#0,d0			; ready d0.b : BANK#
		jsr	get_BANK_addr(pc)	; return d2.l : SCSPBIN top
		bcs.w	ret_22			; jump if ���FBANK-ID nothing
		lea	ENGN_wk_cmn(a6),a3
		move.b	#$FF,ENGN_wk_rpm(a3)
		move.l	d2,ENGN_wk_TONE(a3)	; = Engine SCSPBIN top

		; <<<< Engine table work initial >>>>

		movea.l	d2,a0			; = Engine SCSPBIN top
		adda.w	8(a0),a0		; = Voice#0 top
		addq.l	#4,a0			; = Voice#0 Layer#1 top
		move.l	a0,d1			;

		movea.l	$7C0+ENGN_TB_addr,a2	; = Engine Table top
		moveq	#0,d0
		moveq	#16-1,d7
ENGN_init_lp_2:	move.l	d1,ENGN_TB_Layer(a2)	; = Voice#0 Layer#x top addr
		move.w	d0,ENGN_TB_d4(a2)
		move.b	d0,ENGN_TB_flg(a2)
		lea	ENGN_TB_unit(a2),a2	; + $80 : next ENGIN PCM
		addi.l	#32,d1			; next Layer top addr
		dbra	d7,ENGN_init_lp_2
		;-------------------------------;
		lea	slot_work(a6),a3	;
		lea	slot_wk_unit*24(a3),a3	;
		lea	ENGN_slot_TB(pc),a0	;
		move.w	#$300,d4		;
		moveq	#8-1,d7			;
ENGN_init_lp_1:	bset.b	#ENGN_flg,sl_flag1(a3)	;
		lea	slot_wk_unit(a3),a3	;
		move.w	d4,(a0)+		;
		addi.w	#SCSP_slot_unit,d4	; = $300,$320,...,$3E0
		dbra	d7,ENGN_init_lp_1	;

ret_22:		rts

		global	ENGN_init_x
ENGN_init_x:	moveq	#0,d0			;
		lea	ENGN_wk_cmn(a6),a3	;
		move.l	d0,ENGN_wk_TONE(a3)	; = Engine SCSPBIN top
		rts
;===============================================;
	global	CTRL_12,CTRL_13
CTRL_12:	move.l	ENGN_wk_cmn+ENGN_wk_TONE(a6),d0	;
		beq.w	ret_23			; jump if not ready

		andi.w	#$7F,d3
		add.w	d3,d3
		move.w	RPM_CHG_TB(pc,d3.w),d0
		lea	$7C0,a1			;
		move.w	d0,ENGN_P1_rpm(a1)	; = 0000H�`2260H
ret_23:		rts
		;-------------------------------;
CTRL_13:	move.l	ENGN_wk_cmn+ENGN_wk_TONE(a6),d0	;
		beq.w	ret_24			; jump if not ready

		andi.l	#$78,d3
		lsr.l	#3,d3			; d3 = 0,1,2,...,F
		lea	$7C0,a1			;
		move.l	d3,ENGN_P1_accl(a1)	; = 0000H�`000FH
ret_24:		rts
RPM_CHG_TB:
	DC.W	$45*$00,$45*$01,$45*$02,$45*$03,$45*$04,$45*$05,$45*$06,$45*$07
	DC.W	$45*$08,$45*$09,$45*$0A,$45*$0B,$45*$0C,$45*$0D,$45*$0E,$45*$0F
	DC.W	$45*$10,$45*$11,$45*$12,$45*$13,$45*$14,$45*$15,$45*$16,$45*$17
	DC.W	$45*$18,$45*$19,$45*$1A,$45*$1B,$45*$1C,$45*$1D,$45*$1E,$45*$1F
	DC.W	$45*$20,$45*$21,$45*$22,$45*$23,$45*$24,$45*$25,$45*$26,$45*$27
	DC.W	$45*$28,$45*$29,$45*$2A,$45*$2B,$45*$2C,$45*$2D,$45*$2E,$45*$2F
	DC.W	$45*$30,$45*$31,$45*$32,$45*$33,$45*$34,$45*$35,$45*$36,$45*$37
	DC.W	$45*$38,$45*$39,$45*$3A,$45*$3B,$45*$3C,$45*$3D,$45*$3E,$45*$3F
	DC.W	$45*$40,$45*$41,$45*$42,$45*$43,$45*$45,$45*$45,$45*$46,$45*$47
	DC.W	$45*$48,$45*$49,$45*$4A,$45*$4B,$45*$4C,$45*$4D,$45*$4E,$45*$4F
	DC.W	$45*$50,$45*$51,$45*$52,$45*$53,$45*$54,$45*$55,$45*$56,$45*$57
	DC.W	$45*$58,$45*$59,$45*$5A,$45*$5B,$45*$5C,$45*$5D,$45*$5E,$45*$5F
	DC.W	$45*$60,$45*$61,$45*$62,$45*$63,$45*$64,$45*$65,$45*$66,$45*$67
	DC.W	$45*$68,$45*$69,$45*$6A,$45*$6B,$45*$6C,$45*$6D,$45*$6E,$45*$6F
	DC.W	$45*$70,$45*$71,$45*$72,$45*$73,$45*$74,$45*$75,$45*$76,$45*$77
	DC.W	$45*$78,$45*$79,$45*$7A,$45*$7B,$45*$7C,$45*$7D,$45*$7E,$45*$7F

	endif
;--------------------------------------------------------------------------

	end

