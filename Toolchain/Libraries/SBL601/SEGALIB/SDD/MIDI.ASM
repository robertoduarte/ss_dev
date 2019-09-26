; 93'10�`	(C)SEGA sound Room   AM2 Yamamoto
;
;	SCSP Contol Program      
;

	include	SCSP.LIB

	external	PEG_key_off			; [  TRGT.ASM]
	external	Note_on
	external	Note_off
	external	MIXER_wr,PRG_chg
	external	CTRL_10

	global		MIDI_ctrl

;  <<<< ���������� >>>>
;
;	  CTRL_01	���W�����[�V����  �z�B�[��			
;	  MIDI_En	�s�b�`  �x���h					
;	  PRG_chg	�v���O���� �i���F�j�`�F���W			
;	  CTRL_5B	�dffect �`�F���W�i�c�r�o �licro �orogram �X�V�j	
;	  CTRL_40	�_���p�[					
;	  CTRL_20	�a�`�m�j�`�F���W	----> CHG.ASM		
;	  CTRL_10	�l�h�w�d�q�`�F���W				
;	  CTRL_07	���ʃR���g���[��				
;	  CTRL_0A	�o�`�m�R���g���[��				
;	  CTRL_11	�dffect �p�� �`�F���W				
;	  CTRL_47	�dffect �uol �`�F���W				
;	  CTRL_50	3D

	external	CTRL_20

;************************************************************************
;�y �@ �\ �z �����݂ɂăo�b�t�@�����O���ꂽ�l�h�c�h�f�[�^�����o����	*
;	     �m�[�g�I��/�I�t�A�v���O�����`�F���W�A�R���g���[���`�F���W	*
;            ���𐧌䂷��B						*
;�y �� �� �za5/a6: FH1005 top / work RAM top				*
;	    d0.w : MIDI buffer offset point				*
;�y �o �� �znothing							*
;�ydestroy�zd0.w/d1.w/d2.b/d3.b/d4.w					*
;�y hold  �za5/a6							*
;************************************************************************


		global	MIDI_ctrl
MIDI_ctrl:	moveq	#0,d0
		move.b	_MIDI_RCV_RDPT(a6),d0	; �X�V
		addq.b	#1,_MIDI_RCV_RDPT(a6)	; �X�V 11/16
		add.w	d0,d0			;
		add.w	d0,d0			;
		lea	_MIDI_RCV_BF(a6),a1	;
		adda.w	d0,a1			;
		;-------------------------------;
		;  SCSP-MIDI OUT   95/03/09	;
		;-------------------------------;
MIDI_out_st:	lea	_MIDI_OUT_BF(a6),a2	;
		moveq	#0,d4			;
		move.b	_MIDI_OUT_WRPT(a6),d4	; byte
		move.b	bs_SIFWK+OF_HNDSHK_flg,d0	; HOST I/F H-shake flg
		add.b	d0,d0			;
		bpl.s	MIDI_out_end		; jump if bit6 = L
		move.b	(a1),d0			; Priority(7-3) & �R�}���h(2-0)
		lsl.b	#4,d0			;
		ori.b	#$80,d0			;
		move.b	1(a1),d1		; �����Ǘ��ԍ� �� MIDI ch#
		andi.b	#$0F,d1			;
		or.b	d1,d0			;
		move.b	d0,(a2,d4.w)		; MIDI 1st
		addq.b	#1,d4			;
		cmpi.b	#$C0,d0			;
		bcs.s	MIDI_out_st_3		; jump if 3byte out
		cmpi.b	#$E0,d0			;
		bcc.s	MIDI_out_st_3		; jump if 3byte out
		move.b	2(a1),(a2,d4.w)		; MIDI 2nd
		bra.s	MIDI_out_st_2		; jump if 2byte out
MIDI_out_st_3:	move.b	2(a1),(a2,d4.w)		; MIDI 2nd
		addq.b	#1,d4			;
		move.b	3(a1),(a2,d4.w)		; MIDI 3rd
MIDI_out_st_2:	addq.b	#1,d4			;
		move.b	d4,_MIDI_OUT_WRPT(a6)	; byte
		;-------------------------------;
MIDI_out_end:	move.b	(a1)+,d0		; Priority(7-3) & �R�}���h(2-0)
		move.b	(a1)+,d1		; �����Ǘ��ԍ� �� MIDI ch#
		move.b	d1,d2			;
		andi.w	#$E0,d2			; = �����Ǘ��ԍ��~20H
		lsl.w	#4,d2			; = �����Ǘ��ԍ��~200H
		move.w	d2,_tmp_kanri(a6)	;
		andi.w	#$1F,d1			; = MIDI ch#

		move.w	d1,d2			;
		lsl.w	#4,d2			; MIDI ch * $10
		add.w	_tmp_kanri(a6),d2	; = �����Ǘ��ԍ��~200H
		move.w	d2,k_kanri_ofst(a6)	;

		move.w	d1,_tmp_MIDI_ch(a6)	; word	; $0000 �` $001F
		move.b	(a1)+,d2		; 2nd Message ( Note# )
		move.b	(a1),d3			; 3rd Message ( Velo )
		;-------------------------------;
		andi.w	#7,d0			; 00000xxx
		lsl.w	#2,d0			; 000xxx00

		jmp	MIDI_JPTB(pc,d0.w)
MIDI_JPTB:	jmp	Note_off(pc)	; 8n ; note off
		jmp	Note_on(pc)	; 9n : note on
		jmp	er_1C(pc)	; An : polyphonic key pressure
		jmp	CTRL_CHG(pc)	; Bn : control change
		jmp	PRG_chg(pc)	; Cn : program change
		jmp	er_1D(pc)	; Dn : channel pressure
		jmp	MIDI_En(pc)	; En : pitch bend change
		jmp	er_1E(pc)	; Fn :

er_1C:		bset.b	#2,Mem_DRVERR_FLG+1	; MIDI Polyphonic Key pressure
		rts
er_1D:		bset.b	#3,Mem_DRVERR_FLG+1	; MIDI channel pressure
		rts
er_1E:		bset.b	#4,Mem_DRVERR_FLG+1	; MIDI F0H�`FFH input
		rts
;************************************************************************
;�y �@ �\ �z �e�X�̂l�h�c�h�R���g���[���`�F���W[$Bn,$cc,$xx]�ԍ��֕���	*
;	     ������B����`�̃R���g���[���ԍ��ɑ΂��Ă̓G���[�����i�v��	*
;	     �O�����J���p�j�܂��͖������i���@�p�j�Ƃ���B		*
;�y �� �� �za6   : work RAM top						*
;	    d1.w : MIDI ch#            	( 00 �` 1FH )			*
;	    d2.b : MIDI Control Number 	( 00 �` 7FH )			*
;           d3.b : Parameter 		( 00 �` 7FH )			*
;�y �o �� �znothing							*
;�ydestroy�zd1.w							*
;�y hold  �za5/a6						94/07/28*
;************************************************************************
		global	CTRL_CHG
CTRL_CHG:	move.w	d2,d4
		andi.w	#$0F,d2			; 0000 xxxxB
		add.w	d2,d2
		add.w	d2,d2
		andi.w	#$70,d4			; 0xxx 0000B
		lsr.w	#2,d4			; 000x xx00B
		jmp	MIDI_Bn_TB0(pc,d4.w)	;
MIDI_Bn_TB0:	jmp	Bn_00_0F(pc)
		jmp	Bn_10_1F(pc)
		jmp	Bn_20_2F(pc)
		jmp	Bn_30_3F(pc)
		jmp	Bn_40_4F(pc)
		jmp	Bn_50_5F(pc)
		jmp	Bn_60_6F(pc)
		jmp	Bn_70_7F(pc)

Bn_00_0F:	jmp	MIDI_Bn_TB1(pc,d2.w)
MIDI_Bn_TB1:	jmp	CTRL_NO_er(pc)	; $00
		jmp	CTRL_01(pc)	; $01 �����n�[�h Modulation Wheel
		jmp	CTRL_NO_er(pc)	; $02 �s����i�u���X�^�C�v����q�j
		jmp	CTRL_NO_er(pc)	; $03
		jmp	CTRL_NO_er(pc)	; $04 �s����i�t�b�g�^�C�v����q�j
		jmp	CTRL_NO_er(pc)	; $05 Portament time
		jmp	CTRL_NO_er(pc)	; $06 data entry ( RPN/NRPN )
		jmp	CTRL_07(pc)	; $07 Main Volume
		jmp	CTRL_NO_er(pc)	; $08 Balance Control
		jmp	CTRL_NO_er(pc)	; $09
		jmp	CTRL_0A(pc)	; $0A Direct Pan
		jmp	CTRL_0B(pc)	; $0B Expression
		jmp	CTRL_NO_er(pc)	; $0C
		jmp	CTRL_NO_er(pc)	; $0D
		jmp	CTRL_NO_er(pc)	; $0E
		jmp	CTRL_NO_er(pc)	; $0F
Bn_10_1F:	jmp	MIDI_Bn_TB2(pc,d2.w)
MIDI_Bn_TB2:	jmp	CTRL_10(pc)	; $10 Mixer change
		jmp	CTRL_11(pc)	; $11 Effect Pan
		jmp	CTRL_NO_er(pc)	; $12 �s����i�ėp����q�R�j
		jmp	CTRL_NO_er(pc)	; $13 �s����i�ėp����q�S�j
		jmp	CTRL_NO_er(pc)	; $14
		jmp	CTRL_NO_er(pc)	; $15
		jmp	CTRL_NO_er(pc)	; $16
		jmp	CTRL_NO_er(pc)	; $17
		jmp	CTRL_NO_er(pc)	; $18
		jmp	CTRL_NO_er(pc)	; $19
		jmp	CTRL_NO_er(pc)	; $1A
		jmp	CTRL_NO_er(pc)	; $1B
		jmp	CTRL_NO_er(pc)	; $1C
		jmp	CTRL_NO_er(pc)	; $1D
		jmp	CTRL_NO_er(pc)	; $1E
		jmp	CTRL_NO_er(pc)	; $1F Sequencer loop mode
Bn_20_2F:	jmp	MIDI_Bn_TB3(pc,d2.w)
MIDI_Bn_TB3:	jmp	CTRL_20(pc)	; $20 Tone BANK change
		jmp	CTRL_NO_er(pc)	; $21
		jmp	CTRL_NO_er(pc)	; $22
		jmp	CTRL_NO_er(pc)	; $23
		jmp	CTRL_NO_er(pc)	; $24
		jmp	CTRL_NO_er(pc)	; $25
		jmp	CTRL_NO_er(pc)	; $26
		jmp	CTRL_NO_er(pc)	; $27
		jmp	CTRL_NO_er(pc)	; $28
		jmp	CTRL_NO_er(pc)	; $29
		jmp	CTRL_NO_er(pc)	; $2A
		jmp	CTRL_NO_er(pc)	; $2B
		jmp	CTRL_NO_er(pc)	; $2C
		jmp	CTRL_NO_er(pc)	; $2D
		jmp	CTRL_NO_er(pc)	; $2E
		jmp	CTRL_NO_er(pc)	; $2F
Bn_60_6F:	jmp	MIDI_Bn_TB7(pc,d2.w)
Bn_30_3F:	jmp	MIDI_Bn_TB4(pc,d2.w)
MIDI_Bn_TB4:
MIDI_Bn_TB7:	jmp	CTRL_NO_er(pc)	; $30	$60 DATA INC
		jmp	CTRL_NO_er(pc)	; $31	$61 DATA DEC
		jmp	CTRL_NO_er(pc)	; $32	$62 NRPN(LSB
		jmp	CTRL_NO_er(pc)	; $33	$63 NRPN(MSB)
		jmp	CTRL_NO_er(pc)	; $34	$64 RPN
		jmp	CTRL_NO_er(pc)	; $35	$65 RPN(MSB)
		jmp	CTRL_NO_er(pc)	; $36	$66
		jmp	CTRL_NO_er(pc)	; $37	$67
		jmp	CTRL_NO_er(pc)	; $38	$68
		jmp	CTRL_NO_er(pc)	; $39	$69
		jmp	CTRL_NO_er(pc)	; $3A	$6A
		jmp	CTRL_NO_er(pc)	; $3B	$6B
		jmp	CTRL_NO_er(pc)	; $3C	$6C
		jmp	CTRL_NO_er(pc)	; $3D	$6D
		jmp	CTRL_NO_er(pc)	; $3E	$6E
		jmp	CTRL_NO_er(pc)	; $3F	$6F
Bn_40_4F:	jmp	MIDI_Bn_TB5(pc,d2.w)
MIDI_Bn_TB5:	jmp	CTRL_40(pc)	; $40 Damper
		jmp	CTRL_NO_er(pc)	; $41 Portament
		jmp	CTRL_NO_er(pc)	; $42 Sosutenute(Code Hold)
		jmp	CTRL_NO_er(pc)	; $43 Soft Pedal
		jmp	CTRL_NO_er(pc)	; $44
		jmp	CTRL_NO_er(pc)	; $45 Hold2(Freeze)
		jmp	CTRL_NO_er(pc)	; $46 ����`
		jmp	CTRL_47(pc)	; $47 Effect return
		jmp	CTRL_NO_er(pc)	; $48
		jmp	CTRL_NO_er(pc)	; $49
		jmp	CTRL_NO_er(pc)	; $4A
		jmp	CTRL_NO_er(pc)	; $4B
		jmp	CTRL_NO_er(pc)	; $4C
		jmp	CTRL_NO_er(pc)	; $4D
		jmp	CTRL_NO_er(pc)	; $4E
		jmp	CTRL_NO_er(pc)	; $4F
Bn_50_5F:	jmp	MIDI_Bn_TB6(pc,d2.w)
MIDI_Bn_TB6:	jmp	CTRL_50(pc)	; $50 Q sound position
		jmp	CTRL_51(pc)	; $51 3D 
		jmp	CTRL_52(pc)	; $52 3D horizontal position
		jmp	CTRL_53(pc)	; $53 3D vertical position
		jmp	CTRL_NO_er(pc)	; $54
		jmp	CTRL_NO_er(pc)	; $55
		jmp	CTRL_NO_er(pc)	; $56
		jmp	CTRL_NO_er(pc)	; $57
		jmp	CTRL_NO_er(pc)	; $58
		jmp	CTRL_NO_er(pc)	; $59
		jmp	CTRL_NO_er(pc)	; $5A
		jmp	CTRL_5B(pc)	; $5B Effect change
		jmp	CTRL_NO_er(pc)	; $5C Tremono depth
		jmp	CTRL_NO_er(pc)	; $5D Chorus depth
		jmp	CTRL_NO_er(pc)	; $5E Seleste depth
		jmp	CTRL_NO_er(pc)	; $5F fazer depth
Bn_70_7F:	jmp	MIDI_Bn_TB8(pc,d2.w)
MIDI_Bn_TB8:	jmp	CTRL_NO_er(pc)	; $70
		jmp	CTRL_NO_er(pc)	; $71
		jmp	CTRL_NO_er(pc)	; $72
		jmp	CTRL_NO_er(pc)	; $73
		jmp	CTRL_NO_er(pc)	; $74
		jmp	CTRL_NO_er(pc)	; $75
		jmp	CTRL_NO_er(pc)	; $76
		jmp	CTRL_NO_er(pc)	; $77
		jmp	CTRL_78(pc)	; $78 ALL SOUND OFF
		jmp	CTRL_NO_er(pc)	; $79 RES ALL CTRLER
		jmp	CTRL_NO_er(pc)	; $7A local control
		jmp	CTRL_7B(pc)	; $7B all note off
		jmp	CTRL_NO_er(pc)	; $7C OMNI OFF
		jmp	CTRL_NO_er(pc)	; $7D omni mode on
		jmp	CTRL_NO_er(pc)	; $7E mono mode
		jmp	CTRL_NO_er(pc)	; $7F Play mode

CTRL_NO_er:	bset.b	#6,Mem_err_bit+2	; �s�� Control change# input
		rts
;************************************************************************
;�y�@�\�z �l�h�c�h�R���g���[���`�F���W[$Bn,$11,$xx]����B		*
;		<<  Effect Pan �`�F���W >>				*
;�y���́zd1.w : EFREG# ( �{���� MIDI ch# : $00�`$1F )			*
;	 d3.b : Parameter					94/07/26*
;************************************************************************
		global	CTRL_11
CTRL_11:
		cmpi.b	#$12,d1
		bcc	CTRL_11_ret

		andi.b	#$7F,d3			; 0(L) �` 7FH(R)
		lsr.b	#2,d3			;
		subi.b	#$10,d3			;
		bcc.s	CTRL_11_2		;
		not.b	d3			;
		addi.b	#$10,d3			; 1FH(L)�`10H(C)00H�`0FH(R)
CTRL_11_2:	;-------------------------------; d3.b = EFPAN ( 0 �` 1FH )
		move.b	SND_OUT_ST(a6),d2	; MONO/STEREO status
		bpl.s	CTRL_11_ST		; jump if not MONO mode
		andi.b	#$E0,d3			;
CTRL_11_ST:	move.w	d1,d2
		addi.w	#mixer_wk_SCSP,d2
		move.b	(a6,d2.w),d0
		andi.b	#$E0,d0
		or.b	d0,d3
		move.b	d3,(a6,d2.w)

		lsl.w	#5,d1			; 20H * MIDI ch#
		move.b	d3,SCSP_EFSDLPN(a5,d1.w)
CTRL_11_ret:	rts
;************************************************************************
;�y�@�\�z �l�h�c�h�R���g���[���`�F���W[$Bn,$46,$xx]����B		*
;		<<  �dffect Return  >>					*
;�y���́zd1.w : EFREG# ( �{���� MIDI ch# : $00�`$1F )			*
;	 d3.b : Parameter					94/07/26*
;************************************************************************
		global	CTRL_47
CTRL_47:
		cmpi.b	#$12,d1
		bcc	CTRL_47_ret

		andi.b	#$70,d3			; 00H(Min) �` 7FH(Max)
		add.b	d3,d3			; 00H(Min) �` E0H(Max)

		move.w	d1,d2
		addi.w	#mixer_wk_SCSP,d2
		move.b	(a6,d2.w),d0
		andi.b	#$1F,d0
		or.b	d0,d3
		move.b	d3,(a6,d2.w)

		lsl.w	#5,d1			; 20H * MIDI ch#
		move.b	d3,SCSP_EFSDLPN(a5,d1.w)
CTRL_47_ret:	rts
;************************************************************************
;�y �@ �\ �z �l�h�c�h�R���g���[���`�F���W[$Bn,$01,$xx]����B		*
;		<< �����n�[�h Modulation_Wheel >>			*
;************************************************************************
		global	CTRL_01
CTRL_01:	move.w	_tmp_kanri(a6),d2	; = 000,200,400,..,E00
		add.b	d3,d3			; $00�`$3F:off / $40�`$7F:on
		andi.b	#$E0,d3			; = xxx0 0000B : [PLFOS]

		moveq	#slot_size-1,d7
		clr.w	d4
		lea	slot_work(a6),a0
CTRL_01_lp:	cmp.b	sl_MIDI(a0,d4.w),d1	; MIDIch# equal ?
		bne.s	CTRL_01_1		; jump if no
		cmp.w	_sl_kanri(a4,d4.w),d2	; �Ǘ��ԍ� equal ?
		bne.s	CTRL_01_1		; jump if no
		movea.l	sl_layer_adr(a0,d4.w),a1
		move.b	LY_SISD(a1),d0		; Mod-Wheel execute ?
		bpl.s	CTRL_01_1		; jump if no
		move.b	LY_LFOS(a1),d5		;
		move.b	d3,d3			;
		bmi.s	CTRL_01_2		; jump if velo = $40 �` $7F
		andi.b	#$18,d5			; LFO sens <-- 0
CTRL_01_2:	lsr.w	#1,d4			; = $00,$20,$40,...,$3E0
		move.b	d5,SCSP_PLFOS(a5,d4.w)	; [PLFOS] & [ALFOWS] & [ALFOS]
		add.w	d4,d4			; = $00,$40,$80,...,$7C0
CTRL_01_1:	addi.w	#slot_wk_unit,d4
		dbra	d7,CTRL_01_lp
		rts
;************************************************************************
;�y �@ �\ �z �l�h�c�h�R���g���[���`�F���W[$Bn,$78,$nn]����B		*
;		<< all sound off >>					*
;	 �Y���Ǘ��ԍ�&�Y���`�����l���̋����I�� key-off & rerease off 	*
; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
;�y �@ �\ �z �l�h�c�h�R���g���[���`�F���W[$Bn,$7B,$nn]����B		*
;		<< all note off >>					*
;	 �Y���Ǘ��ԍ�&�Y���`�����l���̋����I�� key-off�i�����[�X�͗L���j*
; $nn = 40H ---> �Y���Ǘ��ԍ��Ȃ̑S�Ă� MIDI ch �ɑ΂��Ď��s		*
;************************************************************************
		global	CTRL_78,CTRL_7B
CTRL_7B:	moveq	#1,d5			; set Control# = 7BH mode
		bra.s	CTRL_7B_1
CTRL_78:	moveq	#0,d5			; set Control# = 78H mode
CTRL_7B_1:
		rts

		lea	bs_PWKTP,a6		; 68K Prg work top address
		lea	IO_SCSP,a5		; �Œ�
		add.b	d3,d3			; d3 = Parameter

		lea	_KEYHISTB(a6),a0	;
		moveq	#slot_size,d7		; clear 
		sub.w	off_slot_cnt(a6),d7	; = key-on slot��
		beq	CTRL_78_exit		; jump if key-on slot nothing

		subq.w	#1,d7			; loop size
		move.w	HIS_off_pt(a6),d0	;
CTRL_78_lp:	andi.w	#HOPM,d0		; 0,2,4,...,3EH
		move.w	(a0,d0.w),d4		; = $00,$40,$80,$C0,$100,..
		andi.w	#KHMSK,d4		; $7C0

		move.w	_sl_kanri(a4,d4.w),d6	;
		cmp.w	_tmp_kanri(a6),d6	; �����Ǘ��ԍ� equal ?
		bne	CTRL_78_next		; jump if not equal
		move.b	PSPN(a4,d4.w),d6	; PCM Stream playing ?
		bmi	CTRL_78_next		; jump if PCM Stream playing 
		move.b	d3,d3
		bmi.s	CTRL_78_2
		cmp.b	sl_MIDI(a4,d4.w),d1	; MIDI ch# equal ?
		bne	CTRL_78_next		; jump if not equal
CTRL_78_2:
		bclr.b	#flg_non,sl_flag2(a4,d4.w)	; set Note-off�� mode
		move.b	sl_MIDI(a4,d4.w),d6	; d1 = MIDIch#
		andi.w	#$1F,d6			;
		lsl.w	#4,d6			;
		add.w	_tmp_kanri(a6),d6	;
		bclr.b	#k_DMPR,k_MIDI_flg(a6,d6.w)	; set Damper off mode
		bclr.b	#flg_KON,sl_flag2(a4,d4.w)	; set key-off�� mode
		addq.w	#1,off_slot_cnt(a6)	; = key off slot�� + 1
		bclr.b	#PEON_flg,sl_flag1(a4,d4.w)	; PEG off
;@		jsr	PEG_off(pc)		;

;		<<<< Key-off ���s >>>>

		lsr.w	#1,d4			;
		move.b	d5,d5			;
		bne.s	CTRL_78_3		; jump if BnH,7BH,nnH
		move.b	#$FF,SCSP_FH_RR(a5,d4.w)	; rerease off
CTRL_78_3:	move.b	SCSP_KXKB(a5,d4.w),d2	; @0001
		andi.b	#%00000111,d2		; clear KYONEX
		move.b	d2,SCSP_KXKB(a5,d4.w)	; write KYONEX & KYONB
		add.w	d4,d4			;

;		<<<< KEYHISTB �X�V >>>>

		swap	d7		;+	;
		move.w	HIS_off_pt(a6),d5	;
		andi.w	#HOPM,d5		; 0,2,4,...,3EH
		move.w	d0,d6			;
OF_1_lp:	move.w	d6,d7			;
		cmp.w	d6,d5			;
		beq.s	OF_1			;
		move.w	(a0,d7.w),d4		;
		subq.w	#2,d6			;
		andi.w	#HOPM,d6		; 0,2,4,...,3EH
		move.w	(a0,d6.w),(a0,d7.w)	;
		move.w	d4,(a0,d6.w)		;
		bra.s	OF_1_lp			;
OF_1:		addq.w	#2,HIS_off_pt(a6)	;
		swap	d7		;+	;

CTRL_78_next:	addq.w	#2,d0
		dbra	d7,CTRL_78_lp		;

		move.b	(a5),d0			;
		ori.b	#%00010000,d0		; set KYONEX
		move.b	d0,(a5)			; write KYONEX & KYONB
CTRL_78_exit:	rts
;************************************************************************
;�y�@�\�z �l�h�c�h�R���g���[���`�F���W[$Bn,$07,$xx]����B		*
;	  �������̓��ꔭ���Ǘ��ԍ�,����MIDI-ch�ɑ΂��ĉ��ʂ�ύX�B	*
;	  �Ȍ�̓��ꔭ���Ǘ��ԍ�,����MIDI-ch�ɑ΂��Ă͂��̉��ʔ{���Ŋ��Z*
;		Default = 100						*
;�y���́za5   : FH1005 top						*
;	 d1.w : MIDI ch# ( 0 �` 1F )					*
;        d3.b : Parameter 00(-��dB)�`7FH(-0dB)			94/07/26*
;************************************************************************
		global	CTRL_07
CTRL_0B:
CTRL_07:
		move.w	d1,d0
		lsl.w	#4,d0			; = MIDI ch# * 10H
		move.w	_tmp_kanri(a6),d2	;
		add.w	d2,d0			; + �����Ǘ��ԍ��~200H
		andi.w	#$7F,d3			;
		move.b	d3,k_MIDI_VOL(a6,d0.w)	;

		moveq	#slot_size-1,d7		; loop size
		moveq	#0,d4			;
		lea	slot_work(a6),a0	;
CTRL_07_lp:
		; PCM Stream �Đ�����BGM���Đ������ PCM Stream �Đ����ʂ�
		; �e�����󂯂Ă��܂��B
		btst.b	#PCM_flg,PSPN(a0,d4.w)	; 10/21	PCM stream playing ?
		bne.s	CTRL_07_1		; 10/21	jump if yes


		cmp.b	sl_MIDI(a0,d4.w),d1	;
		bne	CTRL_07_1		;
		cmp.w	_sl_kanri(a0,d4.w),d2	;
		bne	CTRL_07_1		;
		btst.b	#flg_FMCR,sl_flag2(a0,d4.w)
		beq.s	CTRL_07_1		;
		;-------------------------------;
		move.w	sl_VL(a0,d4.w),d5	; d5 = 0001 �` 8000(Max) 
		andi.w	#$7F,d3			; d3 = 0000 �` 007F(max)
		addq.w	#1,d3			;      0001 �` 0080(max)
		mulu	d3,d5			; d5 = 0001 �` 400000(Max)
		add.l	d5,d5			;    = 0002 �` 800000(Max)
		add.l	d5,d5			;    = 0004 �`1000000(Max)
		subq.l	#1,d5			;    = 0003 �` FFFFFF(Max)
		swap	d5			; d5.b = 00�`FF(Max)
		not.b	d5			;
		lsr.w	#1,d4			;
		move.b	d5,SCSP_TLVL(a5,d4.w)	;
		add.w	d4,d4			;
		;-------------------------------;
CTRL_07_1:	addi.w	#slot_wk_unit,d4	;
		dbra	d7,CTRL_07_lp
		rts
;************************************************************************
;�y�@�\�z �l�h�c�h�R���g���[���`�F���W[$Bn,$0A,$xx]����B		*
;	  �������̓��ꔭ���Ǘ��ԍ�,����MIDI-ch�ɑ΂��� Pan ��ύX�B	*
;	  �Ȍ�̓��ꔭ���Ǘ��ԍ�,����MIDI-ch�ɑ΂��Ă͂��� Pan pot�ɏ]��*
;	  �i���� Pan �� Direct�����ł���AEffect�����ł͂Ȃ��B�j	*
;�y���́za5   : FH1005 top						*
;	 d1.w : MIDI ch# ( 0 �` 1F )					*
;        d3.b : Parameter 00H(left)�`40H(center)�`7FH(right)	94/07/26*
;************************************************************************
		global	CTRL_0A
CTRL_0A:	andi.b	#$7F,d3			; 0(L) �` 40H(C) �` 7FH(R)
		lsr.b	#2,d3			;	    ��
		subi.b	#$10,d3			;	    ��
		bcc.s	CTRL_0A_0		;	    ��
		not.b	d3			;	    ��
		addi.b	#$10,d3			; 1FH(L)�`10H(C)00H�`0FH(R)
CTRL_0A_0:	;-------------------------------; d3.b = DIPAN ( 0 �` 1FH )
		move.w	k_kanri_ofst(a6),d0	;
		bset.b	#7,k_MIDI_PAN(a6,d0.w)	; set Layer PAN off
		move.b	k_MIDI_PAN(a6,d0.w),d4	;
		btst	#6,d4			; SEQ PAN on ?
		beq.s	CTRL_0A_2		; jump if off
		btst.b	#7,SND_OUT_ST(a6)	; MONO/STEREO status
		bne.s	CTRL_0A_mono		; jump if MONO mode
		rts				; return if STEREO mode
		; <<< SEQ PAN off >>>
CTRL_0A_2:	btst.b	#7,SND_OUT_ST(a6)	; MONO/STEREO status
		bne.s	CTRL_0A_mono		; jump if MONO mode
		; <<< SEQ PAN off & STEREO mode >>>
		ori.b	#$80,d3			; set layer PAN off
		move.b	d3,k_MIDI_PAN(a6,d0.w)	;
		andi.b	#$1F,d3			;
		bra.s	CTRL_0A_ST		; jump STEREO mode
		;-------------------------------;
CTRL_0A_mono:	clr.w	d3			; --> Center
		;-------------------------------;
		; �������X���b�g�̓��� MIDI ch	;
		; �̑S�Ăɑ΂��� PAN ���X�V	;
		;-------------------------------;
CTRL_0A_ST:	moveq	#slot_size-1,d7		; loop size
		moveq	#0,d4			;
		lea	slot_work(a6),a0	;
CTRL_0A_lp:	cmp.b	sl_MIDI(a0,d4.w),d1	;
		bne	CTRL_0A_1		; jump if not equal MIDI ch#
		cmp.w	_sl_kanri(a0,d4.w),d0	;
		bne	CTRL_0A_1		; jump if not equal �Ǘ��ԍ�
		lsr.w	#1,d4			;
		move.b	SCSP_DISDLPN(a5,d4.w),d5	; = xxxa aaaa B
		andi.b	#$E0,d5			; = xxx0 0000
		or.b	d3,d5			;
		move.b	d5,SCSP_DISDLPN(a5,d4.w)	;
		add.w	d4,d4			;
CTRL_0A_1:	addi.w	#slot_wk_unit,d4	;
		dbra	d7,CTRL_0A_lp
		rts
;************************************************************************
;�y�@�\�z �l�h�c�h�R���g���[���`�F���W[$Bn,$40,$xx]����B�_���p�[	*
;�y���́za5   : FH1005 top						*
;	 d1.w : MIDI ch# ( 0 �` 1F )					*
;        d3.b : Parameter ( 00 �` 7FH )				94/07/26*
;************************************************************************
		global	CTRL_40
CTRL_40:	; Hold1(dumper)

		move.w	d1,d7
		lsl.w	#4,d7			; = 10H * MIDI ch#
		move.w	_tmp_kanri(a6),d6	; = �����Ǘ��ԍ��~200H
		add.w	d6,d7			; + �����Ǘ��ԍ��~200H
		move.b	k_MIDI_flg(a6,d7.w),d0	; bit 6 = Damper on/off
		andi.b	#%01000000,d3		; Damper on/off
		bne.s	Damper_on		;

		global	Damper_off
Damper_off:	andi.b	#%10111111,d0		; set Damper off mode
		move.b	d0,k_MIDI_flg(a6,d7.w)	; = MIDI status �X�V

		moveq	#slot_size,d7		;
		sub.w	off_slot_cnt(a6),d7	; = key-on�� slot��
		beq.s	ret_11			;
		subq.w	#1,d7			; loop size
		lea	_KEYHISTB(a6),a0	;
		lea	slot_work(a6),a4	;
		move.w	HIS_off_pt(a6),d0	;
DMP_off_lp:	andi.w	#HOPM,d0		; 0,2,4,...,3EH
		move.w	(a0,d0.w),d4		;
		andi.w	#KHMSK,d4		; $7C0
		cmp.b	sl_MIDI(a4,d4.w),d1	; MIDI ch# equal ?
		bne.s	DMP_off_1
		cmp.w	_sl_kanri(a4,d4.w),d6	; �����Ǘ��ԍ� equal ?
		bne.s	DMP_off_1

		btst.b	#flg_KON,sl_flag2(a4,d4.w)	; key-on(����)�� ?
		beq.s	DMP_off_1			; jump if key-off��
		btst.b	#flg_non,sl_flag2(a4,d4.w)	; Note-on��?
		bne.s	DMP_off_1			; jump if Note-on��
		; Note-off ���͂ɑ΂��� Damper�ɂ�� Key-off �����s
;@		move.b	PSPN(a4,d4.w),d2	; PCM_flg on ?
;@		bmi.s	DMP_off_1		; jump if PCM Stream play ��
		move.b	sl_note(a4,d4.w),d2	; ready MIDI Note#
		move.b	sl_velo(a4,d4.w),d3	; ready MIDI velo#
		movem.l	d0-d7/a0-a4,-(SP)	; reg. push
		jsr	Note_off(pc)
		movem.l	(SP)+,d0-d7/a0-a4	; reg. pop
DMP_off_1:	addq.w	#2,d0
		dbra	d7,DMP_off_lp
ret_11:		rts

;		global	Damper_on
Damper_on:	ori.b	#%01000000,d0		; set Damper on mode
		move.b	d0,k_MIDI_flg(a6,d7.w)	; = MIDI status �X�V
		rts

;************************************************************************
;�y �@ �\ �z �l�h�c�h�R���g���[���`�F���W[$Bn,$5B,$xx]����B		*
;	     �c�r�o �licro �orogram ���X�V����B			*
;�y �� �� �za5   : FH1005 top						*
;	    d1.w : MIDI ch# ( 0 �` 1F )					*
;           d3.b : Parameter ( 00 �` 7FH )				*
;�y �o �� �znothing							*
;�ydestroy�za0/a1/d0/d2.b/d7						*
;�y hold  �za5/a6							*
;************************************************************************
		global	CTRL_5B
CTRL_5B_er0:	bset.b	#1,Mem_err_bit+2	; cannot find DSP-Prg-ID
		rts
CTRL_5B_er1:	bset.b	#2,Mem_err_bit+2	; not DSP-Prg down load
		rts
CTRL_5B_er2:	bset.b	#3,Mem_err_bit+2	; cannot find DSP-RAM-ID
		rts
CTRL_5B_er3:	bset.b	#4,Mem_err_bit+2	; DSP access area unit is 2000H
		rts
CTRL_5B_er4:	bset.b	#5,Mem_err_bit+2	; jump if Memory �s��
		rts
er_03:		bset.b	#6,Mem_err_bit+1	; Effect change No. > 0FH
		rts
CTRL_5B:
		cmpi.b	#$10,d3
		bcc.s	er_03			; jump if MIDI parameter > 0FH
		move.b	d3,d1
		ori.b	#DSP_PRG_ID*$10,d1
		moveq	#DSP_RAM_ID*$10,d3
		;-------------------------------;
		;    �Y���c�r�o�ԍ��̌���	;
		;-------------------------------;
		lea	bs_AMAPC,a0		; area map current work top
		moveq	#256/8-1,d7		; loop size
CTRL_5B_lp:	move.b	(a0),d0			; 
		bmi	CTRL_5B_er0		; jump if not find DSP-Prg-ID
		cmp.b	d0,d1			;
		beq.s	CTRL_5B_4		;
		lea	8(a0),a0
		dbra	d7,CTRL_5B_lp
		bra	CTRL_5B_er0		; jump if not find DSP-Prg-ID
CTRL_5B_4:	bclr.b	#1,Mem_err_bit+2	;
		move.l	(a0)+,d0		;
		andi.l	#$0FFFFF,d0		; = DSP Micro Prg top addr
		move.l	(a0),d2			;
		bpl	CTRL_5B_er1		; jump if not data ready
		bclr.b	#2,Mem_err_bit+2	;
		andi.l	#$0FFFFF,d2		; d2 = size
		movea.l	d0,a0			; a0 = DSP Micro Prg top addr
		move.l	d0,DSP_PRG_top(a6)	; save
		;-------------------------------;
		;  �Y���c�r�o-�q�`�l�ԍ��̌���	;
		;-------------------------------;
		lea	bs_AMAPC,a1		; area map current work top
		moveq	#256/8-1,d7		; loop size
CTRL_5B_4lp:	move.b	(a1),d0			; 
		bmi	CTRL_5B_er2		; jump if not find DSP-RAM-ID
		bclr.b	#3,Mem_err_bit+2	;
		andi.b	#$70,d0
		cmp.b	d0,d3
		beq.s	CTRL_5B_7
		lea	8(a1),a1
		dbra	d7,CTRL_5B_4lp
		bra	CTRL_5B_er2		; jump if not find DSP-RAM-ID
CTRL_5B_7:	move.l	(a1)+,d5		;
		move.w	d5,d3
		andi.w	#$1FFF,d3
		bne	CTRL_5B_er3		; DSP access area unit is 2000H
		bclr.b	#4,Mem_err_bit+2	; DSP access area unit is 2000H
		andi.l	#$0FE000,d5		; d5 = DSP access area top
		move.l	(a1),d3			;
		andi.l	#$0FFFFF,d3		; d3 = size

	cmpi.l	#$20040+1,d3
	bcc	CTRL_5B_er5
	bclr.b	#7,Mem_err_bit+1	; jump if R/W size > $20040
;@		andi.l	#$01FFFF,d3		; d3 = size
		;-------------------------------;
		;  DSP access Memory size check	
		; raedy a0 = DSP Micro Prg top addr
		; 	d2 = size		
		; 	d5 = DSP access area top	
		; 	d3 = size		
		;-------------------------------;
		move.l	d5,DSP_RW_top(a6)	;
		move.l	d3,DSP_RW_sz(a6)	;

		move.b	DL_NEL(a0),DFL_ELMNT_NO(a6)
		move.l	#$4000,d1		; RBLen unit
		move.b	DL_RBL(a0),d0		; = RBLEN 0=$04000
						;	  1=$08000
						;	  2=$10000
		andi.w	#3,d0			;	  3=$20000
		beq.s	CTRL_5B_2		; jump if RBLen = 0
		lsl.l	d0,d1			; d1 = 2exp[RBLen] * $4000
CTRL_5B_2:	addi.l	#$40,d1			; + 20H*2(�u���l����area)
		moveq	#0,d4			
		move.b	DL_NCT(a0),d4		; = COEF table��
		beq.s	CTRL_5B_3		; jump if COEF table nothing
		mulu	#$A00,d4
CTRL_5B_3:	add.l	d4,d1			; + COEF table size
		cmp.l	d1,d3
		bcs	CTRL_5B_er4		; jump if Memory �s��
		bclr.b	#5,Mem_err_bit+2	; jump if Memory �s��

		move.b	#8,EFCT_CHG_CNT(a6)	; set Effect change exe mode
		rts
CTRL_5B_er5:	bset.b	#7,Mem_err_bit+1	; jump if R/W size > $20040
		rts
;************************************************************************
;     �oitch �aend							*
;�y���́z a6   = CPU work top addr.					*
;	  d1.w = MIDI 1st. byte	0 �` 1FH ( $En )			*
;	  d2.b = MIDI 2nd. byte	0 �` 7FH ( LSB )			*
;	  d3.b = MIDI 3rd. byte	0 �` 7FH ( MSB )		94/07/26*
;************************************************************************
		global	MIDI_En
MIDI_En:	andi.w	#$7F,d2			; LSB
		andi.w	#$7F,d3			; MSB
		lsl.w	#7,d3
		or.w	d3,d2			; d2 = 00xx xxxx xxxx xxxx
	
		subi.w	#$2000,d2		; d2 = -$2000�`0�`$1FFF

		move.w	d1,d4			;
		lsl.w	#4,d4			; = 10H * MIDI ch#
		add.w	_tmp_kanri(a6),d4	; + �����Ǘ��ԍ��~200H
		move.b	k_PROG_no(a6,d4.w),d0	;
		movea.l	k_BANK_adr(a6,d4.w),a2	; = desti. SCSPBIN top address
		add.w	d0,d0			;
		move.w	BIN_VOICE(a2,d0.w),d0	; voice#xx offset addr
		move.b	V_PM_BR(a2,d0.w),d0	; low 4bit = Pitch Bend range
		andi.w	#$0f,d0			; = 0 �` D
		add.w	d0,d0			; = 0,2,4,6,8,...1AH
		add.w	d0,d0			; = 0,4,8,10H,...34H
		jmp	En_JPTB(pc,d0.w)	;
En_JPTB:	clr.w	d2			; �} 0
		bra.s	EN_0x
		asr.w	#5,d2			; �} 100�Z���g
		bra.s	EN_0x
		asr.w	#4,d2			; �} 200�Z���g
		bra.s	EN_0x
		asr.w	#4,d2			; �} 300�Z���g
		bra.s	EN_00
		asr.w	#3,d2			; �} 400�Z���g
		bra.s	EN_0x
		asr.w	#3,d2			; �} 500�Z���g
		bra.s	EN_01
		asr.w	#3,d2			; �} 600�Z���g
		bra.s	EN_00
		asr.w	#3,d2			; �} 700�Z���g 400+200+100
		bra.s	EN_04
		asr.w	#2,d2			; �} 800�Z���g
		bra.s	EN_0x
		asr.w	#2,d2			; �} 900�Z���g 800+100
		bra.s	EN_05
		asr.w	#2,d2			; �} 1000�Z���g 800+200
		bra.s	EN_01
		asr.w	#2,d2			; �} 1100�Z���g 800+200+100
		bra.s	EN_06
		asr.w	#2,d2			; �} 1200�Z���g 800+400
		bra.s	EN_00
		asr.w	#1,d2			; �} 2400�Z���g 1000+800
;@		bra.s	EN_00
EN_00:		move.w	d2,d3
		moveq	#1,d4
		bra.s	EN_07
EN_06:		move.w	d2,d3
		asr.w	#2,d3
		bra.s	EN_02
EN_05:		move.w	d2,d3
		moveq	#3,d4
		bra.s	EN_07
EN_01:		move.w	d2,d3
		moveq	#2,d4
		bra.s	EN_07
EN_04:		move.w	d2,d3
		asr.w	#1,d3
EN_02:		add.w	d3,d2
		moveq	#1,d4
EN_07:		asr.w	d4,d3
		add.w	d3,d2
EN_0x:
;
;		d2.w �� -$0C00 �` +$0BFF ( Pitch Bend data )
;
		move.w	_tmp_kanri(a6),d3	; = �����Ǘ��ԍ��~200H
		lsl.w	#4,d1			; = MIDI ch# * 10H
		add.w	d1,d3			;
		move.w	d2,k_PBend_BF(a6,d3.w)	; save
ret_80:		rts

;===============================================================;
;	MIDI CONTROL CHANGE [$Bn,$50,$xx]	95/02/03	;
;		Q sound position set				;
;	input	d1.w : EFREG# ( �{���� MIDI ch# : $00�`$1F )	;
;		d3.b : Parameter				;
;				edit by Y.Kashima		;
;===============================================================;
		global	CTRL_50
		extern	xseq_qsound
CTRL_50:
		move.b	d1,d0		; channel
		move.b	d3,d1		; position
		bsr	xseq_qsound	;
		rts			;
;===============================================================;
;	MIDI CONTROL CHANGE [$Bn,$5(1-3),$xx]	95/04/11	;
;		3D sound set					;
;	input	d3.b : Parameter				;
;				edit by Y.Kashima		;
;===============================================================;
		global	CTRL_51,CTRL_52,CTRL_53
		extern	xseq_YMH3Dw
CTRL_51:
		lea.l	Y3D_buf,a0	;
		move.b	d3,d0		; d0=distance
		move.b	d0,(a0)		;
		move.b	1(a0),d1	; d1=azimuth
		move.b	2(a0),d2	; d2=elevation
		bsr	xseq_YMH3Dw	;
		rts			;
CTRL_52:
		lea.l	Y3D_buf,a0	;
		move.b	d3,d1		; d1=azimuth
		move.b	d1,1(a0)	;
		move.b	(a0),d0		; d0=distance
		move.b	2(a0),d2	; d2=elevation
		bsr	xseq_YMH3Dw	;
		rts			;
CTRL_53:
		lea.l	Y3D_buf,a0	;
		move.b	d3,d2		;
		move.b	d2,2(a0)	; d2=elevation
		move.b	(a0),d0		; d0=distance
		move.b	1(a0),d1	; d1=azimuth
		bsr	xseq_YMH3Dw	;
		rts			;
Y3D_buf:
		db	0
		db	0
		db	0

	end


