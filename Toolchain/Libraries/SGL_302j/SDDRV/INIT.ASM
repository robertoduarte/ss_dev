
	include	SCSP.LIB

	external	get_BANK_addr

;===============================================;
;	      �uector table �]��		;
; �r�b�r�h�x�N�^(Level1:$64)�͏����֎~		;
;===============================================;
	if	sw_MODEL_M
		global	vector_tb_init
vector_tb_init:
		lea	8,a1
		lea	vecter_tb(pc),a0	;
		move.l	$000064,d1		; d1: SCSI vector
		moveq	#30-1,d7		; loop size
vtbinit_0:	move.l	(a0)+,(a1)+		; set vector#02�`#31
		dbra	d7,vtbinit_0

		moveq	#16-1,d7		; ready loop size
		jsr	vtbinit_sb(pc)		; set vector#32�`#47
		moveq	#16-1,d7		; loop size
		jsr	vtbinit_sb(pc)		; set vector#48�`#63
		move.w	#192-1,d7		; loop size
		jsr	vtbinit_sb(pc)		; set vector#64�`#255
		bra	vi00
		;-------------------------------;
vtbinit_sb:	move.l	(a0)+,d0
vtbinit_3:	move.l	d0,(a1)+		; 
		dbra	d7,vtbinit_3
		rts
;-----------------------------------------------; 1994.8.15 rep K/F
vi00:		lea	$000064,a0		; SCSI vector address
		lea	IO_SRAM1,a1		; SSbug Memory = E00000~E07FFF
		cmp.l	$64(a1),d1		; SSbug Vecter $64 equal ?
		bne	vi10			; jump if not equal
		move.l	#"SSBg",d0		; = "SSBg" ASCII
		cmp.l	$80(a1),d0		; (e00080)="SSBg"?
		bne	vi10			; jump if not SSbug exe
		move.l	d1,(a0)			; copy SSBug vector
		bra	vi20			;
vi10:		move.l	$600064,(a0)		; copy ROM vector
vi20:
;-----------------------------------------------; 1994.8.15 rep K/F
		rts

	external	level_2,level_5,level_6
	external	er_1F,er_01,er_0B,er_0C,er_0D
	external	er_0E,er_0F,er_10,er_11,er_12,er_13
	external	er_14,er_15,er_16,er_17,er_18,er_19

vecter_tb:	dc.l	er_1F		; CPU vector#02 buss error
		dc.l	er_01		; CPU vector#03 address error
		dc.l	er_0E		; CPU vector#04 �s������
		dc.l	er_0F		; CPU vector#05 �O���Z
		dc.l	er_10		; CPU vector#06 CHK����
		dc.l	er_11		; CPU vector#07 TRAPV����
		dc.l	er_12		; CPU vector#08 �����ᔽ
		dc.l	er_13		; CPU vector#09 TRACE��O����
		dc.l	er_14		; CPU vector#10 ����������
		dc.l	er_14		; 	//  #11    //
		dc.l	er_15		; CPU vector#12 reserved
		dc.l	er_15		;	//  #13	   //
		dc.l	er_15		;	//  #14	   //
		dc.l	er_16		; CPU vector#15 uninitialized
		dc.l	er_15		; CPU vector#16 reserved
		dc.l	er_15		;	//  #17	   //
		dc.l	er_15		;	//  #18	   //
		dc.l	er_15		;	//  #19	   //
		dc.l	er_15		;	//  #20	   //
		dc.l	er_15		;	//  #21	   //
		dc.l	er_15		;	//  #22	   //
		dc.l	er_15		;	//  #23	   //
		dc.l	er_17		; CPU vector#24 �X�v���A�X
VCTR_SCSI:	dc.l	0		; CPU vector#25 Level#1 SCSI
		dc.l	level_2		; 	    #26 Level#2 Timer-B
		dc.l	er_0B		; 	    #27 Level#3 no asigned
		dc.l	er_0C		; 	    #28 Level#4 no asigned
		dc.l	level_5		; 	    #29 Level#5 MIDI #1
		dc.l	level_6		; 	    #30 Level#6 MIDI #2
		dc.l	er_0D		;	    #31 Level#7 debugger

		dc.l	er_18		; CPU vector#32�`47  TRAP����
		dc.l	er_15		; CPU vector#48�`63  reserved
		dc.l	er_19		; CPU vector#64�`255 user vector

	endif
;===============================================;
;		�x�l�R�W�O�Q ������		;
;===============================================;
	if	sw_MODEL_M
		global	YM3802_init
