;=======================================================================;
;	DEC2.ASM  ( M6 data decoder )					;
;	Ver. 2.20 / 1994.10.26						;
;						by  �V�c���M		;
;=======================================================================;

	globals on

;	area_1 ���ł̊e�ϐ��� Offset

ofst_nxt_adr			equ	0
ofst_lp_start			equ	4
ofst_ret_of_ref			equ	8
ofst_top_of_track		equ	12
ofst_nxt_adr_in_tmp		equ	16
ofst_1st_tmp_in_lp		equ	20
ofst_rest_of_referenced		equ	24
ofst_dummy_1			equ	25
ofst_dummy_2			equ	26
ofst_dummy_3			equ	27
ofst_dummy_4			equ	28
ofst_dummy_5			equ	29
ofst_dummy_6			equ	30
ofst_dummy_7			equ	31

mc_add_to_delta_time:	macro
			moveq.l	#0,d7
			move.b	(a0)+,d7
			add.l	d7,d1
			endm

sq_decoder:	lsl.w	#5,d1			;*32
		lea.l	area_1(pc,d1),a1	; culc work adr of song in 
		add.w	d0,d0			;			area_1
		bne	jump_cont		;jump - 10 clk , through = 8 clk
tp_0:		move.l	(a1),a0
output_event:	moveq.l	#0,d0			;processing_status
		moveq.l	#0,d1			;delta time
		moveq.l	#0,d2			;channel number
		moveq.l	#0,d3			;MIDI CMD
		moveq.l	#0,d4			;MIDI Data 1
		moveq.l	#0,d5			;MIDI Data 2
		moveq.l	#0,d6			;gate time
get_1_event:	move.b	(a0)+,d2		;�C�x���g���o��
		cmpi.b	#7fh,d2			;�C�x���g <= 7fh ?
		bhi	not_note

