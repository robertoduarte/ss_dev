;		���䍀��
;	drv_87	: HOST Comand $87 Mixer change
;	CTRL_10 : $Bn,$10,$xx     Mixer change
;	CTRL_20	: $Bn,$20,$xx	  Bank  change
;

	include	SCSP.LIB

	external	BANK_init
	external	EFSDL_all_off		; MIXER[EFREG0�`F] clear

;************************************************************************
;		  �g�n�r�s�R�}���h�i���W�V�j�lixer# change		*
; P1 : 0 �` F ( ���FBANK# )					94/07/27*
; P2 : 0 �` 7F ( Mixer# )						*
;************************************************************************
	global	drv_87
drv_87:		move.b	(a0)+,d0		; P1 : ���F BANK#
		move.b	(a0)+,d3		; P2 : Mixer#
		andi.w	#$7F,d3			;
		move.w	d3,_activ_MIX_NO(a6)	; ready Mixer#

		jsr	get_BANK_addr(pc)	; ready d0
		bcs	return_2		; jump if ���FBANK-ID nothing
		move.l	d2,_activ_MIX_BNK(a6)	;
;@		jsr	set_MIXER_wk2(pc)	;
;@		bcs.s	drv_87_er		; jump if Mixer# data nothing
		jmp	MIXER_wr(pc)		; jump and return
;@drv_87_er:	rts
;************************************************************************
;		SCSPBIN address set into [k_BANK_adr]			*
;�y�@�\�z�w�艹�F�a�`�m�j"SCSPBIN"�̐擪�A�h���X�� �`rea �lap �J�����g	*
;	 ���猟������[k_BANK_adr]�Ɋi�[�B				*
;	 �܂�[k_PRG_no],[k_MIDI_PAN],[k_PBend_BF]�� 0 clear		*
;�y���́zd0.w : ���F�a�`�m�j�ԍ� ( $00�`$0F )				*
;�y�j��za0/a1/d2/d3/d7						94/07/26*
;�y�o�́zd2.l : ���F�a�`�m�j top address				*
;************************************************************************
		external	er_1B
		global	get_BANK_addr
get_BANK_addr:
		andi.w	#$0f,d0			; = BANK#
		lea	bs_AMAPC,a0		; area map current work top
		moveq	#32-1,d7		; loop size
BNK_chg:	move.b	(a0),d2			; data exist ?
		bmi.s	er_IF_bit2		; jump if ���FBANK-ID nothing
		move.b	d2,d3			; = data ID & ID#
		andi.b	#$70,d2			; = data ID only
		bne.s	BNK_chg_4		; jump if not ���FBANK-ID(=0)
		andi.b	#$0F,d3			; = ID# only
		cmp.b	d0,d3			; BANK# equal ?
		beq.s	BNK_chg_3		; jump if Yes
BNK_chg_4:	lea	8(a0),a0		;
		dbra	d7,BNK_chg		;
		bra.s	er_IF_bit2		; jump if ���FBANK-ID nothing

BNK_chg_3:	move.w	(a0)+,d2		;
		andi.w	#$0F,d2			; = addr D19�`D16
		swap	d2			;
		move.w	(a0)+,d2		; = addr D15�`D0
		move.b	(a0),d3			; �f�[�^ down load �� ?
		bpl.s	er_IF_bit2		; jump if ���FBANK not ready
;@		move.l	d2,_activ_MIX_BNK(a6)	;

		bclr.b	#2,Mem_err_bit+3
		clc
		rts
er_IF_bit2:	bset.b	#2,Mem_err_bit+3
		stc
return_2:	rts
;************************************************************************
;�y�@�\�z �l�h�c�h�R���g���[���`�F���W[$Bn,$10,$xx]����B		*
;	  �w��l�h�w�d�q�f�[�^[EFSDL]&[EFPAN]�����݁B			*
;�y���́zd1.w : MIDI ch# ( 0 �` 1F )					*
;        d3.b : Parameter 00�`7FH = MIXER Number		94/07/26*
;************************************************************************
		global	CTRL_10
CTRL_10:	; �s����i�ėp����q�P�j
		move.w	_tmp_kanri(a6),d2	; = �����Ǘ��ԍ��~200H
		lsl.w	#4,d1			; activ Mixer MIDI ch# 0�`1F*10H
		add.w	d1,d2			;
		move.l	k_BANK_adr(a6,d2.w),d0	; = SCSPBIN top address
		beq	CTRL_10_er0		;
		bclr.b	#3,Mem_err_bit+3	;
		move.l	d0,_activ_MIX_BNK(a6)	;

		andi.w	#$7F,d3			;
		move.w	d3,_activ_MIX_NO(a6)	; ready Mixer#
;@		bra	MIXER_wr
;************************************************************************
;			�l�h�w�d�q �X�V					*
;�y�@�\�z�l�h�w�d�q�f�[�^��"mixer_wk"�ɓ]����A�r�b�r�o�ɏ����B		*
;�y���́z[_activ_MIX_BNK] MIX�f�[�^�̂��鉹�FBANK�擪�A�h���X		*
;	 [_activ_MIX_NO]  MIXER#					*
;************************************************************************
		global	MIXER_wr
MIXER_wr:
		move.l	_activ_MIX_BNK(a6),d0	; = SCSPBIN top address
		beq.s	er_IF_bit0
		movea.l	d0,a0
		move.w	_activ_MIX_NO(a6),d0	; ready Mixer#
		movea.l	a0,a1			;
		adda.w	BIN_VL(a0),a1		; = V-L data top
		adda.w	BIN_MIX(a0),a0		; = MIXER top addr

		add.w	d0,d0			;
		adda.w	d0,a0			; +  2*Mixer#
		lsl.w	#3,d0			; + 16*Mixer#
		adda.w	d0,a0			; = desti. Mixer addr

		cmpa.l	a1,a0
		bcc.s	er_IF_bit0		; jump if not find Mixer
		lea	mixer_wk_SCSP(a6),a2	;
		move.l	(a0)+,(a2)+		;
		move.l	(a0)+,(a2)+		;
		move.l	(a0)+,(a2)+		;
		move.l	(a0)+,(a2)+		;
;@		move.w	(a0)+,(a2)+		;
		bclr.b	#0,Mem_err_bit+3
		;************************************************
		;    �l�h�w�d�q �iEffect Return & Pan ) �X�V	*
		;�y�@�\�z mixer_wk �f�[�^�� "�d�e�q�d�f"�ɏ����B*
		;************************************************
		global	MIXER_wk_wr
MIXER_wk_wr:
		move.b	EFCT_CHG_CNT(a6),d0	; Effect change exe mode ?
		bne.s	MIXER_ret0		; jump if Yes

		lea	mixer_wk_SCSP(a6),a0
;@		moveq	#18-1,d7		; loop size
		moveq	#16-1,d7		; loop size 95/02/21
		move.b	SND_OUT_ST(a6),d4	; MONO/STEREO status
		bmi.s	MIXER_MONO
		; <<<< Stereo mode >>>>
		clr.w	d4
MIXER_wr1_lp:	move.b	(a0)+,SCSP_EFSDLPN(a5,d4.w)
		addi.w	#SCSP_slot_unit,d4
		dbra	d7,MIXER_wr1_lp
MIXER_ret0:	rts
		; <<<< MONO mode >>>>
MIXER_MONO:	clr.w	d4
MIXER_wk_wr_lp:	move.b	(a0)+,d0
		andi.b	#$E0,d0			; --> Center
		move.b	d0,SCSP_EFSDLPN(a5,d4.w)
		addi.w	#SCSP_slot_unit,d4
		dbra	d7,MIXER_wk_wr_lp
		clc
		rts
er_IF_bit0:	bset.b	#0,Mem_err_bit+3
		jmp	EFSDL_all_off(pc)	; MIXER[EFREG0�`F] clear
;@		rts
CTRL_10_er0:	bset.b	#3,Mem_err_bit+3
		jmp	EFSDL_all_off(pc)	; MIXER[EFREG0�`F] clear
;@		stc
;@		rts
;************************************************************************
; Command I/F = $08	  Map change					;
;************************************************************************
		global	seq_map
seq_map:	move.b	(a0)+,d0		; P1 ready d0 : ( 0 �` 7 )
		andi.w	#$FF,d0			;
		jsr	MAP_chg(pc)		; ready d0
		jmp	BANK_init(pc)
;@@@@@		clc
;@@@@@		rts
;************************************************************************
;			    �`�q�d�` �l�`�o �X�V			*
;�y�@�\�z�w�肳�ꂽ�l�`�o���X�y�A�̈悩��J�����g�̈�ɓ]���B		*
;�y���́zd0.w : MAP#	( $00�`$FF )					*
;�y�o�́zCY = error / NC = complete				94/07/26*
;************************************************************************
		global	MAP_chg
MAP_chg:
		lea	bs_AMAPC,a0		; area map current work top
		moveq	#$100/4-1,d7		;
		moveq	#0,d4			;
MAP_clear:	move.l	d4,(a0)+		;
		dbra	d7,MAP_clear		;
		;-------------------------------;
		lea	k_BANK_adr(a6),a0	;
		move.w	#8*32-1,d7
		moveq	#0,d4
k_BANK_adr_clr:	move.l	d4,(a0)			; = clear SCSPBIN top address
		lea	$10(a0),a0		;
		dbra	d7,k_BANK_adr_clr
		;-------------------------------;
		lea	slot_work(a6),a0	;
		move.w	#slot_size-1,d7
		moveq	#0,d4
sl_flg_clr:	move.b	d4,sl_flag1(a0)		; clear slot_work[sl_flag1]
		move.b	d4,PSPN(a0)		; clear slot_work[PSPN]
		move.l	d4,sl_layer_adr(a0)	; clear slot_work[sl_layer_adr]
		lea	slot_wk_unit(a0),a0
		dbra	d7,sl_flg_clr
		;-------------------------------;
		lea	bs_AREAM,a0		; Area Map �X�y�A�̈� top
		lea	bs_AREAM+sz_AREAM,a1
		;-------------------------------;
		andi.w	#$FF,d0
		beq	MAP_chg_ex		; jump if Map#0
		subq.w	#1,d0			; = loop size
MAP_chg_lp:	move.b	(a0),d1			; = E , data ID & ID#
		bmi.s	MAP_chg_3		; jump if Map unit end
		lea	8(a0),a0		;
		cmpa.l	a0,a1
		bcs.s	er_IF_bit1		; jump if not find MAP
		bra	MAP_chg_lp
MAP_chg_3:	addq.l	#1,a0			;
		dbra	d0,MAP_chg_lp
		;-------------------------------;
		; a0 : �Y�� MAP unit top address
		;-------------------------------;
MAP_chg_ex:	lea	bs_AMAPC,a2		; area map current work top
		moveq	#256/8-1,d7		; loop size
MAP_chg_2lp:	move.b	(a0)+,(a2)+		; = E , data ID , ID#
		move.b	(a0)+,(a2)+		; = start addr H
		move.b	(a0)+,(a2)+		; = 	//     M
		move.b	(a0)+,(a2)+		; = 	//     L
		move.b	(a0)+,(a2)+		; = L
		move.b	(a0)+,(a2)+		; = size H
		move.b	(a0)+,(a2)+		; =  //  M
		move.b	(a0)+,(a2)+		; =  //  L
		dbra	d7,MAP_chg_2lp
		;-------------------------------;
		lea	DSP_COEF(a5),a0		;
		move.w	#$500/4-1,d7		; loop size
		moveq	#0,d0			;
MAP_chg_DSP:	move.l	d0,(a0)+		;
		dbra	d7,MAP_chg_DSP		;
		bclr.b	#1,Mem_err_bit+3	;
		rts
		;-------------------------------;
er_IF_bit1:	bset.b	#1,Mem_err_bit+3	;
		rts
;************************************************************************
;�y�@�\�z �l�h�c�h�R���g���[���`�F���W[$Bn,$20,$xx]����B		*
;		<<  ���F �a�`�m�j �`�F���W >>				*
;�y���́zd1.w : MIDI ch# ( $00�`$1F )					*
;	 d3.b : Parameter					94/07/26*
;************************************************************************
	global	CTRL_20
CTRL_20_er:	rts
CTRL_20:	move.w	d3,d0			; ready d0 : tone-BANK#
		jsr	get_BANK_addr(pc)	; d2.l = desti. tone-BANK addr
		bcs.s	CTRL_20_er		; d1.w = MIDI ch#
		move.w	d1,d0			; = MIDI ch#
		lsl.w	#4,d0			; = 10H*MIDIch#
		add.w	_tmp_kanri(a6),d0	; + �����Ǘ��ԍ��~200H
		move.l	d2,k_BANK_adr(a6,d0.w)	;
;@		rts				;@@ 95/03/12

		move.b	#init_MIDI_VOL,k_MIDI_VOL(a6,d0.w)	; 94/11/26
		moveq	#0,d2			; ready d2 : MIDI Prg#
		move.w	_tmp_MIDI_ch(a6),d1	; ready d1 : MIDI ch#
;@@@@@@@	jmp	PRG_chg(pc)		;
;************************************************************************
;�y�@�\�z �l�h�c�h�v���O�����i���F�j�`�F���W[$Cn,$xx]����		*
;�y���́zd1.w : MIDI ch# ( 0 �` 1F )			       		*
;	 d2.b : MIDI Program change# ( 00 �` 7FH )			*
;�ydestroy�za0/d0/d1						94/07/26*
;************************************************************************
		external	er_08
		global	PRG_chg
PRG_chg:
		andi.w	#$7f,d2
		move.w	k_kanri_ofst(a6),d0	;
		move.l	k_BANK_adr(a6,d0.w),d3	; = desti. tone-BANK address
		beq.s	PRG_chg_er1
		movea.l	d3,a0
		bclr.b	#4,Mem_err_bit+3	;
		move.w	#BIN_VOICE,d3		; = 8
		add.w	d2,d3			; + Prg#
		add.w	d2,d3			; + Prg# = offset addr
		cmp.w	BIN_MIX(a0),d3		; Mixer offset addr
		bcc.s	PRG_chg_er2		; jump if Voice# too large
		bclr.b	#5,Mem_err_bit+3	;

		move.b	d2,k_PROG_no(a6,d0.w)	; <<< set MIDI Prg# >>>
		moveq	#0,d3			;
		move.w	d3,k_PBend_BF(a6,d0.w)	; <<< clear Pitch Bend >>>
		;-------------------------------;
;		global	PRG_chg_PAN
PRG_chg_PAN:	move.b	k_MIDI_PAN(a6,d0.w),d3	; clear PAN buffer
		btst	#6,d3			; SEQ PAN on ?
		bne.s	PRG_chg_PAN1		; jump if on
		andi.b	#$7F,d3
;@		ori.b	#$80,d3			; 
		move.b	d3,k_MIDI_PAN(a6,d0.w)	;
PRG_chg_PAN1:

;@	if	.not.T_E
;@		move.b	d3,k_MIDI_PAN(a6,d0.w)	; <<< clear PAN >>>
;@	endif
		;-------------------------------;
	if	RD_mode
		cmpi.w	#$E00,_tmp_kanri(a6)	; = �����Ǘ��ԍ����V�H
		bne	PRG_chg_ret		;
		lea	bs_MONTR,a0		;
		add.w	d1,d1			;
		add.w	d1,d1			;
		andi.w	#$7C,d1			;
		move.b	d2,Mem_MVC(a0,d1.w)	; set Tool I/F Monitor(Voice#)
	endif
PRG_chg_ret:	clc
		rts
PRG_chg_er2:	bset.b	#5,Mem_err_bit+3	;
		stc
		rts
PRG_chg_er1:	bset.b	#4,Mem_err_bit+3	;
		stc
		rts


