;
; SCSP 68k program
;
; Target : 171-6638A SATURN SOUND BOARD
;
;						SEGA R&D AM2 Sound yamamoto

	include	SCSP.LIB

	external	tune_wr,MIDI_ctrl,HOST_IF
	external	FH1005_init,wk_RAM_init,MONITOR_init
	external	HOST_IF_init,BANK_init,sl_layer_adr_init
	external	vector_tb_init,SEGA_MARK
	external	xseq_2msec
	external	send_ACK,send_NACK,get_MIDI
	external	MIXER_wr,MIXER_wk_wr,MAP_chg
	external	YM3802_init

	if	RD_mode
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
	dc.l	er_09		; 	    #29 Level#5 MIDI #1
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
		cmpi.l	#$53454741,SEGA_MARK
		bne.s	trgt_top
		moveq	#0,d0
		move.l	d0,d1
		move.l	d0,d2
		move.l	d0,d3
		move.l	d0,d4
		move.l	d0,d5
		move.l	d0,d6
		bra.s	trgt_top_1

		dc.b	"Ver1.31 95/06/20"	; �ꏊ���ړ�����ȁI
						;     �i��ʂɕ\�����邼�j
	if	RD_mode
		dc.b	"SATURN(M) sample"
        else
		if	SW_TITAN
		dc.b	"<< STV Master >>"
		else
		dc.b	"SATURN(S) sample"
		endif
	endif

 		dc.b	"Nao V0.00q      "
 		dc.b	"Kas V0.00c      "
trgt_top_1:
		move.l	d0,d7
		movea.l	d0,a0
		movea.l	d0,a1
		movea.l	d0,a2
		movea.l	d0,a3
		movea.l	d0,a4
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
	if	RD_mode
		jsr	MONITOR_init(pc)	; for Tone Editor
		jsr	HOST_IF_init(pc)	;
		moveq	#0,d0			; ready �l�`�o�ԍ�
		jsr	MAP_chg(pc)		;
		jsr	vector_tb_init(pc)	; Vector table initial
		jsr	BANK_init(pc)		; [scspbin_adr_bf] initial
		jsr	sl_layer_adr_init(pc)

		move.w	#0,_activ_MIX_NO(a6)	; ready Mixer# ( default )
		movea.l	k_BANK_adr(a6),_activ_MIX_BNK(a6)
			; = �Ǘ��ԍ����O�AMIDI ch#0 �̉��FBANK#0
		jsr	MIXER_wr(pc)		;
		jsr	YM3802_init(pc)		;
	endif
		move.b	#2,RG_M4D8(a5)		; SCSP 4Mbit D-RAM mode

	if	SCSI_vctr_chg
		jsr	TRGTSCSI_trans(pc)
	endif
		move.w	#$2000,SR		; Enable Interupt
;=======================================================================;
;									;
;				main routine				;
;									;
;=======================================================================;
		global	main_lp