YM3802_init:
		lea	IO_MIDI1+1,a0		; ready a0
		jsr	MCS_init(pc)		;

		; <<< set input direction I/O port : MCS-1 only >>>

		move.b	#9,R01(a0)
		move.b	#init_R94,Rx4(a0)	; R94 <-- all I/O input @001

		lea	IO_MIDI2+1,a0		; ready a0
;@		jmp	MCS_init(pc)		; jump and return

MCS_init:	; <<< reset >>>

		move.b	#$80,R01(a0)		; bit7 = H ( MCS initial clear )
		rol.w	#8,d0		; 2 #22	; wait @004
		rol.w	#8,d0		; 2 #22	;
		move.b	#0,R01(a0)		; reset ����

		; <<< set IRQ vector offset 0 >>>

		move.b	#0,R01(a0)
		move.b	#init_R04,Rx4(a0)	; R04 <-- IRQ vector offset

		; <<< initial ����M/FIFO >>>

		move.b	#3,R01(a0)
RxBusy0:	move.b	Rx4(a0),d0		; R34 read
		lsr.b	#1,d0			; receive busy ?
		bcs.s	RxBusy0			; jump if busy
		move.b	#2,R01(a0)
		move.b	#init_R24,Rx4(a0)	; R24 <-- ��Mrate�ݒ�
		move.b	#init_R25,Rx5(a0)	; R25 <-- ��M���[�h�ݒ�
		move.b	#5,R01(a0)
TxBusy0:	move.b	Rx4(a0),d0		; R54 read
		lsr.b	#1,d0			; transmitter busy ?
		bcs.s	TxBusy0
		move.b	#4,R01(a0)
		move.b	#init_R44,Rx4(a0)	; R44 <-- ���Mrate�ݒ�
		move.b	#init_R45,Rx5(a0)	; R45 <-- ���M���[�h�ݒ�
		move.b	#3,R01(a0)
		move.b	#init_R35,Rx5(a0)	; R35 <-- FIFO-Rx �ݒ�
		move.b	#5,R01(a0)
		move.b	#init_R55,Rx5(a0)	; R55 <-- FIFO-Tx �ݒ�
		move.b	#0,R01(a0)
		move.b	#init_R06,Rx6(a0)	; IRQ enable select
;@		move.b	#6,R01		; click counter enable & CLKM=1MHz
;@		move.b	#11B,Rx6	; R66 <-- ____ __xxB
		rts
	endif
;************************************************************************
; 			Tool I/F Monitor area clear 			*
;�y�@�\�z�������� MIDI Prg#�� �S�Ă� MIDI channel �ɂ����� "0" �Ƃ���	*
;�y���́z a6   : work ram top addr				94/07/26*
;************************************************************************
	if	sw_MODEL_M
		global	MONITOR_init
MONITOR_init:
		lea	bs_MONTR,a0		;
		moveq	#slot_size-1,d7		; loop size
		moveq	#0,d1			;
MNTR_init_lp:	move.l	d1,(a0)+		;
		dbra	d7,MNTR_init_lp		;
		rts
	endif
;************************************************************************
;		SCSPBIN address stack buffer initialize			*
;�y�@�\�z�g�n�r�s �h�^�e �̈�� "�O" �N���A				*
;�y���́z a6   : work ram top addr				94/07/26*
;************************************************************************
	if	sw_MODEL_M
		global	HOST_IF_init
HOST_IF_init:	lea	bs_HIFWK,a0	; HOST I/F work top addr
		moveq	#0,d0
		moveq	#64/4-1,d7		; loop size
HOST_IF_lp:	move.l	d0,(a0)+
		dbra	d7,HOST_IF_lp
		rts
	endif

;@
;@	94/11/26 �폜
;@	94/12/01 �폜��������
;@
;************************************************************************
;�y�@�\�z[k_BANK_adr] initial						*
;	 [k_PRG_no],[k_MIDI_PAN],[k_PBend_BF]�� 0 clear			*
;�y���́z�Ȃ�								*
;************************************************************************

