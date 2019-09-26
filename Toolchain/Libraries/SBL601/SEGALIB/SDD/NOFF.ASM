; 93'10�`	(C)SEGA sound Room   AM2 Yamamoto
;
;	SCSP Contol Program      
;

	include	SCSP.LIB

	global		Note_off

	external	get_EXPTBS,get_EXPTB2,get_LEVEL
	external	Note_on
;@@	external	er_26

;��������������������������������������������������������������������������
;��     MIDI note off							 ��
;�� Key on ���� ����MIDI# �� Note# �̑S�Ă� slot �ɑ΂��� Key off �����s ��
;���y ��  �� �z a6 = CPU work top addr.					 ��
;��		d1.w = MIDI 1st. byte	0 �` 1FH ( MIDI ch# )		 ��
;��		d2.b = MIDI 2nd. byte	0 �` 7FH ( note )		 ��
;��		d3.b = MIDI 3rd. byte	0 �` 7FH ( velocity )		 ��
;���y �o  �� �z �Ȃ�							 ��
;���y���W�X�^�z destroy : d0,d1,d4,d5,d6				 ��
;��������������������������������������������������������������������������
Note_off:	move.w	k_kanri_ofst(a6),d5	;
		btst.b	#k_mono,k_MIDI_flg(a6,d5.w)	; mono mode ?
		beq	NOFF_1
;		����������������������������������������������������������
;		��    		���m���[�h�F�m�[�g�I�t	  		��
;		����������������������������������������������������������
NOFF_M1:	move.b	k_MONO_NT0(a6,d5.w),d6	; Note# exist ?
		beq.s	NOFF_er_ret
		cmp.b	d2,d6			; Note# equal ?
		beq.s	NOFF_M2
		cmp.b	k_MONO_NT1(a6,d5.w),d2	; Note# equal ?
		beq.s	NOFF_M3
		cmp.b	k_MONO_NT2(a2,d5.w),d2	; Note# equal ?
		beq.s	NOFF_M4
		bra.s	NOFF_er_ret
NOFF_M3:	move.b	k_MONO_NT2(a6,d5.w),k_MONO_NT1(a6,d5.w)
NOFF_M4:	move.b	#0,k_MONO_NT2(a6,d5.w)	; clear
NOFF_er_ret:
;@		bset.b	#7,Mem_err_bit+2	; not find Key-off slot
;@		rts
		bset.b	#0,Mem_DRVERR_FLG+0	; key-off ���ׂ� key-on��slot
		rts				; �����݂��Ȃ�

NOFF_M2:
		move.b	k_MONO_NT1(a6,d5.w),d6	;
		beq.s	NOFF_M6			; jump if [note1] nothing
		move.b	d6,k_MONO_NT0(a6,d5.w)	;
		move.b	d6,d2			; set new Note on data
		move.b	k_MONO_VOL(a6,d5.w),d3	;
		andi.b	#$7f,d3			; set new velocity data
		move.b	k_MONO_NT2(a6,d5.w),k_MONO_NT0(a6,d5.w)
		move.b	#0,k_MONO_NT1(a6,d5.w)	; clear
		move.b	#0,k_MONO_NT2(a6,d5.w)	; clear
		jmp	Note_on(pc)
NOFF_M6:
;@@		btst.b	#k_DMPR,k_MIDI_flg(a6,d5.w)	; ���� MIDI ch 
;@@		beq.s	NOFF_M5				;   Damper on/off ?
;@@		;-------------------------------;
;@@		; set flg_DMPR �F�_���p�[�ɂ��jey-off�����s�t���O�Z�b�g
;@@
;@@		moveq	#32,d7			;
;@@		sub.w	off_slot_cnt(a6),d7	; = key-on�� slot��
;@@		beq.s	ret_12			;
;@@		subq.w	#1,d7			; loop size
;@@		lea	_KEYHISTB(a6),a0		;
;@@		lea	slot_work(a6),a4	;
;@@		move.w	HIS_off_pt(a6),d0	;
;@@NOFF_M7_lp:	andi.w	#HOPM,d0		; 0,2,4,...,3EH
;@@		move.w	(a0,d0.w),d4		;
;@@		andi.w	#KHMSK,d4		; $7C0
;@@		cmp.b	sl_MIDI(a4,d4.w),d1	; MIDI ch# equal ?
;@@		bne.s	NOFF_M7			;
;@@		cmp.b	sl_note(a4,d4.w),d2	; MIDI Note# equal ?
;@@		bne.s	NOFF_M7			;
;@@
;@@		move.w	_sl_kanri(a4,d4.w),d6	;
;@@		cmp.w	_tmp_kanri(a6),d6	; �����Ǘ��ԍ� equal ?
;@@		bne.s	NOFF_M7			;
;@@
;@@		bset.b	#flg_DMPR,sl_flag2(a4,d4.w)
;@@NOFF_M7:	addq.w	#2,d0
;@@		dbra	d7,NOFF_M7_lp
;@@ret_12:	
;@@		jmp	er_26(pc)
;@@		rts
		;-------------------------------;
NOFF_M5:	move.b	d6,k_MONO_NT0(a6,d5.w)	;

; ������������������������������������������������������������������������
; ��			�|�����[�h�F�m�[�g�I�t	  			��
; �� �y ���� �z   d1,d2,			     ,a5,a6		��
; �� �y �j�� �zd0,      d3,d4,d5,d6,d7,a0,a1,a2,a3,a4			��
; �� �y �ێ� �z   d1,d2,			     ,a5,a6		��
; �� �y���g�p�z				        a3			��
; ������������������������������������������������������������������������
NOFF_1:
		lea	_KEYHISTB(a6),a0		;
		lea	slot_work(a6),a4	;
		moveq	#slot_size,d7		; clear 
		sub.w	off_slot_cnt(a6),d7	; = key-on slot��
		bne.s	OF_0			; jump if key-on slot exist
OF_exit:	jmp	er_20(pc)
		;-------------------------------------------------------;
		; key-on ���� ���� MIDI ch & Note# slot �ւ� key-off ���s;
		;�y���́z d7.w : key-on slot ��
		;-------------------------------------------------------;
		global	OF_0
OF_0:		subq.w	#1,d7			; loop size
		move.w	HIS_off_pt(a6),d0	;
OF_lp_1:	andi.w	#HOPM,d0		; 0,2,4,...,3EH
		move.w	(a0,d0.w),d4		;
		andi.w	#KHMSK,d4		; $7C0

		move.b	PSPN(a4,d4.w),d6	; PCM Stream playing ?
		bmi	OF_2

		cmp.b	sl_MIDI(a4,d4.w),d1	; MIDI ch# equal ?
		bne	OF_2			; jump if not equal
		cmp.b	sl_note(a4,d4.w),d2	; MIDI note# equal ?
		bne	OF_2			; jump if not equal
		move.w	_sl_kanri(a4,d4.w),d6	;
		cmp.w	_tmp_kanri(a6),d6	; �����Ǘ��ԍ� equal ?
		bne	OF_2			; jump if not equal

		bclr.b	#flg_non,sl_flag2(a4,d4.w)	; set Note-off�� mode
		move.w	k_kanri_ofst(a6),d6		;
		btst.b	#k_DMPR,k_MIDI_flg(a6,d6.w)	; Damper on/off ?
		bne	OF_2	; = no key-off		; jp if Damper on mode

		bclr.b	#flg_KON,sl_flag2(a4,d4.w)	; set key-off�� mode
		addq.w	#1,off_slot_cnt(a6)	; = key off slot�� + 1

		btst.b	#PEON_flg,sl_flag1(a4,d4.w)
		beq.s	PEG_off_1		; jump if if PEG off
		jsr	PEG_off(pc)		;

;		<<<< Key-off ���s >>>>

PEG_off_1:	lsr.w	#1,d4			; = $00,$20,.....,$3E0
		move.b	SCSP_KXKB(a5,d4.w),d5	;
		andi.b	#%00000111,d5		; clear KYONB
		ori.b	#%00010000,d5		; set   KYONEX
		move.b	d5,SCSP_KXKB(a5,d4.w)	;
		add.w	d4,d4			; = $00,$40,.....,$7C0

;		<<<< KEYHISTB �X�V >>>>

		swap	d7			;
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
		swap	d7			;

;@		subq.w	#1,DVA_lyr_cnt(a6)	; 1�`32
;@		beq.s	OF_exit
		;-------------------------------;