main_lp:	addq.w	#2,CPU_CAPA_cnt(a6)
		;-------------------------------;
		; ��M �l�h�c�h �o�b�t�@ �qead	;
		;-------------------------------;
		moveq	#0,d0			;
		move.b	_MIDI_RCV_RDPT(a6),d0	; 11/16
		add.w	d0,d0			;
		add.w	d0,d0			;
		moveq	#0,d1			;
		move.b	_MIDI_RCV_WRPT(a6),d1	; 0 �` ff
		add.w	d1,d1			;
		add.w	d1,d1			;

		cmp.w	d0,d1			;
		beq.s	main_1			; jump if input MIDI nothing
		jsr	MIDI_ctrl(pc)		; ready d0.w
		;-------------------------------;
		; ���� �l�h�c�h �o�b�t�@ �o��	;
main_1:		;-------------------------------;
		move.b	bs_SIFWK+OF_HNDSHK_flg,d0	; HOST I/F H-shake flg
		andi.b	#$60,d0
		beq.s	main_2			; jump if bit6,5 = L
		moveq	#0,d4			;
		move.b	_MIDI_OUT_RDPT(a6),d4	; byte
		cmp.b	_MIDI_OUT_WRPT(a6),d4	; byte
		beq.s	main_2			;
		lea	_MIDI_OUT_BF(a6),a2	;
		btst.b	#4,RG_MSTS(a5)		; MIDI OUT FIFO full ?
		bne.s	main_2			;
		move.b	(a2,d4.w),RG_MOBUF(a5)	;
		addq.b	#1,_MIDI_OUT_RDPT(a6)	; byte
main_2:		;-------------------------------;
		jsr	HOST_IF_rd(pc)
		;-------------------------------;
		;  �^�C�}�[�` (�|�[�����O�Ď�)	;
		;-------------------------------;
		global	Timebase
Timebase:	move.w	RG_SCIPD(a5),d0		; �`12
		andi.w	#$40,d0			; �`8 Timer-A over flow ?
		beq.s	main_lp			; jump if < 2msec
		;-------------------------------;
	if	RD_mode
		move.w	CPU_CAPA_cnt(a6),d1
		subq.w	#1,d1
		bne.s	MIDI_capa_1
		ori.b	#%00000010,LED_status(a6)
		move.b	#CTO_LED_speed,CTO_LED_cnt(a6)
		addi.l	#1,OVER_count(a6)
MIDI_capa_1:	;jsr	MIDI_capa_out(pc)
		move.w	#0,CPU_CAPA_cnt(a6)
	endif

Timebase_rst:	move.w	#TIMA_base,RG_TACTL(a5)	; 2msec [TACTL] & [TIMA]
		move.w	#$40,RG_SCIRE(a5)		; timer-A reset
		addq.b	#1,BIG_timer(a6)	;
		;-------------------------------;
	if	RD_mode
		external	LED_CTRL
		jsr	LED_CTRL(pc)		; �k�d�c����
		jsr	MAC_MIXER(pc)		; for MAC TONE-Editor
	endif
		nop
		nop
		nop
	if	list_play
		jsr	code_list_play(pc)	; Sound code list play
	endif
		jsr	slot_ctrl(pc)
		jsr	DSP_CPU(pc)
	if	RD_mode
		jsr	DSP_LINK(pc)
	endif
		;************************************************
		;�s�u�p �T�E���h�o�͂̃��m�����^�X�e���I�Ή�	*
		;************************************************
		global	TV_mono_chg
TV_mono_chg:	move.b	bs_SIFWK+OF_MONO_FLAG,d0	; STEREO/MONO ?
		cmp.b	SND_OUT_ST(a6),d0
		beq	main_00			; jump if bit7 no change
		move.b	d0,SND_OUT_ST(a6)	; save 
		bmi.s	to_mono
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
		bra	main_00
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
		;-------------------------------;
		;	    94/12/15		;
		;-------------------------------;
main_00:
;@		move.b	BIG_timer(a6),d0	;
;@		andi.b	#3,d0
;@		bne	main_lp
;@		move.b	(a5),d0			;
;@		ori.b	#%00010000,d0		; set KYONEX
;@		move.b	d0,(a5)			; write KYONEX & KYONB

		bra	main_lp
;-----------------------------------------------------------------------;
;		sound code list play [ $800 �` ]			;
;-----------------------------------------------------------------------;
	if	list_play
		global	CLPLY
CLPLY:
code_list_play:	move.w	CLPLY_cont(a6),d0
		beq.s	CLPLY_3
		subq	#1,d0
		move.w	d0,CLPLY_cont(a6)
		rts
CLPLY_3:	move.l	CLPLY_addr(a6),d1
		bne.s	CLPLY_0
CLPLY_5:	lea	$800,a0
		move.l	a0,CLPLY_addr(a6)
		bra.s	code_list_play
CLPLY_0:	movea.l	d1,a0
		lea	$700,a1
		moveq	#8,d7
CLPLY_2:	move.b	(a1),d0
		beq.s	CLPLY_1
		lea	$10(a1),a1
		dbra	d7,CLPLY_2
		rts
		;----------------------------
CLPLY_1:	move.l	(a0)+,(a1)+
		beq.s	CLPLY_5
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		move.w	(a0)+,(a1)+
		move.w	(a0)+,d0		; = counter
		move.w	$0E(a0),d0		; = counter
		move.l	a0,CLPLY_addr(a6)
		move.w	d0,CLPLY_cont(a6)
		beq.s	code_list_play
		rts
	endif
;-----------------------------------------------------------------------;
;			�c�r�o�����J �cirect down load			;
;-----------------------------------------------------------------------;
	if	RD_mode
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
		beq.s	main_A			; jump if Element nothing
	external	DSPEGLFO
		jsr	DSPEGLFO(pc)		; ready d7.w
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
		beq.s	EFCT_CHG_0
		subi.b	#$20,(a0)
EFCT_CHG_0:	lea	SCSP_slot_unit(a0),a0
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

		moveq	#0,d4			;
		move.w	#$100,d5
		move.b	BIG_timer(a6),d0	;
		andi.w	#3,d0
		add.w	d0,d0
		jmp	slot_JPTB(pc,d0.w)

slot_JPTB:	add.w	d5,d4			; 4msec
		add.w	d5,d4			; 4msec
		add.w	d5,d4			; 4msec
slot_ctrl_00:	lea	slot_work(a6),a4	;
		moveq	#slot_size/4-1,d7	; loop ( slot ) size

slot_ctrl_lp:	move.w	d4,d5				;
		add.w	d5,d5				;
		btst.b	#PCM_flg,PSPN(a4,d5.w)		; PCM �Đ� mode ?
		bne.s	next_slot			; jump if Yes
		add.w	d4,d4				; = 0,$40,..,$7C0

		; pitch bend

		clr.w	d5			;
		move.b	sl_MIDI(a4,d4.w),d5	; = MIDI ch# (0�`1FH)
		lsl.w	#4,d5			; *10H
		or.w	_sl_kanri(a4,d4.w),d5	; + �����Ǘ��ԍ��~200H
		move.w	k_PBend_BF(a6,d5.w),d5	; = Pitch Bend data

		move.l	sl_layer_adr(a4,d4.w),d0	;ready
		bne.s	slot_ctrl_3
		lsr.w	#1,d4			; = 0,$20,$40,..,$3E0
		bra.s	next_slot

slot_ctrl_3:	movea.l	d0,a2
		btst.b	#PLON_flg,sl_flag1(a4,d4.w)
		beq.s	slot_ctrl_1		; jump if if PLFO off
		jsr	add_PLFO(pc)		; return d5.w = + PLFO freq
slot_ctrl_1:
		btst.b	#PEON_flg,sl_flag1(a4,d4.w)
		beq.s	slot_ctrl_2		; jump if if PEG off
		jsr	add_PEG(pc)		; return d5.w = + PEG freq
slot_ctrl_2:
		cmp.w	sl_tune(a4,d4.w),d5	;
		bne.s	slot_ctrl_4
		lsr.w	#1,d4			; = 0,$20,$40,..,$3E0
		bra.s	next_slot		;
slot_ctrl_4:	move.w	d5,sl_tune(a4,d4.w)	;
		lsr.w	#1,d4			; = 0,$20,$40,..,$3E0

		jsr	tune_wr(pc)		; ready d5.w
next_slot:	addi.w	#SCSP_slot_unit,d4	; = 0,$20,$40,..,$3E0
		dbra	d7,slot_ctrl_lp		;
		rts
;-----------------------------------------------------------------------;
;			�g�n�r�s �h�^�e					;
; �R�}���h�̃N���A�̓R�}���h������Ɏ��s���邱�ƁB			;
;-----------------------------------------------------------------------;
		global	HOST_IF_rd