; A.Miyazawa	{

;		global	BANK_init
;BANK_init:	moveq	#8-1,d4			; loop size
;		move.w	#0,_tmp_kanri(a6)	; clear
;		move.w	#0,knr_kanri_ofst(a6)	;
;BANK_init_lp2:	moveq	#16-1,d6		; loop size
;		move.w	#0,_tmp_MIDI_ch(a6)	; word	; $0000 �` $001F
;BANK_init_lp1:	movem.l	d1/d3/d4/d6,-(SP)	; reg. push
;		moveq	#0,d0			; ready d0 : tone-BANK#
;		move.w	_tmp_MIDI_ch(a6),d1	; ready d1 : MIDI ch#
;		jsr	get_BANK_addr(pc)	; return d2.l
;
;		move.w	d1,d0			; = MIDI ch#
;		lsl.w	#5,d0			; = knr_unit*MIDIch#
;		add.w	_tmp_kanri(a6),d0	; + �����Ǘ��ԍ��~200H
;		move.l	d2,knr_BANK_adr(a6,d0.w)	;
;		move.b	#init_MIDI_VOL,knr_MIDI_Volume(a6,d0.w)
;		move.b	#init_MIDI_VOL,knr_MIDI_7(a6,d0.w)
;		move.b	#init_MIDI_VOL,knr_MIDI_11(a6,d0.w)
;
;		moveq	#0,d2			; ready d2 : MIDI Prg#
;		move.w	_tmp_MIDI_ch(a6),d1	; ready d1 : MIDI ch#
;;@		jsr	PRG_chg(pc)		;
;		movem.l	(SP)+,d1/d3/d4/d6	; reg. pop
;		addi.w	#$10,knr_kanri_ofst(a6)	;
;		addq.w	#1,_tmp_MIDI_ch(a6)	;
;		dbra	d6,BANK_init_lp1
;		addi.w	#$200,_tmp_kanri(a6)	;
;		dbra	d4,BANK_init_lp2
;		move.w	#0,_tmp_kanri(a6)	; clear
;		rts

			.public		BANK_init

BANK_init:
		movem.l	d0-d7/a0/a1,-(sp)
		movea.l	#management_work,a0

		moveq	#7-1,d7				; loop size

		moveq	#0,d5
		move.w	d5,_tmp_kanri(a6)
		move.w	d5,knr_kanri_ofst(a6)


	?management_number_loop:
		moveq	#0,d1
		move.w	d1,up_down_flag(a0)
		move.w	d1,seq_volume_arrival(a0)
		move.w	d1,appointed_fade_rate(a0)
		move.w	d1,fade_count_variable(a0)
		move.w	#$0080,sequence_volume(a0)
		move.w	d1,performance_flag(a0)

		moveq	#16-1,d6			; loop size
		move.w	d5,d4

	?midi_channel_loop:
		moveq	#default_volume,d1
		move.w	d1,midi_master_volume(a6,d4.w)
		move.w	d1,midi_volume(a6,d4.w)
		move.w	d1,midi_expression(a6,d4.w)
		move.w	d1,noteon_master_volume(a6,d4.w)
		move.w	d1,total_volume(a6,d4.w)

		move.w	a0,d1
		move.l	d1,management(a6,d4.w)

		addi.w	#$0020,d4
		dbra	d6,?midi_channel_loop

		adda.w	#SQ_SIZ,a0
		addi.w	#$0200,d5
		dbra	d7,?management_number_loop

only_management_number7:
		lea	bs_AMAPC,a1			; area map current work top
		moveq	#32-1,d0

	?area_map_loop:
		move.b	(a1),d1				; data exist ?

		andi.b	#$70,d1				; = data ID only
		beq.s	?300				; jump if not ���FBANK-ID(=0)

		addq.w	#8,a1
		dbra	d0,?area_map_loop

		bset.b	#ERRb_2,Mem_err_bit+3
		bra.s	?end_of_bank_init		; error!, without timber bank.
	?300:
		move.l	(a1)+,d2
		andi.l	#$000fffff,d2

		moveq	#0,d1
		move.w	d1,up_down_flag(a0)
		move.w	d1,seq_volume_arrival(a0)
		move.w	d1,appointed_fade_rate(a0)
		move.w	d1,fade_count_variable(a0)
		move.w	#$0080,sequence_volume(a0)
		move.w	d1,performance_flag(a0)

		moveq	#16-1,d6			; loop size
		move.w	d5,d4

	?midi_channel_loop:
		move.l	d2,knr_BANK_adr(a6,d4.w)
		sf.b	knr_PROG_no(a6,d4.w)
		clr.w	knr_PBend_BF(a6,d4.w)

		moveq	#default_volume,d1
		move.w	d1,midi_master_volume(a6,d4.w)
		move.w	d1,midi_volume(a6,d4.w)
		move.w	d1,midi_expression(a6,d4.w)
		move.w	d1,noteon_master_volume(a6,d4.w)
		move.w	d1,total_volume(a6,d4.w)

		move.w	a0,d1
		move.l	d1,management(a6,d4.w)

		addi.w	#$0020,d4
		dbra	d6,?midi_channel_loop

	?end_of_bank_init:

		movem.l	(sp)+,d0-d7/a0/a1
		rts

;		}