OF_2:		addq.w	#2,d0
		dbra	d7,OF_lp_1
;@OF_exit:
;		����������������������������������������������������������
;		��  �c�r�o �b�o�t�쓮�c-�eilter	�̌���			��
; 		���y���́z a6						��
; 		��	   d1.w : ���� MIDI ch#				��
; 		���y�j��z a1/a2					��
; 		��	   d0.w/d6.w					��
;		����������������������������������������������������������
		global	DFLTR_off
DFLTR_off:	lea	_DFL_ELMNT_wk(a6),a2
;@		movea.l	Mem_Snd_tool_pt,a1	; = sound tool I/F work top
		clr.w	d6			;
		move.b	DFL_ELMNT_NO(a6),d6	; �G�������g��  �F�w
;@		move.b	Mem_ELMNT_no(a1),d6	; = �G�������g��  �F�w
		beq.s	DFLTR_off_1		;
		movea.l	DFL_ELMNT_addr(a6),a1	; = Element data top
;@		movea.l	Mem_ELMNT_addr(a1),a1	; = Element data top
		subq.w	#1,d6			; loop size
		andi.w	#$1F,d6			;
DFLTR_off_3:	cmp.b	(a1),d1			; Element MIDI ch = MIDI# ?
		bne.s	DFLTR_off_2
		move.b	1(a1),d0		;
		cmpi.b	#8,d0			; 1000 ?
		beq.s	DFLTR_off_LFO		; jump if LFO
		bcs.s	DFLTR_off_2		; jump if LFO/EG off
		;-------------------------------;