HOST_IF_rd:	move.b	bs_SIFWK+OF_HNDSHK_flg,d0	; HOST I/F H-shake flag
		bpl.s	HOST_IF_old		;
		;===============================;
		;   Hand shake mode   95/03/08	;
		;===============================;
HOST_IF_new:	move.b	bs_SIFWK+OF_HNDSHK_EN,d0
		bpl.s	HOST_IF_rd_DI		; jump if HOST ������
		jsr	HOST_IF_old(pc)		;
		andi.b	#$7F,bs_SIFWK+OF_HNDSHK_EN	; set Sound ��������
HOST_IF_rd_DI:	rts
		;===============================;
HOST_IF_old:	move.w	#bs_HIFWK,d0		; HOST I/F work top addr
HOST_IF_rd_lp:	movea.w	d0,a0			; = 700,710,720,,,7E0,7F0
		move.b	(a0),d7			; Command �� 0 ?
		beq.s	main_1A			; jump if Command nothing
		addq.l	#2,a0			; P1 address
		move.w	d0,tmp_HIF_wk(a6)	;+push	4,12
		;-------------------------------;
		; Sound Driver �����R�}���h�̗���
		;-------------------------------;
	if	.not.list_play
		movem.l	d0/a1,-(SP)		; reg. push
		move.w	debug_offset(a6),d0	;
		andi.w	#$7f0,d0		;
		lea	$800,a1			;
		adda.w	d0,a1			;
		move.w	-2(a0),(a1)+		;
		move.l	0(a0),(a1)+		;
		move.l	4(a0),(a1)+		;
		move.l	8(a0),(a1)+		;
		addi.w	#$10,d0			;
		move.w	d0,debug_offset(a6)	;
		move.w	d0,Mem_CMDHS_pt		; HOST Commnd bufferring point
		move.w	debug_cnt(a6),(a1)	;
		move.w	#0,debug_cnt(a6)	;
		;-------------------------------;
		; 95/04/21 HOST Command MIDI out
		;-------------------------------;
		move.b	bs_SIFWK+OF_HNDSHK_flg,d0 ; D5 = H : HOST input 
		add.b	d0,d0			  ;	     Command MIDI out
		add.b	d0,d0			  ;
		bpl.s	Rireki_1

		moveq	#0,d0			;
		move.b	_MIDI_OUT_WRPT(a6),d0	; byte
		lea	_MIDI_OUT_BF(a6),a2	;
		lea	-2(a0),a3		;
		moveq	#16-1,d1		; loop size