; ������������������������������������������������������������������������
; �� 			�r�b�r�o �� �� �� �� ��				��
; ���y���́z a5   : SCSP[FH1005] I/O addr				��
; ���y�j��z all							��
; ������������������������������������������������������������������������
		global	FH1005_init
FH1005_init:
		move.b	#2,RG_M4D8(a5)		; SCSP 4Mbit D-RAM mode

		move.b	RG_MIBUF(a5),d0		; dammy read
		move.b	RG_MIBUF(a5),d0		;
		move.b	RG_MIBUF(a5),d0		;
		move.b	RG_MIBUF(a5),d0		;

		jsr	MPRO_all_clr(pc)	; DSP clear 95/02/20
		jsr	MADR_all_clr(pc)	;
		jsr	COEF_all_clr(pc)	;
		jsr	TEMP_all_clr(pc)	;
		jsr	SLOT_all_clr(pc)
		jsr	all_TL_off(pc)		;
		jsr	all_key_off(pc)		;
;		<<<< Master volume -0 dB >>>>
		move.b	#$0F,RG_MVOL(a5)	; Master Volume set 0 dB 
;		<<<< Timer �ݒ� >>>>

		lea	SCSP_init_TB(pc),a0	;
		move.w	(a0)+,d0		;
		adda.w	d0,a5			;
		move.w	(a0)+,d7		; data size ( loop size )
FH1005_init_3:	move.b	(a0)+,(a5)+		;
		dbra	d7,FH1005_init_3	;
		lea	IO_SCSP,a5		;

		move.w	#TIMA_base,RG_TACTL(a5)	; 2msec [TACTL] & [TIMA]
		move.w	#$40,RG_SCIRE(a5)	; timer-A reset
		rts

SCSP_init_TB:
		dc.w	RG_TBCTL		; transmit start Reg
		dc.w	SCSP_init_TB_e-SCSP_init_TB_s-1
SCSP_init_TB_s:	dc.w	TIMB_VL		;[TBCTL,TIMB] Timer-B value
		dc.w	TIMC_VL		;[TCCTL,TIMC] Timer-C value
		global	iiii
iiii:		dc.w	INT_EN		;[SCIEB]      Interrupt EN
		dc.w	INT_RD		;[SCIPD]      status read ( dammy )
		dc.w	INT_RST		;[SCIRE]      Interrupt reset
		dc.w	SCSP_IPL0	;[SCILV0]
		dc.w	SCSP_IPL1	;[SCILV1]
		dc.w	SCSP_IPL2	;[SCILV2]
SCSP_init_TB_e:
		;-------------------------------;
		global	all_TL_off
all_TL_off:	lea	SCSP_TLVL(a5),a0
		move.b	#$FF,d0
		bra.s	FH1005_init_x
		;-------------------------------;
		global	all_key_off
all_key_off:	movea.l	a5,a0			;
		moveq	#%00010000,d0		;
FH1005_init_x:	moveq	#slot_size-1,d7		;
FH1005_init_2:	move.b	d0,(a0)			;
		lea	SCSP_slot_unit(a0),a0	;
		dbra	d7,FH1005_init_2	;
		rts
		;-------------------------------;
		global	SLOT_all_clr
SLOT_all_clr:	movea.l	a5,a0			;
		moveq	#0,d0			;
		move.w	#$400/4-1,d7		; loop size
FH1005_init_1:	move.l	d0,(a0)+		;
		dbra	d7,FH1005_init_1	;
		rts
		;-------------------------------;
		global	clr_sng_mdst
clr_sng_mdst:	; <<< clear song mode/status >>>
		moveq	#0,d0			;++++++++ ver1.29 ++++++++
		lea	bs_HIFWK+80H,a0	; HOST I/F work top addr
		move.l	d0,(a0)+		;
		move.l	d0,(a0)+		;
		move.l	d0,(a0)+		;
		move.l	d0,(a0)+		;
		rts
		;-------------------------------;
		global	all_RR_off
all_RR_off:
		lea	IO_SCSP,a5		; �Œ�
		lea	SCSP_FH_RR(a5),a1	;
		moveq	#32-1,d7		; loop size
all_RR_off_lp:	ori.b	#$1f,(a1)
		lea	SCSP_slot_unit(a1),a1
		dbra	d7,all_RR_off_lp
		rts
		;-------------------------------;
		global	IMXL_all_off
IMXL_all_off:	lea	SCSP_ISEL+1(a5),a1
		moveq	#16-1,d7		; loop size
		moveq	#0,d0			; DSP ���� Level off data
