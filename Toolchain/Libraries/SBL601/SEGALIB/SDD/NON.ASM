; 93'10�`	(C)SEGA sound Room   AM2 Yamamoto
;
;	SCSP Contol Program      
;

	include	SCSP.LIB

	global		non_koff		; for symbol
	external	OCT_TB,FNSTB
	external	Note_off
	external	get_EXPTB,get_LEVEL

;�P    Key-Sprit�ɂ��Y��Layer���̌���			------> key_sprit:
;�Q    key-on ���� ���� MIDI ch & Note# slot �ւ̏��� 	------> PP_0:
;�R    key-off ���̍ŉߋ��ɂjey-on���ꂽ slot �ւ̏���	------> PP_1:
;�S    �c�u�`�@�\�̂��߂�[KEYHISTB]�f�[�^�V�t�g		------> PP_1:
;�T    �r�b�r�o���F���� [SA],[AR],..etc			------> PP_sub:
;�U    ������������  	[OCT],[FNS]			------> tune_wr:
;�V    �u-�k�ϊ��@�\    [TL] 				------> VL_CHG:
;�W    �e�l�����@�\     [MDL],[MDXSL],[MDYSL]		------> FM_set:
;�X    ���j�^�@�\        				------> set_MONITOR:
;�P�O  �����jey-�nn�@�\ [KXKB] 				------> KEY_ON:

;��������������������������������������������������������������������������
;��     MIDI note on							 ��
;���y���́z a6 = CPU work top addr.					 ��
;��	    d1.w = MIDI 1st. byte	0 �` 1FH ( MIDI ch# )		 ��
;��	    d2.b = MIDI 2nd. byte	0 �` 7FH ( note )		 ��
;��	    d3.b = MIDI 3rd. byte	0 �` 7FH ( velocity )		 ��
;��������������������������������������������������������������������������
		global	Note_on,KON11,KON12_lp
Note_on:
		move.b	d3,d3			; velocity check : 0 �` 7FH
		beq	Note_off		; jump if Velo = 0

;************************************************************************
;  �s�J�����̂݁t	Note#��Velo# ��Tool�p���j�^�G���A�ɃZ�[�u	*
;�y���́z a6    [hold] : CPU work top					*
;	  d1.w  [hold] : MIDI ch#					*
;	  d2.b  [hold] : MIDI 2nd. byte	0 �` 7FH ( note )		*
;	  d3.b  [hold] : MIDI 3rd. byte	0 �` 7FH ( velo )		*
;�y�j��z d6/a0							94/07/27*
;************************************************************************
		global	set_MONITOR
set_MONITOR:
	if	RD_mode
		move.w	_tmp_kanri(a6),d6	; = �����Ǘ��ԍ��~200H
		cmpi.w	#$E00,d6		;   �����Ǘ��ԍ����V�H
		bne	set_MONITOR_1		; jump if no
		lea	bs_MONTR,a0		;
		move.w	d1,d6			; = MIDI ch#
		add.w	d6,d6			; * 2
		add.w	d6,d6			; * 2
		move.b	d2,Mem_MNT(a0,d6.w)	; set Tool I/F Monitor(Note#)
		move.b	d3,Mem_MVL(a0,d6.w)	; set Tool I/F Monitor(Velo#)
	endif
;************************************************************************
;			�Y���org#�̎擾				94/07/27*
;************************************************************************
set_MONITOR_1:
		moveq	#0,d0			;
		move.w	k_kanri_ofst(a6),d6	;
		move.b	k_PROG_no(a6,d6.w),d0	; = voice#
;		����������������������������������������������������������
;		��  �c�r�o �b�o�t�쓮�c-�eilter	�̌���			��
; 		���y�j��z a1/a2/d5/d6					��
;		����������������������������������������������������������
		global	srch_DFLTR
srch_DFLTR:	lea	_DFL_ELMNT_wk(a6),a2
;@		movea.l	Mem_Snd_tool_pt,a1	; = sound tool I/F work top
		clr.w	d6			;
		move.b	DFL_ELMNT_NO(a6),d6	; �G�������g��  �F�w
;@		move.b	Mem_ELMNT_no(a1),d6	; = �G�������g��
		beq.s	srch_DFLTR_1		;
		movea.l	DFL_ELMNT_addr(a6),a1	; = Element data top
;@		movea.l	Mem_ELMNT_addr(a1),a1	; = Element data top
		subq.w	#1,d6			; loop size
		andi.w	#$1F,d6			;
srch_DFLTR_3:	cmp.b	(a1),d1			; Element MIDI ch = MIDI# ?
		bne.s	srch_DFLTR_2
		move.b	1(a1),DFL_wk_MSFC(a2)	;
		move.l	a1,DFL_dst_addr(a2)	;
		move.b	#0,DFL_EG_seg(a2)	; Segment# clear
		move.b	$11(a1),DFL_wk_AMP(a2)	;
		clr.w	d5			;
		move.b	$10(a1),d5		; = [FRQR]
		lsl.w	#4,d5			;
		move.w	d5,DFL_add_bs(a2)	;
		add.w	d5,d5			;
		add.w	d5,DFL_add_bs(a2)	; [FRQR]�~48 --> DFL_add_bs
		move.w	#0,DFL_add_wk(a2)	; LFO reset
srch_DFLTR_2:	lea	$18(a1),a1
		lea	$10(a2),a2
		dbra	d6,srch_DFLTR_3
srch_DFLTR_1:
;************************************************************************
;		Key-Sprit�ɂ��Y��Layer���̌���			*
; [DVA_layer]�`�ɊY��layer������ layer address �Ƃ��̔ԍ����Z�[�u	*
;�y���́z d0.l : voice#	( 00H �` 7FH )					*
;	  d1.w ; MIDI ch#						*
;	  d2.b : MIDI note#						*
;	  a6   : Sound work top						*
;�y�o�́z d6.w : layer��						*
;	  a2   : desti.Voice address					*
;�y�j��z a1/a2/d0.l/d6.l/d7.w					94/07/27*
;************************************************************************
		global	key_sprit
key_sprit:
		move.w	k_kanri_ofst(a6),d5	;
		move.l	k_BANK_adr(a6,d5.w),d6	; = desti. SCSPBIN top address
		bne.s	key_sprit_1		;
		rts				; ret if not ready [k_BANK_adr]

key_sprit_1:	movea.l	d6,a2
		add.w	d0,d0			; = Voice# * 2
		adda.w	BIN_VOICE(a2,d0.w),a2	; = desti.Voice address

		; set layer addr. into DVA_layer

	global	aaa
aaa:		lea	_DVA_layer(a6),a1	; ready
		moveq	#0,d5			; increment counter clear
		moveq	#0,d6			; increment counter clear
		move.b	V_layer_sz(a2),d7	; = Layer�� - 1 ( 0 �` 127 )
		bmi	er_21
		andi.w	#$7f,d7			; loop size ( LY_max_sz - 1 )
		moveq	#V_Layer,d0		; ready
KON12_lp:	cmp.b	LY_SNT(a2,d0.w),d2	; start note / MIDI note
		bcs.s	KON11			; jp if MIDI note < start note
		cmp.b	LY_ENT(a2,d0.w),d2	; end note / MIDI note
		bhi.s	KON11			; jp if end note < MIDI note
		move.l	a2,(a1)			;
		add.l	d0,(a1)+		; Layer addr --> (DVALY_addr)
		move.l	d5,(a1)+		; Layer# ( DVALY_NO )
		addq.w	#1,d6			; increment counter
		cmpi.w	#slot_size+1,d6		; ���������� 32���܂�
		bcc	er_24			; jump if 32 < d6
KON11:		addq.w	#1,d5			; increment counter
		addi.w	#LY_unit,d0		; next Key sprit data addr
		dbra	d7,KON12_lp		;
						; d6.w : key sprit �L��Layer��
		move.w	d6,DVA_lyr_cnt(a6)	; 0(error) or 1�`32
		move.w	d6,DVA_lyr_cntx(a6)	; 0(error) or 1�`32
		beq	er_25			; jump if �Y���kayer nothing
;************************************************************************
;			�o�k�`�x���[�h����				*
;�y���́z a2   [hold] : desti.Voice address				*
;	  d6.w [hold] : layer��					94/07/27*
;************************************************************************
		global	Play_mode
Play_mode:
		move.w	k_kanri_ofst(a6),d5	;
		move.b	d3,k_MONO_VOL(a6,d5.w)	; velocity save
		move.b	k_MONO_NT1(a6,d5.w),k_MONO_NT2(a6,d5.w)
		move.b	k_MONO_NT0(a6,d5.w),k_MONO_NT1(a6,d5.w)
		move.b	d2,k_MONO_NT0(a6,d5.w)	; new note save
		movea.l	k_BANK_adr(a6,d5.w),a3	; = desti. SCSPBIN top address

	move.b	V_vol_bias(a2),k_vol_bias(a6,d5.w)	; Volume bias
		move.b	(a2),d0			; Play mode & Bend Range
		lea	_KEYHISTB(a6),a0	;
		andi.b	#%01110000,d0		; Play "MONO" mode ?
		move.b	d0,PM_flag(a6)
		beq	Play_poly
;************************************************************************
;			�l�n�m�n���[�h����				*
;�y���́z d6.w [hold] : layer��						*
;	  a0	      : KEYHISTB address			94/07/27*
;************************************************************************
		global	Play_mono
Play_mono:
		move.w	k_kanri_ofst(a6),d5	;
		bset.b	#k_mono,k_MIDI_flg(a6,d5.w)	; set mono mode

		moveq	#slot_size,d7
		sub.w	off_slot_cnt(a6),d7	; d7 = ������slot�� ( 0 �` 32 )
		beq.s	PM_1			; jump if ������slot�� = 0
;
;	<<<<< MONO ���[�h : key-on ���� ���� MIDI ch �ւ̏��� >>>>>
;
		global	PM_0
PM_0:		subq.w	#1,d7			; loop size
		lea	slot_work(a6),a4	; a0=KEYHISTB
		move.w	HIS_off_pt(a6),d0	;
PM_lp_1:	andi.w	#HOPM,d0		; 0,2,4,...,3EH
		move.w	(a0,d0.w),d4		; = Key-History data
		andi.w	#KHMSK,d4		; $7C0
		move.b	PSPN(a4,d4.w),d5	; PCM Stream palying ?
		bmi.s	PM_2
		cmp.b	sl_MIDI(a4,d4.w),d1	; MIDI ch# equal ?
		bne.s	PM_2
		move.w	_sl_kanri(a4,d4.w),d5	; = �����Ǘ��ԍ��~200H
		cmp.w	_tmp_kanri(a6),d5	; �����Ǘ��ԍ� equal ?
		bne.s	PM_2			;
		jsr	PP_sub(pc)		;
		subq.w	#1,DVA_lyr_cnt(a6)	; 1�`32
		beq.s	PM_exit
PM_2:		addq.w	#2,d0
		dbra	d7,PM_lp_1
;
;	<<<<< MONO ���[�h : key-off ���� slot �ւ̏��� >>>>>
;
		global	PM_1
PM_1:		move.w	#slot_size-1,d7		; loop size
		move.w	HIS_on_pt(a6),d0	;
PM_lp_2:	move.w	off_slot_cnt(a6),d5	; = �񔭉��� slot��
		subq.w	#1,d5			;
		bcc.s	PM_5			; jump if �񔭉��� slot exist
		; <<< all slot ������ >>>
		addq.w	#2,HIS_off_pt(a6)	; KEYHISTB data shift
		addq.w	#1,d5

PM_5:		move.w	d5,off_slot_cnt(a6)	; = set �񔭉��� slot��
		andi.w	#HOPM,d0		; 0,2,4,...,3EH
		move.w	(a0,d0.w),d4		; key-History data
		andi.w	#KHMSK,d4		; $7C0
;@@@@		bcs.s	PM_4			; jump if Priority guard
		addq.w	#2,HIS_on_pt(a6)	; KEYHISTB data shift

		move.b	PSPN(a4,d4.w),d6	; PCM Stream playing ?
		bmi.s	PM_4
		jsr	PP_sub(pc)		;
		subq.w	#1,DVA_lyr_cnt(a6)	; 1�`32
		beq.s	PM_exit
PM_4:		addq.w	#2,d0
		dbra	d7,PM_lp_2
PM_exit:	jmp	FM_set(pc)
;************************************************************************
;			�o�n�k�x���[�h����				*
;�y���́z d6.w [hold] : layer��						*
;	  a0	      : KEYHISTB address			94/07/27*
;************************************************************************
		global	Play_poly
Play_poly:
		move.b	#0,k_MONO_NT0(a6,d5.w)	; new note clear
		lea	slot_work(a6),a4	; a0=KEYHISTB
		moveq	#slot_size,d7		; High word clear
		sub.w	off_slot_cnt(a6),d7	; = ������ slot��
		beq.s	PP_1			; jump if ������ slot nothing
;
;	<<<<< POLY ���[�h : key-on ���� ���� MIDI ch & Note �ւ̏��� >>>>>
;
		global	PP_0
PP_0:		subq.w	#1,d7			; loop size
		move.w	HIS_off_pt(a6),d0	;
PP_lp_1:	andi.w	#HOPM,d0		; 0,2,4,...,3EH
		move.w	(a0,d0.w),d4		; = Key-History data
		andi.w	#KHMSK,d4		; $7C0
		move.b	PSPN(a4,d4.w),d5	; PCM Stream palying ?
		bmi.s	PP_2
		cmp.b	sl_MIDI(a4,d4.w),d1	; MIDI ch# equal ?
		bne.s	PP_2
		cmp.b	sl_note(a4,d4.w),d2	; MIDI note# equal ?
		bne.s	PP_2
		move.w	_sl_kanri(a4,d4.w),d5
		cmp.w	_tmp_kanri(a6),d5	; �����Ǘ��ԍ� equal ?
		bne.s	PP_2
		jsr	PP_sub(pc)		;
		subq.w	#1,DVA_lyr_cnt(a6)	; 1�`32
		beq.s	PP_exit
PP_2:		addq.w	#2,d0
		dbra	d7,PP_lp_1
;
;	<<< key-on ���� ���� MIDI ch & Note �͑��݂��Ȃ� >>>
;
;	<<<<< POLY ���[�h : key-off ���� slot �ւ̏��� >>>>>
;
		global	PP_1
PP_1:		move.w	#slot_size-1,d7		; loop size
		move.w	HIS_on_pt(a6),d0	;
PP_lp_2:	move.w	off_slot_cnt(a6),d5	; = �񔭉��� slot��
		subq.w	#1,d5			;
		bcc.s	PP_5			; jump if �񔭉��� slot exist

		; <<< all slot ������ �ɑ΂��čł��ߋ��� Key-on >>>
		; <<< ���ꂽ slot �ɑ΂��ċ����g�p����          >>>

		global	all_slot_on
all_slot_on:	addq.w	#2,HIS_off_pt(a6)	; KEYHISTB data shift
		addq.w	#1,d5

		global	PP_5

PP_5:		move.w	d5,off_slot_cnt(a6)	; = set �񔭉��� slot��
		andi.w	#HOPM,d0		; 0,2,4,...,3EH
		move.w	(a0,d0.w),d4		; key-History data
		andi.w	#KHMSK,d4		; $7C0
		addq.w	#2,HIS_on_pt(a6)	; KEYHISTB data shift

		move.b	PSPN(a4,d4.w),d6	; PCM Stream playing ?
		bmi.s	PP_4
		jsr	PP_sub(pc)		;
		subq.w	#1,DVA_lyr_cnt(a6)	; 1�`32
		beq.s	PP_exit
PP_4:		addq.w	#2,d0
		dbra	d7,PP_lp_2
PP_exit:
;		����������������������������������������������������������
;		�� <<<<<<<<<<<<<<<<<<<< �e�l�����ݒ� >>>>>>>>>>>>>>>>>> ��
; 		���y���́z a4   [hold] : slot work top			��
; 		��	   a5   [hold] : SCSP[FH1005]			��
; 		��	   a6   [hold] : work top			��
; 		���y�j��z d0/d1/d2/d3/d4/d7/a0/a1/a2			��
; 		���yFree�z a3						��
;		����������������������������������������������������������
		global	FM_set
FM_set:		lea	_DVA_layer(a6),a1	;
		move.w	DVA_lyr_cntx(a6),d7	; = �Y��Layer�� : 1�`32 
		subq.w	#1,d7			; loop size
FM_set_lp_0:	movea.l	(a1),a2			; Carria layer addr(DVALY_addr)
		move.w	DVALY_slofst(a1),d4	; $00,$40,...,$7C0

		; <<<< ���� key-on ���� ( key-off ) >>>>

		lsr.w	#1,d4			; $00,$20,...,$3E0
		move.b	SCSP_KXKB(a5,d4.w),d5	; @0001
		andi.b	#%00000111,d5		; clear KYONEX
;@@@ 94/12/15
		btst.b	#legart,PM_flag(a6)	;
		beq.s	FM_set_3		; jump if ���K�[�g
		ori.b	#%00011000,d5		; set   KYONB ( key-on ���� )
FM_set_3:	ori.b	#%00001000,d5		; set   KYONB ( key-on ���� )
		move.b	d5,SCSP_KXKB(a5,d4.w)	; write KYONEX & KYONB
		add.w	d4,d4			; $00,$40,...,$7C0

		move.w	LY_MDL(a2),d0		; Carria [MDL] = 0 ?
		beq.s	FM_set_next		; jump if not receive [SOUS]

		move.b	LY_FM+0(a2),d1		; Module GN & FMLY
		jsr	FM_SUB(pc)		; <<<< MDXSL >>>>
		or.w	d3,d0			; d0 : mmmm xxxx xx00 0000
		move.b	LY_FM+1(a2),d1		; Module GN & FMLY
		jsr	FM_SUB(pc)		; <<<< MDYSL >>>>
		lsr.w	#6,d3			; d3 : 0000 0000 00yy yyyy
		or.w	d3,d0			; d0 : mmmm xxxx xxyy yyyy

		lsr.w	#1,d4			; $00,$20,...,$3E0
		move.w	d0,SCSP_MDLSL(a5,d4.w)	; write [MDL],[MDXSL],[MDYSL]
		;-------------------------------;
FM_set_next:	addq	#8,a1			;
		dbra	d7,FM_set_lp_0		;
;		����������������������������������������������������������
;		��	 	      �����jey-on ���s			��
;		����������������������������������������������������������
		global	KEY_ON
KEY_ON:
		move.b	(a5),d0			;
		ori.b	#%00010000,d0		; set KYONEX
		move.b	d0,(a5)			; write KYONEX & KYONB
		rts
;		����������������������������������������������������������
;		��	 	      �G���[�r�b�g�ݒ�			��
;		����������������������������������������������������������
er_25:		bset.b	#0,Mem_err_bit+1	; �Y���kayer nothing
		rts
;		����������������������������������������������������������
;		��	 		�e �l �� ��			��
;		����������������������������������������������������������
		global	FM_SUB
FM_SUB:		move.b	d1,d2			;
		andi.b	#$7F,d1			; Voice��Layer# ( 0,1,2,.. )
		lea	_DVA_layer(a6),a0	;
GSNY_lp:	cmp.b	DVALY_NO(a0),d1		; Layer# equal ?
		beq.s	GSNY_2			; jump if equal
		addq	#DVALY_unit,a0		;
		bra.s	GSNY_lp			;
GSNY_2:		move.w	DVALY_slofst(a0),d3	; $00,$40,...,$7C0

get_MDYSL:	sub.w	d4,d3			; Module - Carria
		andi.w	#$7c0,d3		;
		cmpi.w	#$700,d3		;
		bcs.s	get_MDY_1		;
		addi.w	#$800,d3		;
get_MDY_1:	move.b	d2,d2			; Generation H or L ?
		bpl.s	get_MDY_2		; jump if �ŐVSample
		addi.w	#$800,d3		;
get_MDY_2:	andi.w	#$FC0,d3		;
		rts

; ������������������������������������������������������������������������
; ��     	     <<<< ���F write on �r�b�r�o >>>>	94'03/20	��
; �ņ���������������������������������������������������������������������
; ��         d0 H [----]						��
; ��         d0.w [hold] : History offset point                         ��
; ���y���́z d1  L[hold] : MIDI ch#                                     ��
; ��             H[dstr] : work                                         ��
; ���y���́z d2  L[hold] : MIDI note#                                   ��
; ��             H[dstr] : work                                         ��
; ���y���́z d3  L[hold] : MIDI velo#                                   ��
; ��             H[dstr] : work                                         ��
; ���y���́z d4.w [hold] : $00,$40,$60,...,$7C0  for slot offset    	��
; ��         d5.l [dstr] : work                                         ��
; ��         d6.l [dstr] : work                                         ��
; ��         d7  L[hold] : loop size                                    ��
; ��             H[hold] : $00,$08,$10,...,$F8   DVA_layer offset work  ��
; ��         a0   [hold] : KEYHISTB                                     ��
; ��         a1   [dstr] : work                                         ��
; ��         a2   [dstr] : Layer address work                           ��
; ���y���́z a3   [hold] : SCSPBIN top                                  ��
; ���y���́z a4   [hold] : slot work top                                ��
; ���y���́z a5   [hold] : SCSP[FH1005]                                 ��
; ���y���́z a6   [hold] : work top                                     ��
; ������������������������������������������������������������������������
		global	PP_sub
PP_sub:
	; <<< note-on �̂��߂� key-off ���s >>>	; 95/03/06
		lsr.w	#1,d4			; 95/03/06
		move.b	SCSP_KXKB(a5,d4.w),d5	; 95/03/06
		andi.b	#%00000111,d5		; 95/03/06
		ori.b	#%00010000,d5		; 95/03/06
non_koff:	move.b	d5,SCSP_KXKB(a5,d4.w)	; 95/03/06
		add.w	d4,d4			; 95/03/06

		; <<<< set slot work data >>>>

;@	bclr.b	#flg_DMPR,sl_flag2(a4,d4.w)
	bset.b	#flg_KON,sl_flag2(a4,d4.w)	; set key-on�� mode
	bset.b	#flg_non,sl_flag2(a4,d4.w)	; set Note-on�� mode

	move.w	_tmp_kanri(a6),_sl_kanri(a4,d4.w)
	move.b	d1,sl_MIDI(a4,d4.w)
	move.b	d2,sl_note(a4,d4.w)
	move.b	d3,sl_velo(a4,d4.w)
;	����������������������������������������������������������
;	��	 Key-off �̎��s	�� Key-on �̏���		��
;	��	 �g�`��΃A�h���X�̎Z�o���ݒ� ��		��
;	����������������������������������������������������������
	swap	d7				; d7.l : L = DVA_layer offset
	lea	_DVA_layer(a6),a2		; ready

	move.w	d4,DVALY_slofst(a2,d7.w)	; $00,$40,...,$7C0

	movea.l	(a2,d7.w),a2			; desti. Layer addr(DVALY_addr)
	move.w	LY_MDL(a2),d6			; [MDL]
	move.l	a2,sl_layer_adr(a4,d4.w)	;
	move.l	LY_SA(a2),d5			; = [PEON] �` [SA]
	add.l	a3,d5				; + SCSPBIN top

	lsr.w	#1,d4				; $00,$20,$40,....,$3E0
	global	CCCC
;@@@@@@	move.b	#$08,$0B(a5,d4.w)		; release off 94/12/15
;@	andi.l	#$E7FF,d5			; 94/12/15
CCCC:	move.l	d5,SCSP_KXKB(a5,d4.w)		; [SPCTL]�`[SA]
	move.w	d6,SCSP_MDLSL(a5,d4.w)		; [MDL]�`
	move.l	LY_LSA(a2),d5
	move.l	d5,SCSP_PCM_LSA(a5,d4.w)	; [LSA]�`[LEA]
	move.l	LY_D2R(a2),SCSP_D2R1R(a5,d4.w)	; [D2R]�`[RR]
	move.b	LY_SISD(a2),d6

	move.w	LY_RELFO(a2),d5			;
	move.b	d6,SCSP_SISD(a5,d4.w)		; [SI],[SD]
	bpl.s	PP_sub_1			; jump if Hard-ware M-wheel
	andi.w	#$ff18,d5			;
PP_sub_1:					;
	move.w	d5,SCSP_RELFO(a5,d4.w)		; [LFORE]�`
	move.w	LY_ISEL(a2),SCSP_ISEL(a5,d4.w)	;
		;-------------------------------;
		global	PP_sub_PAN
PP_sub_PAN:	move.b	LY_DSDPN(a2),d6		; d6 = Layer [DISDL] & [DIPAN]
		move.b	SND_OUT_ST(a6),d5	; MONO/STEREO status
		bpl.s	PP_sub_STEREO		; jump if STEREO mode
		andi.b	#$E0,d6			; --> Center
		bra.s	PP_sub_3		;
PP_sub_STEREO:	move.w	k_kanri_ofst(a6),d5	;
		move.b	k_MIDI_PAN(a6,d5.w),d5	;
		btst	#6,d5			; SEQ PAN on/off ?
		bne.s	PP_sub_SP_on		; jump if Seq PAN on
		move.b	d5,d5			;
		bpl.s	PP_sub_3		; jump if Layer PAN on
PP_sub_SP_on:	andi.b	#$1F,d5			; = PAN buffer
		andi.b	#$E0,d6			;
		or.b	d5,d6			;
PP_sub_3:	move.b	d6,SCSP_DISDLPN(a5,d4.w)	;
		add.w	d4,d4			;
		move.b	d6,sl_DISDLPAN(a4)	;
		lsr.w	#1,d4			;
;		����������������������������������������������������������
;		��			�s�k�Z�o			��
;		����������������������������������������������������������
		global	wr_vol
wr_vol:		clr.w	d6
		btst.b	#FMCB,LY_SA(a2)		; carria ?
		bne.s	VL_CHG			; jump if FM carria

		move.b	LY_ISLMX(a2),d6		; = [ISEL]&[IMXL]
		andi.b	#7,d6			; = [IMXL] only ( Effect send )
		bne.s	VL_CHG			; jump if carria
		move.b	LY_DSDPN(a2),d6		; = [DISDL]&[DIPAN] 
		andi.b	#$E0,d6			; = [DISDL] only ( Direct send )
		bne.s	VL_CHG			; jump if carria
		;-------------------------------;
		move.b	LY_TL(a2),d6		; = [TL] only
		add.w	d4,d4
		bclr.b	#flg_FMCR,sl_flag2(a4,d4.w)
		lsr.w	#1,d4
		bra	VOL_WR_EXE
;		����������������������������������������������������������
;		��		�uelocity - �kevel �ϊ�			��
;		��d3.b = MIDI velocity					��
;		����������������������������������������������������������
		global	VL_CHG
VL_CHG:
		add.w	d4,d4
		bset.b	#flg_FMCR,sl_flag2(a4,d4.w)
		lsr.w	#1,d4

		move.b	d3,d6			; $00 �` $7F(max) : MIDI velo
		swap	d1
		swap	d2
		swap	d3

		movea.l	a3,a1			; = SCSPBIN top addr
		adda.w	BIN_VL(a1),a1

		moveq	#0,d5			;
		move.b	LY_VLNO(a2),d5		; = VL�ϊ��ԍ�
		add.w	d5,d5			;
		adda.l	d5,a1			;
		add.w	d5,d5			;
		add.w	d5,d5			;
		adda.l	d5,a1			; = desti. VL data top

		clr.w	d1		; = Vx initial
		clr.w	d2		; = Lx initial
		move.b	(a1)+,d3	; = K0 initial
		;-----------------------;
VL_chg_lp:	cmp.b	(a1),d6		; (a1)=V0 / MIDI Velo
		bls.s	VL_chg_0	; jump if 1st Seg
		move.b	(a1)+,d1	; = V0
		move.b	(a1)+,d2	; = L0
		move.b	(a1)+,d3	; = K1

		cmp.b	(a1),d6		; (a1)=V0
		bls.s	VL_chg_0	; jump if 2nd Seg
		move.b	(a1)+,d1	; = V1
		move.b	(a1)+,d2	; = L1
		move.b	(a1)+,d3	; = K2

		cmp.b	(a1),d6		; (a1)=V0
		bls.s	VL_chg_0	; jump if 3rd Seg
		move.b	(a1)+,d1	; = V2
		move.b	(a1)+,d2	; = L2
		move.b	(a1)+,d3	; = K3

VL_chg_0:	sub.b	d1,d6		; = �u
		move.b	d3,d1
		andi.w	#7,d1		;
		add.w	d1,d1		;
		lsr.b	#4,d3
		jmp	VLJPTB(pc,d1.w)

VLJPTB:		bra.s	VL_0
		bra.s	VL_1
		bra.s	VL_2
		bra.s	VL_3
		bra.s	VL_4
		bra.s	VL_5
		bra.s	VL_6
		bra.s	VL_7

VL_4:	; �X�����O
VL_0:	; �X�����}��
		moveq	#0,d6		; �u = 0
VL_2:	; �X�����{�P
		add.b	d2,d6		; = Lx + �u*�P
		bra.s	VL_exit
VL_6:	; �X�����|�P
		sub.b	d2,d6		; = �u*�P - Lx
		neg.b	d6		; = Lx - �u*�P
		bra.s	VL_exit
VL_1:	; �P���X������
		bcs.s	VL_1A
		lsl.b	d3,d6		; 2,4,8,...�{
		add.b	d2,d6
		bra.s	VL_exit

VL_1A:		andi.w	#$7f,d6		; 1.5,3,6,12,....�{
		lsl.w	#2,d6		; = �u�~�S
		move.w	d6,d1
		add.w	d1,d6
		add.w	d1,d6
		lsl.w	d3,d6
		lsr.w	#3,d6
		add.b	d2,d6
		bra.s	VL_exit

VL_7:	; �|�����X�����|�P
		bcs.s	VL_7A
		lsl.b	d3,d6		; -2,-4,-8,...�{
		bra.s	VL_5A
VL_7A:		andi.w	#$7f,d6		; -1.5,-3,-6,-12,....�{
		lsl.w	#2,d6		; = �u�~�S
		move.w	d6,d1
		add.w	d1,d6
		add.w	d1,d6
		lsl.w	d3,d6
		lsr.w	#3,d6
		bra.s	VL_5A
VL_3:	; �O���X�����P
		lsr.b	d3,d6
		add.b	d2,d6
		bra.s	VL_exit
VL_5:	; �|�P���X�����O
		lsr.b	d3,d6
VL_5A:		sub.b	d2,d6		; = �u*�P - Lx
		neg.b	d6		; = Lx - �u*�P
		;-----------------------;
VL_exit:	bpl.s	VL_8
		clr.w	d2
		add.b	d6,d6
		bmi.s	VL_9
		move.w	#$7f,d2
VL_9:		move.w	d2,d6
VL_8:		swap	d1
		swap	d2
		swap	d3

;		����������������������������������������������������������
;		�� d6 = (MIDI velo)*(V-L) �u�k�ϊ��f�[�^ ( $00�`$7F )	��
;		����������������������������������������������������������
	global	VL_00
VL_00:
		andi.w	#$7F,d6			; 0000�`007F(Max)
		addq.w	#1,d6			; 0001�`0080(Max)
		moveq	#0,d5			;
		move.b	LY_TL(a2),d5		; 00FF�`0000(max)
		not.b	d5			; 0000�`00FF(max)
		addq.w	#1,d5			; 0001�`0100(max)
		mulu.w	d5,d6			; d6 = 0001�`8000(Max)
		add.w	d4,d4			;
		move.w	d6,sl_VL(a4,d4.w)	;
		lsr.w	#1,d4			;

		; MIDI Volume Control [ $Bn,$07,$xx ]

		move.w	k_kanri_ofst(a6),d5	;
		move.b	k_MIDI_VOL(a6,d5.w),d5	;
		andi.w	#$7F,d5			; 00 �` 7F(Max)
		addq.w	#1,d5			; 01 �` 80(Max)
		mulu	d5,d6			; d6 = 0001�` 400000(Max)
		add.l	d6,d6			;    = 0002�` 800000(Max)
		add.l	d6,d6			;    = 0004�`1000000(Max)
		subq.l	#1,d6			;    = 0003�` FFFFFF(Max)
		swap	d6			; 00 �` FF(Max)
		andi.w	#$FF,d6			; 0000 �` 00FF(Max)

		move.w	k_kanri_ofst(a6),d5	; 10/01
		move.b	k_vol_bias(a6,d5.w),d5	; 80 �` 00 �` 7F
		ext.w	d5			; FF80 �` 0000 �` 007F
		add.w	d5,d6			;
		bmi.s	VL_01			;
		cmpi.w	#$100,d6		;
		bcs.s	VL_02			;
		move.b	#$ff,d6			;
		bra.s	VL_02			;
VL_01:		clr.w	d6			;
VL_02:		not.b	d6			; = attenuation
;		����������������������������������������������������������
;		��			�s�k�ݒ�			��
;		����������������������������������������������������������
		global	VOL_WR_EXE
VOL_WR_EXE:
		move.b	d6,SCSP_TLVL(a5,d4.w)	; [TL]
		swap	d2		;+	; d2.H = MIDI note# / L = free
;		����������������������������������������������������������
;		��	 	      ����������			��
;		����������������������������������������������������������
		global	PLFO_ON
PLFO_ON:	add.w	d4,d4			; $00,$40,....,$7C0
		bclr.b	#PLON_flg,sl_flag1(a4,d4.w)
		btst.b	#PLON,LY_SA(a2)		; PLFO on ?
		beq.s	PEG_ON			; jump if off
		jsr	PLFO_init(pc)		; <d0/d1/a1>
		;-------------------------------;
		global	PEG_ON
PEG_ON:		clr.w	d5			; for ���� ( �{���� )
		bclr.b	#PEON_flg,sl_flag1(a4,d4.w)
		btst.b	#PEON,LY_SA(a2)		; PLFO on ?
		beq.s	PEG_ON_e		; jump if off
		jsr	PEG_init(pc)		; return d5.w:����
		;-------------------------------;
PEG_ON_e:	lsr.w	#1,d4			; $00,$20,....,$3E0
		jsr	tune_wr(pc)		; ready d5.w:����
;		����������������������������������������������������������
;		��	 	      [LFORES] ����			��
;		����������������������������������������������������������
		bclr.b	#7,SCSP_RELFO(a5,d4.w)	; [LFORES]����

		swap	d2		;+	; revival d2.w : MIDI note#
		addq.w	#DVALY_unit,d7		; set next DVA_layer offset
		swap	d7			; revival d7.w : loop size
		add.w	d4,d4			; $00,$40,$60,...,$7C0
		rts
; ������������������������������������������������������������������������
; ��     	     <<<< ���� write on �r�b�r�o >>>>			��
; �ņ���������������������������������������������������������������������
; ��  	       d0 H [----]						��
; ��           d0 L [hold] : History offset point			��
; ��  	       d1 H [----]						��
; ��  	       d1 L [hold] : MIDI ch#					��
; ��  	       d2 H [hold] : MIDI note#					��
; ���� 	       d2 L [dstr] : work					��
; ��  	       d3 H [----]						��
; ��  	       d3 L [hold] : MIDI velo#					��
; �����y���́z d4.w [hold] : slot offset ( $00,$20 �` $3E0 )		��
; �����y���́z d5.w [dstr] : ���� XXYYH ( XXH = ���� , YYH = �Z���g )	��
; ���� 	       d6.l [dstr] : work		 			��
; ��  	       d7   [hold]						��
; ��  �y���́z a0   [hold] : KEYHISTB					��
; ��  	       a1   [----] : ?						��
; �����y���́z a2   [dstr] : destination Layer address			��
; ��  �y���́z a3   [hold] : SCSPBIN top				��
; �����y���́z a4   [hold] : slot work top 				��
; �����y���́z a5   [hold] : SCSP[FH1005] 				��
; ��  �y���́z a6   [hold] : work top					��
; ������������������������������������������������������������������������
		global	tune_wr
tune_wr:
		add.w	d4,d4			; = $00,$40,...,$7C0
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
		bcs.s	SD_tune_1
		cmpi.w	#$706/2,d2
		bcs.s	SD_tune_2
		cmpi.w	#$9b2/2,d2
		bcs.s	SD_tune_3
		addi.w	#$100,d5		; $9B2 �` $BFE cent = +$300
SD_tune_3:	addi.w	#$100,d5		; $706 �` $9B0 cent = +$200
SD_tune_2:	addi.w	#$100,d5		; $3DE �` $704 cent = +$100
SD_tune_1:					; $000 �` $3DC cent = +$000
		; d5.w = SCSP [FNS] data $000 �` $3FF
		or.w	d5,d6			; [OCT]+[FNS]
		lsr.w	#1,d4			; = $00,$20,....,$3E0
		move.w	d6,SCSP_OCTFNS(a5,d4.w)
		rts
; ������������������������������������������������������������������������
; ��     		�o�k�e�n �� ��  �i�jey-�nn���j			��
; �ņ���������������������������������������������������������������������
; ����         d0 H [dstr] 						��
; ��           d0 L [hold] : History offset point                       ��
; ��  	       d1 L [hold]                                              ��
; ��  	       d2 H [hold]						��
; ���� 	       d2 L [dstr] : work		 			��
; ��  	       d3 L [hold]                                              ��
; �����y���́z d4.w [hold] : slot offset ( $00,$40 �` $7C0 )		��
; ��  	       d5.w [----]						��
; ���� 	       d6.l [dstr] : work		 			��
; ��  	       d7   [hold]						��
; ��  �y���́z a0   [hold] : KEYHISTB                                   ��
; ���� 	       a1   [----] : ?						��
; �����y���́z a2   [hold] : destination Layer address			��
; �����y���́z a3   [hold] : SCSPBIN top                                ��
; �����y���́z a4   [hold] : slot work top 				��
; ��  �y���́z a5   [hold] : SCSP[FH1005] 				��
; ��  �y���́z a6   [hold] : work top					��
; ������������������������������������������������������������������������
; PLFO_Delay    :       XX XXH : 0000�`FFFFH(65535) [ �~1msec ]
; PLFO_FRQR_bs  : 00 XX XX 00H : 0�`99.99�Z���g	    [ 1/1000000H�Z���g]
; PLFO_FRQR_bs  : XX XX XX 00H : -99.99�`+99.99���  [ 1/1000000H�Z���g]
; PLFO_HTCNT_bs :       0X XXH : 0000�`0FFFH(4095)  [ �~1msec ]
; PLFO_HTCNT_wk :       0X XXH : 0000�`0FFFH(4095)  [ �~1msec ]
; PLFO_FDCNT    :       0X XXH : 1,3,5,7,...,7FDH  �t�F�C�h���Ԃ̕������]��

		global	PLFO_init
PLFO_init:	swap	d0
		moveq	#0,d6			;
		move.l	d6,PLFO_cent(a4,d4.w)	; cent work ������

		movea.l	a3,a1			; = SCSPBIN top
		move.w	BIN_PLFO(a1),d6		; = PLFO data offset addr.
		adda.l	d6,a1			; SCSPBIN top + PLFO offset
		clr.w	d2			;
		move.b	LY_PLFO_NO(a2),d2	; PLFO#
		add.w	d2,d2			;
		add.w	d2,d2			;  ( PLFO data unit = 4 byte )
		adda.w	d2,a1			; a1 = dest. PLFO data top

		move.b	(a1)+,d0		; = [DLY] EXP������ data
		jsr	get_EXPTB(pc)		; return d0.w
		lsr.w	#2,d0			; = delay counter (1msec)
		move.w	d0,PLFO_Delay(a4,d4.w)	; = delay counter

		move.b	(a1)+,d0		; = [FRQR] EXP������ data
		jsr	get_EXPTB(pc)		; return d0.w
		move.w	d0,d6			;
		swap	d6			; XX XX ?? ??
		clr.w	d6			; XX XX 00 00
		lsr.l	#8,d6			; 00 XX XX 00 (1msec)
		move.l	d6,PLFO_FRQR_bs(a4,d4.w)

		move.b	(a1)+,d0		; = [HT] EXP������ data
		jsr	get_EXPTB(pc)		; return d0.w
		lsr.w	#4,d0			; 0X XX (1msec)
		move.w	d0,PLFO_HTCNT_bs(a4,d4.w)
		move.w	d0,d2			;

		move.b	(a1),d0			; = [FDCNT] data
		jsr	get_EXPTB(pc)		; return d0.w
		lsr.w	#6,d0			; 0,1,2,3,4,....3FF
		bne.s	PLFO_INIT1		; 	FDCNT=[FDCT]^2/40H
		lsr.w	#1,d2			; HTCNT/2 (1/4������)
		bra.s	PLFO_INIT2		;
PLFO_INIT1:	add.w	d0,d0			;
		subq.w	#1,d0			; = �t�F�C�h���Ԃ̕������]��
PLFO_INIT2:	move.w	d2,PLFO_HTCNT_wk(a4,d4.w)
		move.w	d0,PLFO_FDCNT(a4,d4.w)	;
		move.l	PLFO_FRQR_bs(a4,d4.w),PLFO_FRQR_wk(a4,d4.w)
		bset.b	#PLON_flg,sl_flag1(a4,d4.w)
		swap	d0
		rts

; ������������������������������������������������������������������������
; ��     		�o�d�f �� ��  �i�jey-�nn���j			��
; �ņ���������������������������������������������������������������������
; ����         d0 H [dstr] 						��
; ��           d0 L [hold] : History offset point                       ��
; ��  	       d1 L [hold]                                              ��
; ��  	       d2 H [hold]						��
; ���� 	       d2 L [dstr] : work		 			��
; ��  	       d3 L [hold]                                              ��
; �����y���́z d4.w [hold] : slot offset ( $00,$40 �` $7C0 )		��
; ��  �y�o�́z d5.w [----] : ����					��
; ���� 	       d6.l [dstr] : work		 			��
; ��  	       d7   [hold]						��
; ��  �y���́z a0   [hold] : KEYHISTB                                   ��
; ���� 	       a1   [----] : ?						��
; �����y���́z a2   [hold] : destination Layer address			��
; �����y���́z a3   [hold] : SCSPBIN top                                ��
; �����y���́z a4   [hold] : slot work top 				��
; ��  �y���́z a5   [hold] : SCSP[FH1005] 				��
; ��  �y���́z a6   [hold] : work top					��
; ������������������������������������������������������������������������
		global	PEG_init
PEG_init:
		move.w	d0,work_temp(a6)	; stack d0
		move.b	#1,PEG_SEG(a4,d4.w)	; = PEG Segment# ������

		moveq	#0,d6			;
		movea.l	a3,a1			; = SCSPBIN top
		move.w	BIN_PEG(a1),d6		; = PEG data offset addr.
		adda.l	d6,a1			; SCSPBIN top + PEG offset
		clr.w	d2			;
		move.b	LY_PEG_NO(a2),d2	; PEG#
		add.w	d2,d2			;
		adda.w	d2,a1			; + 2*PEG#
		add.w	d2,d2			;   ( PEG data unit = 10 byte )
		add.w	d2,d2			;
		adda.w	d2,a1			; a1 = dest. PEG data top
		move.l	a1,PEG_addr(a4,d4.w)	;

		move.b	(a1)+,d0		; = [DLY] EXP������ data
		jsr	get_EXPTB(pc)		; return d0.w
		lsr.w	#2,d0			; = delay counter (1msec)
		move.w	d0,PEG_dly_cnt(a4,d4.w)	; = delay counter (1msec)

		move.b	(a1)+,d0		; = [OL] EXP������ data
		jsr	get_LEVEL(pc)		; return d0.w:-$5FFF�`+$5FFF
		swap	d0
		move.w	d0,PEG_cent(a4,d4.w)	; cent work ������
		move.w	d0,d5			; ready d5.w
		clr.w	d0
		move.w	d0,PEG_cent+2(a4,d4.w)	; cent work ������

		bset.b	#PEON_flg,sl_flag1(a4,d4.w)
		move.w	work_temp(a6),d0	; get stack d0
		rts

er_21:		bset.b	#5,Mem_DRVERR_FLG+1	; ���FBANK����Layer�����s��
		rts
er_24:		bset.b	#6,Mem_DRVERR_FLG+1	; ����������32���𒴉�
		rts