Rireki_2:	move.b	(a3)+,(a2,d0.w)		; MIDI bufferring
		addq.b	#1,d0			;
		dbra	d1,Rireki_2		;
		move.b	d0,_MIDI_OUT_WRPT(a6)	; byte
Rireki_1:	movem.l	(SP)+,d0/a1		; reg. pop
		;-------------------------------;
	endif
		jsr	HOST_IF(pc)		; ready a0 , d7 & flag
		move.w	tmp_HIF_wk(a6),d0	;-pop
		movea.w	d0,a0			;
		move.w	#0,(a0)			; clear Command sign
main_1A:	addi.b	#16,d0			; = 00,10,20,30,..,70H + 10H
		bpl	HOST_IF_rd_lp		; 09/30
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
		global	add_PLFO
add_PLFO:	move.w	PLFO_Delay(a4,d4.w),d0	; 1msec counter
		beq.s	add_PLFO_1		;
		subq.w	#_4msec,d0		;
		bcc.s	add_PLFO_2		;
		clr.w	d0			;
add_PLFO_2:	move.w	d0,PLFO_Delay(a4,d4.w)	;
		rts
		;-------------------------------;
		;	Delay counter = 0	;
		;-------------------------------;
add_PLFO_1:	move.w	PLFO_HTCNT_wk(a4,d4.w),d0
		subq.w	#_4msec,d0		;
		bpl.s	add_PLFO_3
		;-------------------------------;
		;	   �������]		;
		;-------------------------------;
		move.w	d0,d2
		add.w	PLFO_HTCNT_bs(a4,d4.w),d0
		move.w	d0,PLFO_HTCNT_wk(a4,d4.w)

		moveq	#0,d1			; = FRQR_bs
		move.w	PLFO_FDCNT(a4,d4.w),d0	;
		beq.s	add_PLFO_4		; jump if FDCT = 0
		;-------------------------------;
		;	FDCNT �� 0		;
		;-------------------------------;
		subq.w	#1,d0
		move.w	d0,PLFO_FDCNT(a4,d4.w)
		move.l	PLFO_FRQR_bs(a4,d4.w),d1
		neg.l	d1
		move.l	d1,PLFO_FRQR_bs(a4,d4.w)
		neg.l	d1
		;-------------------------------;
		;	FDCNT = 0		;
		;-------------------------------;
add_PLFO_4:	move.l	PLFO_FRQR_wk(a4,d4.w),d6
		move.l	PLFO_cent(a4,d4.w),d0	; (1msec)
		addq.w	#_4msec,d2		; = 0,1,2,3
		andi.w	#3,d2
		add.w	d2,d2
		jmp	add_PLFO_JPTB(pc,d2.w)
add_PLFO_JPTB:	bra.s	d2_0
		bra.s	d2_1
		bra.s	d2_2