IMXL_alof_lp:	move.b	d0,(a1)			;
		lea	SCSP_slot_unit(a1),a1	;
		dbra	d7,IMXL_alof_lp
		rts
		;-------------------------------;
		global	EFSDL_all_off
EFSDL_all_off:	lea	SCSP_EFSDLPN(a5),a1
EFSDL_off_lp:	moveq	#16-1,d7		; loop size
		moveq	#0,d0			; Effect return off data
EFSDL_off_lpx:	move.b	d0,(a1)			;
		lea	SCSP_slot_unit(a1),a1	;
		dbra	d7,EFSDL_off_lpx
		move.l	d0,mixer_wk_SCSP+0(a6)
		move.l	d0,mixer_wk_SCSP+4(a6)
		move.l	d0,mixer_wk_SCSP+8(a6)
		move.l	d0,mixer_wk_SCSP+12(a6)
		rts
		;-------------------------------;
		global	MPRO_all_clr
MPRO_all_clr:	lea	DSP_MPRO(a5),a1		;
		move.w	#$400/4-1,d7		; loop size
		bra.s	all_clr_lp
		global	MADR_all_clr
MADR_all_clr:	lea	DSP_MADRS(a5),a1	;
		moveq	#$80/4-1,d7		; loop size
		bra.s	all_clr_lp
		global	COEF_all_clr
COEF_all_clr:	lea	DSP_COEF(a5),a1		;
		moveq	#$80/4-1,d7		; loop size
		bra.s	all_clr_lp
		global	TEMP_all_clr
TEMP_all_clr:	lea	DSP_TEMP(a5),a1		;
		moveq	#$200/4-1,d7		; loop size
		;-------------------------------;
all_clr_lp:	moveq	#0,d0			; clear data
all_clr_lpx:	move.l	d0,(a1)+		;
		dbra	d7,all_clr_lpx
		rts
;************************************************************************
;			slot_work [sl_layer_adr]			*
;�y���́z a6 : work ram top addr				94/07/26*
;************************************************************************
	global	sl_layer_adr_init
sl_layer_adr_init:
		moveq	#0,d5			;
		movea.l	knr_BANK_adr(a6,d5.w),a1	; = SCSPBIN top address
		moveq	#0,d0			;
		move.w	BIN_VOICE(a1),d0	; = Voice top offset
		adda.l	d0,a1			; + Voice top offset
		addq.l	#4,a1			; + Layer#1 offset


		moveq	#slot_wk_unit,d1	;
		lea	slot_work+sl_layer_adr(a6),a4
		moveq	#slot_size-1,d7		; loop size
wk_RAM_clr_3:	move.l	a1,(a4)			;
		adda.l	d1,a4			;
		dbra	d7,wk_RAM_clr_3		;
		rts
;************************************************************************
;			work RAM clear					*
;�y���́z a6   : work ram top addr				94/07/26*
;************************************************************************
		global	wk_RAM_init

wk_RAM_init:	move.l	#sz_Prg_wk/4-16,d7	; = 68k Program work size
		moveq	#0,d0			;	- stack area
		movea.l	a6,a0			;
wk_RAM_clr_lp:	move.l	d0,(a0)+		;
		dbra	d7,wk_RAM_clr_lp	;

		; <<<<< key HIStory initial >>>>>>

		moveq	#0,d2			;
		lea	_KEYHISTB(a6),a1	;
		moveq	#4-1,d7			; loop size
wk_RAM_2:	moveq	#8-1,d6			;
wk_RAM_1:	andi.w	#$3E,d2			;
		move.w	d0,(a1,d2.w)		;
		addi.w	#$40,d0			;
		addq.w	#8,d2			;
		dbra	d6,wk_RAM_1		;
		subi.w	#$3E,d2			;
		dbra	d7,wk_RAM_2		;

		move.w	#slot_size,off_slot_cnt(a6)

		lea	Pcm_Strm(a6),a3		; PCM stream# stack area
		moveq	#0,d0			;
		moveq	#16-1,d7		;
Pcm_Strm_lp:	move.b	d0,(a3)+		;
		addq.b	#1,d0			;
		dbra	d7,Pcm_Strm_lp		;

		lea	_MIDI_OUT_BF(a6),a2	;
		move.w	#$400/4-1,d7		; loop size
		moveq	#0,d0			;
MIDI_OUT_BF_lp:	move.l	d0,(a2)+		;
		dbra	d7,MIDI_OUT_BF_lp	;

	if	ENGN
		external	ENGN_init_x
		bra.w	ENGN_init_x
	endif
		rts