note:		;�C�x���g <= 7f �̎��̃��[�`��
		move.b	(a0)+,d4		;note
		move.b	(a0)+,d5		;velo
		move.l	d2,d7
		add.w	d7,d7			;delta �ŏ�� bit �������o��
		add.w	d7,d7
		move.w	d7,d3			;prepare gate
						;d3 �� ext cmd �� .w �Ŏg����
						;����̂ŁA�K�� .w , .l �Ŏg��
						;�����Ȃ��ƃS�~������
		andi.w	#FFh,d7
		add.w	d7,d7

		move.b	(a0)+,d3
		add.l	d3,d6			;d6 = gate

		move.b	(a0)+,d7		;d1 = delta
		add.l	d7,d1

		andi.b	#00011111b,d2		;make channel no.
		move.w	#90h,d3			;MIDI CMD

reference_check:
		cmp	ofst_rest_of_referenced(a1),d0
						;d0 = proseccing status , = 0
		bne.w	reference_check_cont	;jump when in referencing
		move.l	a0,(a1)
		move.l	d0,d7
		move.b	d2,d0			;d0 ����
		lsl.w	#8,d0			;1 byte to left
		add.l	d7,d0
		move.l	d6,d2			;d2 ����
		swap	d4
		add.b	d5,d4
		rts
area_1:
		ds.b	32*8

reference_check_cont:
		subq.b	#1,ofst_rest_of_referenced(a1)
		bne.b	store_nxt_read_point
		move.l	ofst_ret_of_ref(a1),(a1)
		move.l	d0,d7
		move.b	d2,d0			;d0 ����
		lsl.w	#8,d0			;1 byte to left
		add.l	d7,d0
		move.l	d6,d2			;d2 ����
		swap	d4
		add.b	d5,d4
		rts
store_nxt_read_point:
		move.l	a0,(a1)
end_of_decode:
		move.l	d0,d7
		move.b	d2,d0			;d0 ����
		lsl.w	#8,d0			;1 byte to left
		add.l	d7,d0
		move.l	d6,d2			;d2 ����
		swap	d4
		add.b	d5,d4
		rts
not_note:
		move.l	d2,d7
		subi.w	#80h,d7
		add.w	d7,d7
		move.w	event_jump_table(pc,d7),d3
		jmp	event_jump_table(pc,d3)

event_jump_table:
	dc.w	other_event-event_jump_table		;#80h(���̑�)
	dc.w	reference-event_jump_table		;#81h
	dc.w	loop-event_jump_table			;#82h
	dc.w	end_of_track-event_jump_table		;#83h
	dc.w	other_event-event_jump_table		;#84h(���̑�)
	dc.w	other_event-event_jump_table		;#85h(���̑�)
	dc.w	other_event-event_jump_table		;#86h(���̑�)
	dc.w	other_event-event_jump_table		;#87h(���̑�)
	dc.w	ext_gate_200h-event_jump_table		;#88h
	dc.w	ext_gate_800h-event_jump_table		;#89h
	dc.w	ext_gate_1000h-event_jump_table		;#8ah
	dc.w	ext_gate_2000h-event_jump_table		;#8bh
	dc.w	ext_delta_100h-event_jump_table		;#8ch
	dc.w	ext_delta_200h-event_jump_table		;#8dh
	dc.w	ext_delta_800h-event_jump_table		;#8eh
	dc.w	ext_delta_1000h-event_jump_table	;#8fh
	dc.w	other_event-event_jump_table		;#90h(���̑�)
	dc.w	other_event-event_jump_table		;#91h(���̑�)
	dc.w	other_event-event_jump_table		;#92h(���̑�)
	dc.w	other_event-event_jump_table		;#93h(���̑�)
	dc.w	other_event-event_jump_table		;#94h(���̑�)
	dc.w	other_event-event_jump_table		;#95h(���̑�)
	dc.w	other_event-event_jump_table		;#96h(���̑�)
	dc.w	other_event-event_jump_table		;#97h(���̑�)
	dc.w	other_event-event_jump_table		;#98h(���̑�)
	dc.w	other_event-event_jump_table		;#99h(���̑�)
	dc.w	other_event-event_jump_table		;#9ah(���̑�)
	dc.w	other_event-event_jump_table		;#9bh(���̑�)
	dc.w	other_event-event_jump_table		;#9ch(���̑�)
	dc.w	other_event-event_jump_table		;#9dh(���̑�)
	dc.w	other_event-event_jump_table		;#9eh(���̑�)
	dc.w	other_event-event_jump_table		;#9fh(���̑�)
	dc.w	poly_key_pressure-event_jump_table	;#a0h
	dc.w	poly_key_pressure-event_jump_table	;#a1h(���̑�)
	dc.w	poly_key_pressure-event_jump_table	;#a2h(���̑�)
	dc.w	poly_key_pressure-event_jump_table	;#a3h(���̑�)
	dc.w	poly_key_pressure-event_jump_table	;#a4h(���̑�)
	dc.w	poly_key_pressure-event_jump_table	;#a5h(���̑�)
	dc.w	poly_key_pressure-event_jump_table	;#a6h(���̑�)
	dc.w	poly_key_pressure-event_jump_table	;#a7h(���̑�)
	dc.w	poly_key_pressure-event_jump_table	;#a8h(���̑�)
	dc.w	poly_key_pressure-event_jump_table	;#a9h(���̑�)
	dc.w	poly_key_pressure-event_jump_table	;#aah(���̑�)
	dc.w	poly_key_pressure-event_jump_table	;#abh(���̑�)
	dc.w	poly_key_pressure-event_jump_table	;#ach(���̑�)
	dc.w	poly_key_pressure-event_jump_table	;#adh(���̑�)
	dc.w	poly_key_pressure-event_jump_table	;#aeh(���̑�)
	dc.w	poly_key_pressure-event_jump_table	;#afh(���̑�)
	dc.w	control_change-event_jump_table		;#b0h(���̑�)
	dc.w	control_change-event_jump_table		;#b1h(���̑�)
	dc.w	control_change-event_jump_table		;#b2h(���̑�)
	dc.w	control_change-event_jump_table		;#b3h(���̑�)
	dc.w	control_change-event_jump_table		;#b4h(���̑�)
	dc.w	control_change-event_jump_table		;#b5h(���̑�)
	dc.w	control_change-event_jump_table		;#b6h(���̑�)
	dc.w	control_change-event_jump_table		;#b7h(���̑�)
	dc.w	control_change-event_jump_table		;#b8h(���̑�)
	dc.w	control_change-event_jump_table		;#b9h(���̑�)
	dc.w	control_change-event_jump_table		;#bah(���̑�)
	dc.w	control_change-event_jump_table		;#bbh(���̑�)
	dc.w	control_change-event_jump_table		;#bch(���̑�)
	dc.w	control_change-event_jump_table		;#bdh(���̑�)
	dc.w	control_change-event_jump_table		;#beh(���̑�)
	dc.w	control_change-event_jump_table		;#bfh(���̑�)
	dc.w	program_change-event_jump_table		;#c0h(���̑�)
	dc.w	program_change-event_jump_table		;#c1h(���̑�)
	dc.w	program_change-event_jump_table		;#c2h(���̑�)
	dc.w	program_change-event_jump_table		;#c3h(���̑�)
	dc.w	program_change-event_jump_table		;#c4h(���̑�)
	dc.w	program_change-event_jump_table		;#c5h(���̑�)
	dc.w	program_change-event_jump_table		;#c6h(���̑�)
	dc.w	program_change-event_jump_table		;#c7h(���̑�)
	dc.w	program_change-event_jump_table		;#c8h(���̑�)
	dc.w	program_change-event_jump_table		;#c9h(���̑�)
	dc.w	program_change-event_jump_table		;#cah(���̑�)
	dc.w	program_change-event_jump_table		;#cbh(���̑�)
	dc.w	program_change-event_jump_table		;#cch(���̑�)
	dc.w	program_change-event_jump_table		;#cdh(���̑�)
	dc.w	program_change-event_jump_table		;#ceh(���̑�)
	dc.w	program_change-event_jump_table		;#cfh(���̑�)
	dc.w	channnel_pressure-event_jump_table	;#d0h(���̑�)
	dc.w	channnel_pressure-event_jump_table	;#d1h(���̑�)
	dc.w	channnel_pressure-event_jump_table	;#d2h(���̑�)
	dc.w	channnel_pressure-event_jump_table	;#d3h(���̑�)
	dc.w	channnel_pressure-event_jump_table	;#d4h(���̑�)
	dc.w	channnel_pressure-event_jump_table	;#d5h(���̑�)
	dc.w	channnel_pressure-event_jump_table	;#d6h(���̑�)
	dc.w	channnel_pressure-event_jump_table	;#d7h(���̑�)
	dc.w	channnel_pressure-event_jump_table	;#d8h(���̑�)
	dc.w	channnel_pressure-event_jump_table	;#d9h(���̑�)
	dc.w	channnel_pressure-event_jump_table	;#dah(���̑�)
	dc.w	channnel_pressure-event_jump_table	;#dbh(���̑�)
	dc.w	channnel_pressure-event_jump_table	;#dch(���̑�)
	dc.w	channnel_pressure-event_jump_table	;#ddh(���̑�)
	dc.w	channnel_pressure-event_jump_table	;#deh(���̑�)
	dc.w	channnel_pressure-event_jump_table	;#dfh(���̑�)
	dc.w	pitch_bend-event_jump_table		;#e0h(���̑�)
	dc.w	pitch_bend-event_jump_table		;#e1h(���̑�)
	dc.w	pitch_bend-event_jump_table		;#e2h(���̑�)
	dc.w	pitch_bend-event_jump_table		;#e3h(���̑�)
	dc.w	pitch_bend-event_jump_table		;#e4h(���̑�)
	dc.w	pitch_bend-event_jump_table		;#e5h(���̑�)
	dc.w	pitch_bend-event_jump_table		;#e6h(���̑�)
	dc.w	pitch_bend-event_jump_table		;#e7h(���̑�)
	dc.w	pitch_bend-event_jump_table		;#e8h(���̑�)
	dc.w	pitch_bend-event_jump_table		;#e9h(���̑�)
	dc.w	pitch_bend-event_jump_table		;#eah(���̑�)
	dc.w	pitch_bend-event_jump_table		;#ebh(���̑�)
	dc.w	pitch_bend-event_jump_table		;#ech(���̑�)
	dc.w	pitch_bend-event_jump_table		;#edh(���̑�)
	dc.w	pitch_bend-event_jump_table		;#eeh(���̑�)
	dc.w	pitch_bend-event_jump_table		;#efh(���̑�)
	dc.w	other_event-event_jump_table		;#f0h(���̑�)
	dc.w	other_event-event_jump_table		;#f1h(���̑�)
	dc.w	other_event-event_jump_table		;#f2h(���̑�)
	dc.w	other_event-event_jump_table		;#f3h(���̑�)
	dc.w	other_event-event_jump_table		;#f4h(���̑�)
	dc.w	other_event-event_jump_table		;#f5h(���̑�)
	dc.w	other_event-event_jump_table		;#f6h(���̑�)
	dc.w	other_event-event_jump_table		;#f7h(���̑�)
	dc.w	other_event-event_jump_table		;#f8h(���̑�)
	dc.w	other_event-event_jump_table		;#f9h(���̑�)
	dc.w	other_event-event_jump_table		;#fah(���̑�)
	dc.w	other_event-event_jump_table		;#fbh(���̑�)
	dc.w	other_event-event_jump_table		;#fch(���̑�)
	dc.w	other_event-event_jump_table		;#fdh(���̑�)
	dc.w	other_event-event_jump_table		;#feh(���̑�)
	dc.w	other_event-event_jump_table		;#ffh(���̑�)
reference:									;reference �R�}���h�̏���
		moveq.l	#0,d7
		move.b	(a0)+,d7		;��Ԓn����̓ǂݍ��݂��l����
						;�o�C�g�A�N�Z�X�� 2 �񂵂Ă���
						;( ���[�h�A�N�Z�X�͂��Ȃ� )
		lsl.w	#8,d7
		move.b	(a0)+,d7		;d7=�g���b�N�擪����Q�Ɛ�܂ł�
						;�I�t�Z�b�g
		move.b	(a0)+,ofst_rest_of_referenced(a1)
						;�Q�ƃC�x���g���L�^
		move.l	a0,ofst_ret_of_ref(a1)
						;�߂�� ( reference �̎� )
						;����ǂݏo���A�h���X ( = �Q�Ɛ� )

		move.l	ofst_top_of_track(a1),a6
		lea.l	(a6,d7.l),a0		;a0 = ���� �ȓ��A�N�Z�X�ʒu
						;����ǂݏo���ʒu�͂��̃��[�`��
						;(= �t�@�C��)�̍Ō�̕��ŏ���
						;�̂ŁAarea_1 �ɂ͂����ł�
						;�����Ȃ��ėǂ�
		bra	get_1_event
loop:
		mc_add_to_delta_time		;��time ���Z
		cmpi.l	#0,ofst_lp_start(a1)	;���܂łɃ��[�v�R�}���h���������� ?
		beq	loop_cmd_is_not_existed	;�Ȃ���������
											;���������̃��[�`��
		move.l	ofst_lp_start(a1),a0
		bra	get_1_event

loop_cmd_is_not_existed:			;���܂łɃ��[�v�R�}���h���Ȃ�����
						;���̃��[�`��
		move.l	a0,ofst_lp_start(a1)
		bra	get_1_event

end_of_track:					;�g���b�N�I�[�̏���
		moveq.l	#1,d0
		bra	end_of_decode
program_change:					;#c0h
channnel_pressure:				;#d0h
		move.b	(a0)+,d4
		bclr.l	#7,d4							;Data 1
		bra	continuous_ctrl_cont
pitch_bend:									;#e0h
		moveq.l	#0,d4
		move.b	(a0)+,d5
		bclr.l	#7,d5			;Data 2
		bra	continuous_ctrl_cont
poly_key_pressure:				;#a0h
control_change:					;#b0h
		move.b	(a0)+,d4		;Data 1
		move.b	(a0)+,d5		;Data 2
		bclr.l	#7,d4

continuous_ctrl_cont:
		bne	continuous_ctrl_ne	;channel <= 15 ( count from 0 )
		move.l	d2,d3			;d3 �� ext cmd �� .w �Ŏg����
						;����̂ŁA�K�� .w , .l �Ŏg��
						;�����Ȃ��ƃS�~������
		andi.b	#00001111b,d2
		andi.b	#11110000b,d3		;clear lower 4 bit of midi cmd

		mc_add_to_delta_time		;��time ���Z
		bra	reference_check

continuous_ctrl_ne
		move.l	d2,d3			;d3 �� ext cmd �� .w �Ŏg����
						;����̂ŁA�K�� .l �Ŏg�������Ȃ���
						;�S�~������
		andi.b	#00001111b,d2
		bset	#4,d2
		andi.b	#11110000b,d3		;clear lower 4 bit of midi cmd

		mc_add_to_delta_time		;��time ���Z
		bra	reference_check




ext_gate_200h:					;#88h
ext_gate_800h:					;#89h
ext_gate_1000h:					;#8ah
ext_gate_2000h:					;#8bh
		move.w	ext_table(pc,d7),d3
		add.l	d3,d6
		bra	get_1_event
ext_delta_100h:					;#8ch
ext_delta_200h:					;#8dh
ext_delta_800h:					;#8eh
ext_delta_1000h:				;#8fh
		move.w	ext_table(pc,d7),d3
		add.l	d3,d1
		bra	get_1_event
ext_table:
	dc.w	0
	dc.w	0
	dc.w	0
	dc.w	0
	dc.w	0
	dc.w	0
	dc.w	0
	dc.w	0
	dc.w	0200h		;gate
	dc.w	0800h
	dc.w	1000h
	dc.w	2000h
	dc.w	0100h		;delta
	dc.w	0200h
	dc.w	0800h
	dc.w	1000h
other_event:					;�C�x���g���o���~�X���̏���
		move.l	#255,d0			;�ُ�I���X�e�[�^�X
		bra	end_of_decode

jump_cont:
		move.w	top_of_jump_table(pc,d0),d0
		jmp	top_of_jump_table(pc,d0)
top_of_jump_table:
		dc.w	tp_0-top_of_jump_table
		dc.w	tp_1-top_of_jump_table
		dc.w	tp_2-top_of_jump_table
		dc.w	tp_3-top_of_jump_table

tp_1:		adda.w	4(a0),a0		;���t�f�[�^ ADR -> area_1
		move.l	a0,(a1)
		move.l	a0,ofst_top_of_track(a1)

		moveq.l	#0,d7
		move.l	d7,ofst_lp_start(a1)	;area_1 �����[�v�J�n�ʒu�N���A
		move.b	d7,ofst_rest_of_referenced(a1)
		bra	output_event

tp_2:		moveq.l	#0,d0			;status
		movea.l	a0,a2
		adda.w	4(a0),a2		;���t�f�[�^ ADR -> area_1
		movea.l	a2,ofst_top_of_track(a1)

		moveq.l	#0,d7
		move.l	d7,ofst_1st_tmp_in_lp(a1)
		move.w	6(a0),d7
		beq	prepare_to_out_1st_tmp
										;when tempo loop exists
		add.l	a0,d7
		move.l	d7,ofst_1st_tmp_in_lp(a1)

prepare_to_out_1st_tmp:
		lea	8(a0),a2
		bra	output_1_tmp_event

tp_3:		moveq.l	#0,d0
		move.l	ofst_nxt_adr_in_tmp(a1),a2	;����ǂݏo���ʒu����
		cmpa.l	ofst_top_of_track(a1),a2
		bne	output_1_tmp_event
		; when rest of tempo not exists
		cmpi.l	#0,ofst_1st_tmp_in_lp(a1)	;tempo loop is exists?
		bne	tempo_lp
										;when tempo loop not exists or old ver.
		moveq.l	#1,d0
		bra	end_of_decode
tempo_lp:
		move.l	ofst_1st_tmp_in_lp(a1),ofst_nxt_adr_in_tmp(a1)
		move.l	ofst_nxt_adr_in_tmp(a1),a2

;�� time �ǂ��o���A�e���|�C�x���g�o��
;���� :
;		a1,����ǂݏo���ʒu
;		a1,area_1 �� ���̋Ȃ̓�
;�j�� :
;�o�� :

output_1_tmp_event:
		move.l	(a2)+,d1		;step
		move.l	(a2)+,d6		;tempo value

		move.l	a2,ofst_nxt_adr_in_tmp(a1)	;����ɔ����ēǂ݈ʒu�ۑ�
		bra	end_of_decode
sounddata:									;���̉��ɋȃf�[�^�������N�����B