d2_3:
		add.l	d6,d0			; Cent    <-- �{ FRQR_wk
		add.l	d6,d0			; Cent    <-- �{ FRQR_wk
		add.l	d6,d0			; Cent    <-- �{ FRQR_wk
		add.l	d1,d6			; FRQR_wk <-- �{ FRQR_bs
		neg.l	d6
		bra.s	d2_0C

d2_2:		add.l	d6,d0			; Cent    <-- �{ FRQR_wk
		add.l	d6,d0			; Cent    <-- �{ FRQR_wk
		add.l	d1,d6			; FRQR_wk <-- �{ FRQR_bs
		neg.l	d6
		bra.s	d2_0B

d2_1:		add.l	d6,d0			; Cent    <-- �{ FRQR_wk
		add.l	d1,d6			; FRQR_wk <-- �{ FRQR_bs
		neg.l	d6
		bra.s	d2_0A

d2_0:		add.l	d1,d6			; FRQR_wk <-- �{ FRQR_bs
		neg.l	d6
		add.l	d6,d0			; Cent    <-- �{ FRQR_wk
d2_0A:		add.l	d6,d0			; Cent    <-- �{ FRQR_wk
d2_0B:		add.l	d6,d0			; Cent    <-- �{ FRQR_wk
d2_0C:		add.l	d6,d0			; Cent    <-- �{ FRQR_wk
d2_0123:	move.l	d6,PLFO_FRQR_wk(a4,d4.w)
		move.l	d0,PLFO_cent(a4,d4.w)
		bra.s	add_PLFO_B
		;-------------------------------;
add_PLFO_3:	move.w	d0,PLFO_HTCNT_wk(a4,d4.w)
		move.l	PLFO_FRQR_wk(a4,d4.w),d6
add_PLFO_5:	add.l	d6,d6			; (4msec)
add_PLFO_C:	add.l	d6,d6			; (2msec)
		add.l	d6,PLFO_cent(a4,d4.w)	; (1msec)
add_PLFO_B:	add.w	PLFO_cent(a4,d4.w),d5	;
		rts
;===============================================;
;  �o�d�f����i������,Key off����܂ށj		;
;	Pitch EG ���� addition			;
;�y���́z a2 : destination layer data top addr	;
;         a4 : slot work address                ;
;�y�j��z d0.l/d1.l/d3.w/a1			;
;===============================================;
		global	add_PEG
add_PEG:	clr.w	d0			;
		move.b	PEG_SEG(a4,d4.w),d0	; = Seg#
;@		beq	add_PEG_exit		;

		move.l	#$80000000,d1
		andi.w	#7,d0			; 0,1,2,3,...,7
		add.w	d0,d0
		add.w	d0,d0
		jmp	PEGJPTB(pc,d0.w)
PEGJPTB:	rts
		rts
		bra	PEG_1
		bra	PEG_2
		bra	PEG_3
		bra	PEG_4
		bra	PEG_5
		bra	PEG_6
		bra	PEG_7
		; <<<<<<<<<<< Delay >>>>>>>>>>>>;
PEG_1:		subq.w	#_4msec,PEG_dly_cnt(a4,d4.w)
		bcc.s	add_PEG_exit
		movea.l	PEG_addr(a4,d4.w),a1
		addq	#2,a1			; ready a1 : for [AR],[AL]
		bra.s	PEG_next_seg		;
		; <<<<<<<<<< Attack >>>>>>>>>>>>;
PEG_2:		movea.l	PEG_addr(a4,d4.w),a1
		addq	#4,a1			; ready a1 : for [DR],[DL]
		bra.s	PEG_23
PEG_3:		movea.l	PEG_addr(a4,d4.w),a1
		addq	#6,a1			; ready a1 : for [SR],[SL]
		bra.s	PEG_23
	global	PEG_4
PEG_4:		movea.l	PEG_addr(a4,d4.w),a1
		addq	#8,a1			; ready a1 : for [SR],[SL]
PEG_23:		move.l	PEG_cent(a4,d4.w),d0	;
		move.l	PEG_level(a4,d4.w),d2	;
		add.l	d1,d0			; +$8000
		add.l	d1,d2			; +$8000
		move.l	PEG_RATE(a4,d4.w),d6	;
		bpl.s	PEG_rate_pls
PEG_rate_mi:	add.l	d6,d0			; _cent + _RATE
		cmp.l	d0,d2			; new cent <cmp> level
		bcs.s	PEG_2A
		bra.s	PEG_rate_mp
		;- - - - - - - - - - - - - - - -;
PEG_rate_pls:	add.l	d6,d0			; _cent + _RATE
		cmp.l	d2,d0			; level <cmp> new cent
		bcs.s	PEG_2A
PEG_rate_mp:	sub.l	d1,d2			; -$8000
		move.l	d2,PEG_cent(a4,d4.w)	;
;@		bra.s	PEG_next_seg
		global	PEG_next_seg
PEG_next_seg:	addq.b	#1,PEG_SEG(a4,d4.w)
		move.b	(a1)+,d0		; = [DR]
		bne.s	PEG_next_sg1
		move.b	(a1)+,d0		; = [DL] = time counter
		beq	add_PEG


		external	get_EXPTB2
PEG_next_sg1:	jsr	get_EXPTB2(pc)		; return d0.w:-$3F01�`+$3F01
		swap	d0
		clr.w	d0
		asr.l	#4,d0			; for 4msec
		move.l	d0,PEG_RATE(a4,d4.w)	; set �X���^4msec
		move.b	(a1),d0			; = [DL]
		external	get_LEVEL
		jsr	get_LEVEL(pc)		; return d0.w:-$5FFF�`+$5FFF
		swap	d0
		clr.w	d0
		move.l	d0,PEG_level(a4,d4.w)	; set ���B���x��
		bra.s	add_PEG_exit
		;- - - - - - - - - - - - - - - -;
PEG_2A:		sub.l	d1,d0			; -$8000
		move.l	d0,PEG_cent(a4,d4.w)	;
		bra.s	add_PEG_exit
PEG_5:
PEG_7:		nop
		;-------------------------------;
add_PEG_exit:	add.w	PEG_cent(a4,d4.w),d5	;
		rts

	global	PEG_6
PEG_6:		move.l	PEG_cent(a4,d4.w),d0	;
		move.l	PEG_level(a4,d4.w),d2	;
		add.l	d1,d0			; +$8000
		add.l	d1,d2			; +$8000
		move.l	PEG_RATE(a4,d4.w),d6	;
		bpl.s	PEG_6D
PEG_6_mi:	add.l	d6,d0			; _cent + _RATE
		cmp.l	d0,d2			; new cent <cmp> level
		bcs.s	PEG_6B
		bra.s	PEG_6C
		;- - - - - - - - - - - - - - - -;
PEG_6D:		add.l	d6,d0			; _cent + _RATE
		cmp.l	d2,d0			; level <cmp> new cent
		bcs.s	PEG_6B
PEG_6C:		sub.l	d1,d2			; -$8000
		move.l	d2,PEG_cent(a4,d4.w)	;
		addq.b	#1,PEG_SEG(a4,d4.w)	;
		bra.s	PEG_6A
PEG_6B:		sub.l	d1,d0			; -$8000
		move.l	d0,PEG_cent(a4,d4.w)	;
PEG_6A:		add.w	PEG_cent(a4,d4.w),d5	;
		rts
;************************************************************************
;									*
;		YM-8302 ( MCS-1 & 2 ) ��������				*
;									*
;************************************************************************
	if	RD_mode
		global	level_5,level_6
		external	INT_YM3802
level_6:	movem.l	d0-d7/a0-a6,-(SP)		; reg. push
		lea	bs_PWKTP,a6			; 68K Prg work top address
		ori.b	#%10000000,LED_status(a6)
		move.b	#MIDI_LED_speed,RCVA_LED_cnt(a6)
		lea	IO_MIDI1+1,a0			; ready a0 @005
		lea	MIDIA_wk(a6),a1			; ready a1
		jsr	INT_YM3802(pc)
		movem.l	(SP)+,d0-d7/a0-a6		; reg. pop
		rte
		;---------------------------------------;
level_5:	movem.l	d0-d7/a0-a6,-(SP)		; reg. push
		lea	bs_PWKTP,a6			; 68K Prg work top address
		ori.b	#%01000000,LED_status(a6)
		move.b	#MIDI_LED_speed,RCVB_LED_cnt(a6)
		lea	IO_MIDI2+1,a0			; ready a0 @005
		lea	MIDIB_wk(a6),a1			; ready a1
		jsr	INT_YM3802(pc)
		movem.l	(SP)+,d0-d7/a0-a6		; reg. pop
		rte
	endif
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

		move.w	RG_SCIPD(a5),d0		;
		andi.w	#$80,d0			;
		beq.s	level_2_exit

		move.w	#TIMB_VL,RG_TBCTL(a5)	; timer-B counter set
		move.w	#$80,RG_SCIRE(a5)	; timer-B reset
		lea	bs_PWKTP,a6		; 68K Prg work top address
		addi.w	#SEQ_LED_speed,SEQ_LED_cnt(a6)	; <<< �r�d�p�𓀊��� >>>
		addq.w	#1,debug_cnt(a6)
;@		lea	SEQ_decode_ofst(a6),a6	; 68k SEQ Prg work top
		jsr	xseq_2msec(pc)
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
	if	RD_mode
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
;--------------------------------------------------------------------------

	end