DFLTR_off_EG:	move.b	#6,DFL_EG_seg(a2)	; ���� set seg#6
		move.w	DFL_add_wk(a2),d4	; = $0000�`$FF00
		move.b	15(a1),d0		; = [RL]
		lsl.w	#8,d0			; = -$8000�`$7F00
		addi.w	#$8000,d0		; = $0000�`$FF00
		move.w	d0,DFL_level(a2)	; set ���B���x��
		cmp.w	d0,d4
		bcc.s	DFL_off_EG_1		; jump if [RL] < add_wk
		move.b	14(a1),d0		; = [RR]
		bpl.s	DFL_off_EG_2
		bra.s	DFL_off_EG_3
DFL_off_EG_1:	move.b	14(a1),d0		; = [RR]
		bmi.s	DFL_off_EG_2
DFL_off_EG_3:	neg.b	d0
DFL_off_EG_2:	jsr	get_EXPTBS(pc)		; return d0.w : -$3F01�`+$3F01
		move.w	d0,DFL_add_bs(a2)	; set rate
		bra.s	DFLTR_off_2
		;-------------------------------;
DFLTR_off_LFO:	move.w	#0,DFL_add_bs(a2)	; LFO only ( don't destroy EG )
;@		bclr.b	#7,DFL_wk_MSFC(a2)	; Note off mode
;@		bra.s	DFLTR_off_2
		;-------------------------------;
DFLTR_off_2:	lea	$18(a1),a1
		lea	$10(a2),a2
		dbra	d6,DFLTR_off_3
DFLTR_off_1:
;@@@@@@@@@@@@@@@		bclr.b	#7,bug_flag(a6)
;@@@@@@@@@@@@@@@		beq.s	bug_00
;@@@@@@@@@@@@@@@		jmp	er_26(pc)
;@@@@@@@@@@@@@@@	bug_00:
		rts

;		����������������������������������������������������������
;		��  �o�d�f�X�V						��
; 		���y����&�ێ��z	a4   : slot work top			��
;		��		d4.w : slot work offset			��
;		��		d0 low : History offset point		��
; 		���y�j��z	a1   : 					��
;		��		d0 High	, d3.l , d5.l , d6.l		��
;		����������������������������������������������������������
		global	PEG_off
PEG_off:	swap	d0
		move.b	#6,PEG_SEG(a4,d4.w)	;
		movea.l	PEG_addr(a4,d4.w),a1	;
		addq	#8,a1			;
		move.b	(a1)+,d0		; = [RR]
		beq.s	PEG_off_3
		;-------------------------------;
		jsr	get_EXPTB2(pc)		; return d0.w:-$3F01�`+$3F01
		move.w	d0,d6
		swap	d6
		clr.w	d6
		asr.l	#4,d6			; for 4msec

		move.b	(a1),d0			; = [RL]
		jsr	get_LEVEL(pc)		; return d0.w:-$5FFF�`+$5FFF
		move.w	d0,d5			;
		swap	d5			;
		clr.w	d5			;
		move.l	d5,PEG_level(a4,d4.w)	; set ���B���x��
		addi.l	#$80000000,d5		;
		move.l	PEG_cent(a4,d4.w),d3	;
		addi.l	#$80000000,d3		;
		cmp.l	d3,d5			;
		beq.s	PEG_off_3		; jump if _cent = _level
		bcc.s	PEG_off_2		; jump if _cent < _level
		move.l	d6,d6			;
		bmi.s	PEG_off_exit		;
		bra.s	PEG_off_4
PEG_off_2:	move.l	d6,d6			;
		bpl.s	PEG_off_exit		;
PEG_off_4:	neg.l	d6
PEG_off_exit:	move.l	d6,PEG_RATE(a4,d4.w)	; set �X���^4msec
		swap	d0
		rts

PEG_off_3:	addq.b	#1,PEG_SEG(a4,d4.w)	;
		swap	d0
		rts


er_20:		bset.b	#7,Mem_DRVERR_FLG+1	; key-off ���ׂ� key-on��slot
		rts				; �����݂��Ȃ�


