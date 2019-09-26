;====================================================================
;	/////  SATURN sound driver  /////
;	[ sequence control : SDDRV.ASM ] program source
;	Ver 1.28 1994/12/13
;	DM/Sound K.Fujishima
;====================================================================
	include SCSP.LIB	;sddrv.inc

	extern	com_eventout		; sdcom
	extern	com_set_seqmode		; sdcom
	extern	com_get_priority	; sdcom
	extern	com_seq_noteoff		; sdcom
	extern	com_seq_noteon		; sdcom
	extern	com_notecue_set		; sdcom
	extern	com_get_mapaddress	; sdcom
	extern	com_get_workaddress	; sdcom
	extern	com_volanalize		; sdcom
	extern	com_pcm_control		; sdcom
	extern	com_make_faderate	; sdcom
	extern	com_load_mixer		; sdcom
	extern	qsound_pan_table	; sdmain
	extern	ret_error		; sdmain
	extern	sq_decoder		; dec_main

;====================================================================
;	SEQ_2MSEC
;	sequence decoder 2msec interval control
;	input	nothing
;	output	nothing
;	break	d0-d7,a0-a6
;====================================================================
	public	xseq_2msec
	extern	dsp3d_op

xseq_2msec:
	bsr	com_get_workaddress		; set a4/a5/a6
	if	nao_chg
	else
	btst.b	#SY_BUSY_BITPOS,SY_BUSY(a4)	; check 2ms busy bit
	bne	seq90				; 2ms busy ?  yes
	bset.b	#SY_BUSY_BITPOS,SY_BUSY(a4)	; set 2ms BUSY bit
	endif
	;------- edit by Y.Kashima 31/01/95 ----;
	;	for YAMAHA 3D sound check	;
	;---------------------------------------;
	movem.l	d0-d7/a0-a7,-(sp)		;
	movea.l	Mem_IFWK_PTR,a0			; a0: system IFwork top(480h)
	move.b	OF_3D_FLAG(a0),d4		;
	andi.b	#$c0,d4				;YAMAHA 3D use check
	beq.w	?skip				;
	bsr	dsp3d_op			;
?skip:
	movem.l	(sp)+,d0-d7/a0-a7		;
	;---------------------------------------;
	if	nao_chg
	move.b	SY_TVOLCTL(a4),d0	;nao	; #4,12	; check vol analize bit
	bpl.w	seq_00			;nao	; test bit7=SY_VANCTL_BITPOS
	bsr	volume_analize		; 18	; CD/DA input volume analize
seq_00:
	else
	bsr	volume_analize			; CD/DA input volume analize
	endif

	bsr	pcm_address_search		; PCM stream play address search
;;;	bsr	mixer_control			; delayd Mixer enable interface

	move.w	#NC_NUM-1,d7			; max 32 cue
seq10:	bsr	noteoncue_control		; === note on cue control ===
	if	nao_chg
	lea	NC_SIZ(a5),a5		; #4,8	; next address
	else
	adda.l	#NC_SIZ,a5		; #6,14	; next address
	endif
	dbra.w	d7,seq10			;

	bsr	com_get_workaddress		; set a4/a5/a6
	bsr	sequence_control		; sequence 1
	lea	SQ_SIZ(a6),a6		; #4, 8	;
	bsr	sequence_control		; sequence 2
	lea	SQ_SIZ(a6),a6		; #4, 8	;
	bsr	sequence_control		; sequence 3
	lea	SQ_SIZ(a6),a6		; #4, 8	;
	bsr	sequence_control		; sequence 4
	lea	SQ_SIZ(a6),a6		; #4, 8	;
	bsr	sequence_control		; sequence 5
	lea	SQ_SIZ(a6),a6		; #4, 8	;
	bsr	sequence_control		; sequence 6
	lea	SQ_SIZ(a6),a6		; #4, 8	;
	bsr	sequence_control		; sequence 7
	lea	SQ_SIZ(a6),a6		; #4, 8	;
	bsr	sequence_control		; sequence 8
seq90:	clc
	rts

;====================================================================
;	SEQ_3BAND_ANALIZE_START
;	SEQ_3BAND_ANALIZE_END
;	input	nothing
;	output	nothing
;	break	d0-d7,a0-a6,flag
;====================================================================
	public	xseq_vlan_st
	public	xseq_vlan_ed

xseq_vlan_st:
	bsr	com_get_workaddress		; set a4/a5/a6
	bset.b	#SY_VANCTL_BITPOS,SY_TVOLCTL(a4)	; set volume analize bit
	if	nao_chg
	else
	bra.w	ret_normal
	endif
	clc
	rts

xseq_vlan_ed:
	bsr	com_get_workaddress		; set a4/a5/a6
	bclr.b	#SY_VANCTL_BITPOS,SY_TVOLCTL(a4)	; clear volume analize bit
	if	nao_chg
	else
	bra.w	ret_normal
	endif
	clc
	rts

;====================================================================
;	SEQ_TEMPOCHANGE
;	input	d0.b: Play Number (0-7)
;		d2.w: Tempo Data  (-32768-->+32767)
;	output	nothing
;	break	d0-d7,a0-a6,flag
;====================================================================
	public	xseq_tempo

xseq_tempo:

	bsr	com_get_workaddress	; set a4/a5/a6
	andi.w	#$0007,d0		; d0=play number(0-7)
	move.w	#SQ_SIZ,d7		; d7=64
	mulu.w	d0,d7			;
	adda.l	d7,a6			; a6: sequence work top address
	tst.w	d2			;
	bpl	tp200			; slow down?  no

tp100:	neg.w	d2			; minus --> plus(0001h-8000h)
	addi.w	#SQ_TPCHG_BASE,d2	; d2(0000h-7FFFh)+1000h
	move.l	CB_CT_ORGTIME(a6),d7	; d7=1 count original time(usec)
	move.w	#SQ_TPCHG_BASE,d6	;
	mulu.w	d6,d7			; count time x 1000h
	divu.w	d2,d7			;
	andi.l	#$0000ffff,d7		; clear divide rest
	move.l	d7,CB_CT_TIME(a6)	; >>>>>  set to NEW count time
	bra	tp900			;

tp200:	addi.w	#SQ_TPCHG_BASE,d2	; d1(0001h-8000h)+1000h
	move.l	CB_CT_ORGTIME(a6),d7	; d7=1 count time(usec)
	mulu.w	d2,d7			;
	lsr.l	#8,d7			;
	lsr.l	#4,d7			; divide 4096(12bit)
	move.l	d7,CB_CT_TIME(a6)	; >>>>>  set to NEW count time

tp900:
	clc
	rts

;====================================================================
;	SEQUENCE ALL STOP
;	Function[ All sequence note off output ]
;	Input	nothing
;	Output	nothing
;	break	d0-d7,a0-a6,flag
;====================================================================
	public	xseq_allstop

xseq_allstop:
	bsr	com_get_workaddress	; set a4/a5/a6
	move.w	#SQ_NUM-1,d6		; max 8 sequence
sas10:	move.b	#$00,(a6)		; >>> control bit all OFF
	move.b	#1,d7			; 1:control off
	bsr	com_seq_noteoff		; ===== all note off =====
	move.b	#SQ_MODE_STOP,CB_MODE(a6)	; mode:   stop
	move.b	#RET_NORMAL,CB_STAT(a6)	; status: normal
	bsr	com_set_seqmode		; ===== set seq mode/status =====
	adda.l	#SQ_SIZ,a6		; next address
	dbra.w	d6,sas10		; 8 sequence end?  no
	rts

;====================================================================
;	STOP_AND_START
;	STOP_TO_START
;	input	d0.b: Play Number	(0-7)
;		d1.b: Priority Level	(0-31)
;		d2.b: Play mode		(0-1)
;		a0.l: sequence data	top address
;		a4.l: decoder work	top address
;		a5.l: note ON cue table	top address
;		a6.l: sequence control block top address
;	output	d0: Status	00 normal
;				01 resolution out of range(24-960)
;				02 tempo data not found
;	break	d0-d7,a0-a6,flag
;====================================================================
	public	stop_and_start
	public	stop_to_start

stop_and_start:
	movem.l	a0/d0/d1/d2,-(sp)	; save
	move.b	#$00,(a6)		; ===== control bit all OFF =====
	move.b	#1,d7			; 1: note off & control off
	bsr	com_seq_noteoff		; ///// all note off /////
	move.b	#0,CB_NT_CNT(a6)	; ===== Note ON counter clear =====
	movem.l	(sp)+,a0/d0/d1/d2	; restore

stop_to_start:

; A.Miyazawa	{

		clr.w	up_down_flag(a6)
		clr.w	seq_volume_arrival(a6)
		clr.w	appointed_fade_rate(a6)
		clr.w	fade_count_variable(a6)
	;@;	move.w	#$0080,sequence_volume(a6)
		clr.w	performance_flag(a6)

;		}

	andi.l	#SQ_SQNUM_MASK,d0	; mask 0-7
	andi.l	#SQ_PRLEV_MASK,d1	; mask 0-31
	andi.l	#SQ_PMODE_MASK,d2	; mask 0-1
	move.b	d0,CB_SQ_NUM(a6)	; >>> SET sequence play number
	move.b	d1,CB_PR_LEV(a6)	; >>> SET sequence priority level
	move.b	d2,CB_SQ_MODE(a6)	; >>> SET sequence play mode
	move.l	a0,CB_SQ_TOP(a6)	; >>> SET sequence data top address

	move.b	#SQ_MODE_PLAY,CB_MODE(a6)	; >>> MODE:   PLAY
	btst.b	#SQ_FADE_BITPOS,(a6)		; fade bit ON?
	beq	stt10				; no
	move.b	#SQ_MODE_FADE,CB_MODE(a6)	; >>> MODE:   FADE
stt10:	move.b	#RET_NORMAL,CB_STAT(a6)		; >>> STATUS: NORMAL
	bsr	com_set_seqmode			; ///// Set seq MODE/STATUS /////

	lsl.l	#1,d0			; play number(0-7) x 2
	movea.l	Mem_HOSTIF_PTR,a1	; a1: host I/F work top(700h)
	adda.l	d0,a1			; add offset(0-14)
	lea	OF_HI_STAT1(a1),a2	;
	move.l	a2,CB_STS_ADRS(a6)	; >>> SET sequence status pointer
	lea	OF_HI_SEQ0(a1),a2	;
	move.l	a2,CB_PTR_ADRS(a6)	; >>> SET sequence position adrs
	move.w	#0,(a2)			; >>> CLEAR sequence position

	move.w	(a0),d0			; D0: Resolution(24-960)
	cmpi.w	#SQ_RESO_MIN,d0		;
	bcs	err_reso_val		; under 24?  yes
	cmpi.w	#SQ_RESO_MAX+1,d0	;
	bcc	err_reso_val		; over 960?  yes
	move.w	d0,CB_SQ_RESO(a6)	; >>> SET sequence resolution
	move.b	#0,CB_NT_CNT(a6)	; >>> SET note on counter(=0)

;
;	/////  Tempo initial  /////
;
	move.l	#DEC_TEMPO_INIT,d0	; mode: Tempo-Initial
	clr.l	d1			;
	move.b	CB_SQ_NUM(a6),d1	; Song Number(0-7)
	move.l	CB_SQ_TOP(a6),a0	; Sequence data top address

	movem.l	a4/a5/a6,-(sp)		;
	bsr	sq_decoder		; /////  decoder call  /////
	movem.l	(sp)+,a4/a5/a6		;
;;;	bsr	com_eventlog		; event log for DEBUG

	cmpi.b	#DEC_RET_NORMAL,d0	;
	beq	stt100			; normal?  yes
	bra	err_dec_error		; d0: 2-255(decoder error)

stt100:	cmpi.l	#SQ_TVAL_MIN,d2		;
	bcs	err_tempo_val		; under 200000(T300)?  yes
	cmpi.l	#SQ_TVAL_MAX+1,d2	;
	bcc	err_tempo_val		; over 1500000(T40)?  yes
	move.l	d1,CB_TP_CNT(a6)	; >>> SET Tempo delta time
	move.l	d2,CB_TP_VAL(a6)	; >>> SET Tempo value(usec/beat)
	lsl.l	#8,d1			; delta time x 256
	move.l	d1,CB_TP_TIMER(a6)	; >>> SET Tempo counter

	move.w	CB_SQ_RESO(a6),d0	; d0=24-960
	divu.w	d0,d2			; d2--->208-62500
	move.l	#IPT_COUNT_BASE,d0	; d0=2000x256(512000)
	divu.w	d2,d0			; d0--->2461-8
	andi.l	#$0000ffff,d0		; clear rest(bit8-15)
	move.l	d0,CB_CT_TIME(a6)	; >>> SET 1 Count time
	move.l	d0,CB_CT_ORGTIME(a6)	; >>> SET 1 Count original time

;
;	/////  Set first event  /////
;
stt150:	moveq.l	#DEC_SONG_INIT,d0	; mode: Song/Initial
	clr.l	d1			;
	move.b	CB_SQ_NUM(a6),d1	; Song Number(0-7)
	move.l	CB_SQ_TOP(a6),a0	; Sequence data top address

	movem.l	a4/a5/a6,-(sp)		;
	bsr	sq_decoder		; ///// decoder call /////
	movem.l	(sp)+,a4/a5/a6		;
;;;	bsr	com_eventlog		; event log for DEBUG

	cmpi.b	#DEC_RET_NORMAL,d0	;
	beq	stt200			; normal?  yes
	cmpi.b	#DEC_RET_NODATA,d0	;
	beq	err_no_event		; no data?  yes
	bra	err_dec_error		; d0: 2-255(decoder error)

stt200:	andi.b	#11110000b,d3		;
	move.b	d3,CB_EV_COM(a6)	; MIDI Command
	move.b	d4,CB_EV_S2(a6)		; MIDI data 2
	swap	d4			;
	move.b	d4,CB_EV_S1(a6)		; MIDI data 1

	lsl.l	#8,d1			; delta x 8
	lsl.l	#8,d2			; gate  x 8
	move.l	d1,CB_SQ_TIMER(a6)	; >>> SET sequence counter
	move.l	d2,CB_EV_GATE(a6)	; >>> SET gate Time

	lsr.w	#8,d0			; xx
	move.w	d0,d3			; xx
	andi.b	#00001111b,d0		; xx
	or.b	d0,CB_EV_COM(a6)	; xx
	lsr.w	#4,d3			; xx
	andi.b	#00000001b,d3		; xx
	move.b	d3,CB_EV_PORT(a6)	; xx

	andi.b	#00010000b,(a6)		; control word clear
	ori.b	#10000000b,(a6)		; >>> CTL bit on
	if	nao_chg
	else
	bra.w	ret_normal		; ===== return to main =====
	endif
	clc
	rts

;
;	/////  Error Handle  /////
;
err_reso_val:	move.b	#RET_ERR_RESOVAL,CB_STAT(a6)
		bra	stt_error
err_no_event:	move.b	#RET_ERR_NOEVENT,CB_STAT(a6)
		bra	stt_error
err_dec_error:	move.b	d0,CB_STAT(a6)
		bra	stt_error
err_tempo_val:	move.b	#RET_ERR_TEMPOVAL,CB_STAT(a6)
		bra	stt_error
err_no_control:	move.b	#RET_ERR_NOCONTROL,CB_STAT(a6)
		bra	stt_error

; A.Miyazawa	{
			.extern		com_set_status

;stt_error:	bsr	com_set_seqmode		; ///// Set seq MODE/STATUS /////
;		bra.w	ret_error		; ===== return to main =====
stt_error:	bsr	com_set_status		; ///// Set seq MODE/STATUS /////
		bra.w	ret_error		; ===== return to main =====

;		}

;====================================================================
;	PLAY_TO_PAUSE
;	FADE_TO_PAUSE
;	input	a4: Decoder work	top address
;		a5: Note ON Cue table	top address
;		a6: Sequence control block top address
;	output	nothing
;	break	d0-d7,a0-a6,flag
;====================================================================
	public	play_to_pause
	public	fade_to_pause

play_to_pause:
	move.b	#SQ_MODE_PLAYPAUSE,CB_MODE(a6)	; mode: play pause
	bra.b	pause				;

fade_to_pause:
	move.b	#SQ_MODE_FADEPAUSE,CB_MODE(a6)	; mode: fade pause

pause:	bset.b	#SQ_PAUSE_BITPOS,(a6)		; /////  control off  /////
	move.b	#0,d7				; 0: note off & control on
	bsr	com_seq_noteoff			; === all note off ===

	move.b	#RET_NORMAL,CB_STAT(a6)		; status: normal
	bsr	com_set_seqmode			; set seq mode
	clc
	rts


;====================================================================
;	PLAY_TO_STOP
;	FADE_TO_STOP
;	PLAYPAUSE_TO_STOP
;	FADEPAUSE_TO_STOP
;	input	a4: decoder work		top address
;		a5: note ON cue table		top address
;		a6: sequence control block	top address
;	output	nothing
;	break	d0-d7,a0-a6,flag
;====================================================================
	public	play_to_stop
	public	fade_to_stop
	public	playpause_to_stop
	public	fadepause_to_stop

play_to_stop:
fade_to_stop:
playpause_to_stop:
fadepause_to_stop:

; A.Miyazawa	{

		clr.w	up_down_flag(a6)
		clr.w	seq_volume_arrival(a6)
		clr.w	appointed_fade_rate(a6)
		clr.w	fade_count_variable(a6)
		move.w	#$0080,sequence_volume(a6)
		clr.w	performance_flag(a6)

;		}

	move.b	#$00,(a6)			; >>> control bit all off
	move.b	#1,d7				; 1:control off
	bsr	com_seq_noteoff			; ===== all note off =====

	move.b	#SQ_MODE_STOP,CB_MODE(a6)	; mode:   stop
	move.b	#RET_NORMAL,CB_STAT(a6)		; status: normal
	bsr	com_set_seqmode			; ===== set seq mode/status =====

	clc
	rts

;====================================================================
;	PLAYPAUSE_TO_PLAY
;	FADEPAUSE_TO_FADE
;	input	d0: Play Number(0-7)
;		a4: decoder work		top address
;		a5: note ON cue table		top address
;		a6: sequence control block	top address
;	output	nothing
;	break	d0-d7,a0-a6,flag
;====================================================================
	public	playpause_to_play
	public	fadepause_to_fade

playpause_to_play:
	move.b	#SQ_MODE_PLAY,CB_MODE(a6)	; mode: play
	bra.b	continue			;

fadepause_to_fade:
	move.b	#SQ_MODE_FADE,CB_MODE(a6)	; mode: fade

continue:
	bclr.b	#SQ_PAUSE_BITPOS,(a6)		; ===== pause bit off =====
	move.b	#RET_NORMAL,CB_STAT(a6)		; status: normal
	bsr	com_set_seqmode			; ///// set seq mode/status /////
	if	nao_chg
	else
	bra.w	ret_normal
	endif
	clc
	rts

;;====================================================================
;;	STOP_VOL_FADE
;;	PLAY_VOL_FADE
;;	FADE_VOL_FADE
;;	PLAYPAUSE_VOL_FADE
;;	FADEPAUSE_VOL_FADE
;;	input	d0.b: play number(0-7)
;;		d1.b: sequence volume(0-127)
;;		d2.b: fade rate(0-255)
;;		a4.l: decoder work	top address
;;		a5.l: note ON cue table	top address
;;		a6.l: sequence control block top address
;;	output	nothing
;;	break	d0-d7,a0-a6,flag
;;====================================================================
;	public	stop_vol_fade
;	public	play_vol_fade
;	public	fade_vol_fade
;	public	playpause_vol_fade
;	public	fadepause_vol_fade
;
;stop_vol_fade:
;	tst.b	d2			;
;	beq	stop_vol		; stop mode / volume
;	bra.w	stop_fade		; stop mode / fade
;play_vol_fade:
;	tst.b	d2			;
;	beq	play_vol		; play mode / volume
;	bra.w	play_fade		; play mode / fade
;fade_vol_fade:
;	tst.b	d2			;
;	beq	fade_vol		; fade mode / volume
;	bra.w	fade_fade		; fade mode / fade
;playpause_vol_fade:
;	tst.b	d2			;
;	beq	playpause_vol		; playpause mode / volume
;	bra.w	playpause_fade		; playpause mode / fade
;fadepause_vol_fade:
;	tst.b	d2			;
;	beq	fadepause_vol		; fadepause mode / volume
;	bra.w	fadepause_fade		; fadepause mode / fade
;;====================================================================
;;
;;	/////  sequence volume set process  /////
;;
;;====================================================================
;	public	stop_vol
;	public	play_vol
;	public	playpause_vol
;	public	fade_vol
;	public	fadepause_vol
;
;stop_vol:
;play_vol:
;playpause_vol:
;	setseqvol
;	bclr.b	#SQ_FADE_BITPOS,(a6)	; *** 95/2/17 add for FADE bug ***
;	move.b	d1,CB_SQ_VOL+0(a6)	; >>> set NEW sequence volume
;	move.b	#0,CB_SQ_VOL+1(a6)	; clesr low
;	if	nao_chg
;	else
;	bra.w	ret_normal		;
;	endif
;	clc
;	rts
;
;fade_vol:
;	setseqvol
;	bclr.b	#SQ_FADE_BITPOS,(a6)		; >>> fade bit OFF
;	move.b	#SQ_MODE_PLAY,CB_MODE(a6)	; mode: play
;	bra.w	vf100				;
;
;fadepause_vol:
;	setseqvol
;	bclr.b	#SQ_FADE_BITPOS,(a6)	; >>> fade bit OFF
;	move.b	#SQ_MODE_PLAYPAUSE,CB_MODE(a6)	; mode: playpause
;vf100:	move.b	#RET_NORMAL,CB_STAT(a6)	; status: normal
;	bsr	com_set_seqmode		; === set seq mode/status ===
;	move.b	d1,CB_SQ_VOL+0(a6)	; >>> set NEW sequence volume
;	move.b	#0,CB_SQ_VOL+1(a6)	; clesr low
;	if	nao_chg
;	else
;	bra.w	ret_normal		;
;	endif
;	clc
;	rts
;
;;====================================================================
;;
;;	/////  volume fade process  /////
;;
;;====================================================================
;	public	stop_fade
;	public	play_fade
;	public	playpause_fade
;	public	fade_fade
;	public	fadepause_fade
;
;playpause_fade:
;fadepause_fade:
;	if	nao_chg
;	else
;	bra.w	ret_normal		; no effect while PAUSE
;	endif
;	clc
;	rts
;
;fade_fade:
;	setseqvol
;	cmp.b	CB_SQ_VOL(a6),d1	; check new volume
;	beq	vf290			; no change?  yes
;	move.b	CB_SQ_VOL(a6),d0	; d0: CRNT volume
;	bra.w	vf210			; d1: NEW  volume
;
;stop_fade:
;	setseqvol
;	cmp.b	CB_SQ_VOL(a6),d1	; check new volume
;	beq	vf290			; no change?  yes
;	move.b	CB_SQ_VOL(a6),d0	; d0: CRNT volume
;	move.w	#SQ_SQVOL_MIN_W,CB_FD_VOL(a6)	; >>> fade volume initial
;	bra.w	vf210			;
;
;play_fade:
;	move.b	#SQ_MODE_FADE,CB_MODE(a6)	; mode: fade
;	move.b	#RET_NORMAL,CB_STAT(a6)	; status: normal
;	bsr	com_set_seqmode		; === set seq mode/status ===
;
;	setseqvol
;	cmp.b	CB_SQ_VOL(a6),d1	; check new volume
;	beq	vf290			; no change?  yes
;	move.b	CB_SQ_VOL(a6),d0	; sequence vol-->fade vol
;	move.b	d0,CB_FD_VOL+0(a6)	; >>> set FADE volume
;	move.b	#0,CB_FD_VOL+1(a6)	; clesr low
;
;vf210:	move.b	d1,CB_SQ_VOL+0(a6)	; >>> set new SEQUENCE volume
;	move.b	#0,CB_SQ_VOL+1(a6)	; clesr low
;	bsr	com_make_faderate	; === make fade rate ===
;	move.w	d2,CB_FD_RATE(a6)	; >>> set fade rate
;
;	bset.b	#SQ_FADE_BITPOS,(a6)	; >>> fade bit ON
;vf290:	
;	if	nao_chg
;	else
;	bra.w	ret_normal		;
;	endif
;	clc
;	rts
;
;====================================================================
;	SEQUENCE CONTROL
;	input	a4.l: Decoder work		top address
;		a5.l: Note ON Cue table		top address
;		a6.l: Sequence control block	top address
;	output	nothing
;	break	a0-a3
;====================================================================
	public	sequence_control
	public	get_next_tempo
;;	public	fade_control			; delete by A.Miyazawa
	public	seq_control
	public	get_next_event
	public	make_event
	public	out_event
	public	stop_sequence
	public	sqc900

sequence_control:
	move.b	(a6),d0			; #2  4	; bit7 = SQ_CTL_BITPOS
	bpl.w	ret_02				; Control bit OFF?  yes

	add.b	d0,d0			; #2  4	; bit6 = SQ_PAUSE_BITPOS
	bmi.w	ret_02				; Pause bit ON?  yes

; A.Miyazawa	{

		move.b	performance_flag(a6),d0
		cmpi.b	#%10000000,d0
		beq	stop_sequence			; sqc800:

;		}

	move.l	CB_CT_TIME(a6),d0	; d0=count value(of 2msec)
	sub.l	d0,CB_TP_TIMER(a6)	; ===== counter replace =====
	bcc	sqc200			; TEMPO counter over?  no

;
;	/////  GET next tempo  /////
;
get_next_tempo:

sqc100:	moveq.l	#DEC_TEMPO_CONT,d0	; mode: Tempo-Continue
	clr.l	d1			;
	move.b	CB_SQ_NUM(a6),d1	; Song Number(0-7)

	movem.l	a4/a5/a6,-(sp)		;
	bsr	sq_decoder		; ///// decoder call /////
	movem.l	(sp)+,a4/a5/a6		;
;;;	bsr	com_eventlog		; event log for DEBUG

	move.b	d0,d0			; DEC_RET_NORMAL=0 
	beq	sqc110			; jump if normal
	cmpi.b	#DEC_RET_LOOPBACK,d0	;
	bcs	sqc200			; jump if no data
	beq	sqc110			; jump if loop back
;	cmpi.b	#DEC_RET_NORMAL,d0	;
;	beq	sqc110			; normal?  yes
;	cmpi.b	#DEC_RET_LOOPBACK,d0	;
;	beq	sqc110			; loop back?  yes
;	cmpi.b	#DEC_RET_NODATA,d0	;
;	beq	sqc200			; no data?  yes
	bra.w	sqc_dec_error		; d0: 2-255(decoder error)

sqc110:	cmpi.l	#SQ_TVAL_MIN,d2		;
	bcs	sqc_tval_error		; under 200000(T300)?  yes
	cmpi.l	#SQ_TVAL_MAX+1,d2	;
	bcc	sqc_tval_error		; over 1500000(T40)?  yes

	move.l	d1,CB_TP_CNT(a6)	; >>> SET Tempo delta time
	move.l	d2,CB_TP_VAL(a6)	; >>> SET Tempo value(usec/beat)
	lsl.l	#8,d1			; delta time x 256
	add.l	d1,CB_TP_TIMER(a6)	; >>> REPLACE tempo counter

	move.w	CB_SQ_RESO(a6),d0	; d0=24-960
	divu.w	d0,d2			; d2--->208-62500
	move.l	#IPT_COUNT_BASE,d0	; d0=2000x256(512000)
	divu.w	d2,d0			; d0--->2461-8
	andi.l	#$0000ffff,d0		; clear rest(bit8-15)
	move.l	d0,CB_CT_TIME(a6)	; >>> SET 1 Count time
	move.l	d0,CB_CT_ORGTIME(a6)	; >>> SET 1 Count original time

;
;	/////  FADE control  /////
;
sqc200:

; A.Miyazawa	{
;;fade_control:
;;
;;	sqc200:	btst.b	#SQ_FADE_BITPOS,(a6)		; fade bit ON?
;;		beq	sqc250				; no
;;		subq.b	#1,CB_FD_CT(a6)			; counter -1
;;		bcc	sqc250				;
;;		move.b	#SQ_FD_WAIT,CB_FD_CT(a6)	; counter init(2x50=100ms)
;;		move.w	CB_SQ_VOL(a6),d0		; sequence volume
;;		move.w	CB_FD_VOL(a6),d1		; fade volume
;;		move.w	CB_FD_RATE(a6),d2		; fade rate
;;		cmp.w	d0,d1				; sequence : fade
;;		bcs	sqc220				; sequence < fade?  yes(fade out)
;;
;;	sqc210:	sub.w	d2,CB_FD_VOL(a6)		; /////  FADE IN  /////
;;		bcs	sqc230				; end of fade
;;		cmp.w	CB_FD_VOL(a6),d0		;
;;		beq	sqc230				; sequence = fade
;;		bcc	sqc230				; sequence < fade
;;		bra	sqc250				; sequence > fade
;;
;;	sqc220:	add.w	d2,CB_FD_VOL(a6)		; /////  FADE OUT  /////
;;		bcs	sqc225				; end of fade
;;		cmp.w	CB_FD_VOL(a6),d0		;
;;		beq	sqc225				; sequence = fade
;;		bcs	sqc225				; sequence > fade
;;		bra	sqc250				; sequence > fade
;;	sqc225:	cmpi.w	#SQ_SQVOL_MIN_W,d0		;
;;		beq	sqc240				; sequence volume=7F00h(MIN)?  yes
;;
;;	sqc230:	bclr.b	#SQ_FADE_BITPOS,(a6)		; /////  END OF FADE  /////
;;		move.b	#SQ_MODE_PLAY,CB_MODE(a6)	; mode:   play
;;		move.b	#RET_NORMAL,CB_STAT(a6)		; status: normal
;;		bsr	com_set_seqmode			; === set seq mode/status ===
;;		bra	sqc250				;
;;
;;	sqc240:	move.b	#$00,(a6)			; /////  STOP SEQUENCE  /////
;;		move.b	#1,d7				; 1:control off
;;		bsr	com_seq_noteoff			; === all note off ===
;;		move.b	#0,CB_NT_CNT(a6)		; >>> Note ON counter clear
;;		bra.w	sqc800				; goto STOP sequence

;		}

;
;	/////  SEQUENCE control  /////
;
seq_control:

sqc250:	subq.b	#1,CB_PTR_CT(a6)	; wait counter -1
	bcc	sqc260			; 100ms wait?  no
	move.b	#SQ_PTR_WAIT,CB_PTR_CT(a6)	; counter init(2x50=100ms)
	movea.l	CB_PTR_ADRS(a6),a0	; a0: sequence pointer address
	addq.w	#1,(a0)			; 100ms counter +1

sqc260:	btst.b	#SQ_EMPTY_BITPOS,(a6)	;
	bne.w	ret_02			; Empty bit ON?  yes
	move.l	CB_CT_TIME(a6),d0	; d0=count value(of 2msec)
	sub.l	d0,CB_SQ_TIMER(a6)	;
	beq	sqc270			; >>> timer = 0
	bcc.w	ret_02			; >>> wait timer overflow

sqc270:	move.b	CB_EV_COM(a6),d0	; MIDI command
	move.b	CB_EV_S1(a6),d1		; MIDI data 1
	move.b	CB_EV_S2(a6),d2		; MIDI data 2
	move.b	CB_EV_PORT(a6),d3	; MIDI port#
	bsr	com_eventout		; ///// OUTPUT wait event /////

	cmpi.b	#MIDI_COM_NON,d0	; d0=MIDI command
	bne	sqc300			; NOTE ON command?  no
	move.b	CB_EV_COM(a6),d0	; MIDI command
	move.l	CB_EV_GATE(a6),d4	; Gate time
	add.l	CB_SQ_TIMER(a6),d4	; >>>>> adjust gate time
	bsr	com_notecue_set		; ///// SET note on cue /////
	addq.b	#1,CB_NT_CNT(a6)	; ===== Note ON counter +1 =====

;
;	/////  GET next event  /////
;
get_next_event:

sqc300:	moveq.l	#DEC_SONG_CONT,d0	; mode: Song-Continue
	clr.l	d1			;
	move.b	CB_SQ_NUM(a6),d1	; Song Number(0-7)

	movem.l	a4/a5/a6,-(sp)		;
	bsr	sq_decoder		; ///// decoder call /////
	movem.l	(sp)+,a4/a5/a6		;
;;;	bsr	com_eventlog		; event log for DEBUG

	cmpi.b	#DEC_RET_NORMAL,d0	;
	beq	sqc400			; normal?  yes
	cmpi.b	#DEC_RET_NODATA,d0	;
	beq	sqc310			; no data?  yes
	bra.w	sqc_dec_error		; d0: 2-255(decoder error)

sqc310:	bset.b	#SQ_EMPTY_BITPOS,(a6)	; ===== SET empty bit =====
	tst.b	CB_NT_CNT(a6)		;
	beq	sqc800			; note on=0
	bra.w	ret_02			; note on=1-32

;
;	/////  make event  /////
;
make_event:

sqc400:	andi.b	#11110000b,d3		;
	move.b	d3,CB_EV_COM(a6)	; MIDI command
	move.b	d4,CB_EV_S2(a6)		; MIDI data 2
	swap	d4			;
	move.b	d4,CB_EV_S1(a6)		; MIDI data 1

	lsr.w	#8,d0			; xx
	move.w	d0,d3			; xx
	andi.b	#00001111b,d0		; xx
	or.b	d0,CB_EV_COM(a6)	; xx
	lsr.w	#4,d3			; xx
	andi.b	#00000001b,d3		; xx
	move.b	d3,CB_EV_PORT(a6)	; xx

	lsl.l	#8,d2			; gate  x 8
	move.l	d2,CB_EV_GATE(a6)	; >>> SET gate Time
	lsl.l	#8,d1			; delta x 8
	add.l	d1,CB_SQ_TIMER(a6)	; >>> REPLACE sequence counter
	beq	sqc600			; ZERO: output
	bpl.w	ret_02			; PLUS: wait

;
;	/////  OUTPUT event  /////
;
out_event:

sqc600:	move.b	CB_EV_COM(a6),d0	; MIDI command
	move.b	CB_EV_S1(a6),d1		; MIDI data 1
	move.b	CB_EV_S2(a6),d2		; MIDI data 2
	move.b	CB_EV_PORT(a6),d3	; MIDI port#
	bsr	com_eventout		; ///// OUTPUT wait event /////

	cmpi.b	#MIDI_COM_NON,d0	; d0=MIDI command
	bne	sqc610			; [ NOTE ON ] command?  no
	move.b	CB_EV_COM(a6),d0	; MIDI command
	move.l	CB_EV_GATE(a6),d4	; Gate time
	add.l	CB_SQ_TIMER(a6),d4	; >>>>> adjust gate time
	bsr	com_notecue_set		; ///// SET note on cue /////
	addq.b	#1,CB_NT_CNT(a6)	; ===== Note ON counter +1 =====
sqc610:	bra	sqc300			; ===== Get NEXT event =====

;
;	/////  STOP sequence  /////
;
stop_sequence:

sqc800:	move.b	#RET_NORMAL,CB_STAT(a6)		; status=normal
sqc850:	move.b	#SQ_MODE_STOP,CB_MODE(a6)	; MODE=STOP
	move.b	#$00,(a6)			; ///// Control bit all OFF /////
	bsr	com_set_seqmode			; ///// set seq mode /////

; A.Miyazawa	{

		clr.b	performance_flag(a6)

;		}

sqc900:
ret_02:	rts

;
;	/////  Error Handle  /////
;
sqc_dec_error:	move.b	d0,CB_STAT(a6)		;
		bra	sqc_error		;
sqc_tval_error:	move.b	#RET_ERR_TEMPOVAL,CB_STAT(a6)
		bra	sqc_error		;
sqc_error:
		move.b	#1,d7			; 1:control OFF
		bsr	com_seq_noteoff		; ///// All note OFF /////
		bra.b	sqc850			;

;====================================================================
;	NOTE ON CUE CONTROL
;	input	a4.L: Decoder work		top address
;		a5.L: Note ON Cue table		top address
;		a6.L: Sequence control block	top address
;	output	nothing
;	break	a0-a3,a6,d0-d3
;====================================================================
;@	public	noteoncue_control

noteoncue_control:
	btst.b	#NC_CTL_BITPOS,(a5)	;
	beq	ntc900			; control bit OFF?  yes
	movea.l	NC_PTR(a5),a6		; a6: control block address
	btst.b	#SQ_PAUSE_BITPOS,(a6)	;
	bne	ntc900			; Pause bit ON?  yes
	move.l	CB_CT_TIME(a6),d0	; d0=count value(of 2msec)
	sub.l	d0,NC_CNT(a5)		; ===== counter replace =====
	beq	ntc100			; >>> time = 0
	bpl	ntc900			; >>> wait time over flow

;
;	/////  [ NOTE OFF ] event output  /////
;
ntc100:	move.b	NC_CMD(a5),d0		; MIDI command
	move.b	NC_S1(a5),d1		; MIDI data 1
	move.b	NC_S2(a5),d2		; MIDI data 2
	move.b	NC_CTL(a5),d3		; MIDI port#
	andi.b	#NC_PORT_MASK,d3	; port# mask
	andi.b	#MIDI_COM_CHMASK,d0	; channel mask
	ori.b	#MIDI_COM_NOFF,d0	; SET [NOTE OFF] command
	bsr	com_eventout		; ///// MIDI event output /////

	bclr.b	#NC_CTL_BITPOS,(a5)	; ===== Contrl Bit OFF =====
	subq.b	#1,CB_NT_CNT(a6)	; ===== Note ON counter -1 =====
	bne	ntc900			;
	btst.b	#SQ_EMPTY_BITPOS,(a6)	; ===== test empty bit =====
	beq	ntc900			;

;
;	/////  STOP sequence  /////
;
	move.b	#$00,(a6)		; ===== control bit all OFF =====
	move.b	#SQ_MODE_STOP,CB_MODE(a6)	; mode:   stop
	move.b	#RET_NORMAL,CB_STAT(a6)	; status: normal
	bsr	com_set_seqmode		; ///// set seq mode /////
ntc900:	rts

;====================================================================
;	VOLUME ANALIZE
;	/////  CD-DA input level analize  /////
;	input	a4.L: Decoder work		top address
;		a5.L: Note ON Cue table		top address
;		a6.L: Sequence control block	top address
;	output	nothing
;	break	a0-a3,d0-d7
;====================================================================
	public	volume_analize

volume_analize:
	if	nao_chg
	else
	btst.b	#SY_VANCTL_BITPOS,SY_TVOLCTL(a4)	; check volume 
	beq	va900				;		analize bit
	endif

	subq.b	#1,SY_TVOLCNT(a4)	; #4,16	; counter -1
	bcc.w	ret_100				;
	move.b	#SY_VANWAIT_CNT-1,SY_TVOLCNT(a4)	; counter init(2msx8=16ms)

	movea.l	Mem_HOSTIF_PTR,a2		; host I/F work top(700h)
	movea.l	#SCSP_EXTS_L,a0			; EXTS Lch address
	lea	OF_HI_TVOLL(a2),a1		; average area
	bsr	com_volanalize			;
	movea.l	#SCSP_EXTS_R,a0			; EXTS Rch address
	lea	OF_HI_TVOLR(a2),a1		; average area
	bsr	com_volanalize			;

	movea.l	#SCSP_EFREG9,a0			; --- High band ---
	lea	OF_HI_HVOLL(a2),a1		; average area
	bsr	com_volanalize			; set Lch
	move.w	d0,OF_HI_HVOLR(a2)		; set Rch
	movea.l	#SCSP_EFREG10,a0		; --- Mid band ---
	lea	OF_HI_MVOLL(a2),a1		; average area
	bsr	com_volanalize			; set Lch
	move.w	d0,OF_HI_MVOLR(a2)		; set Rch
	movea.l	#SCSP_EFREG11,a0		; --- Low band ---
	lea	OF_HI_LVOLL(a2),a1		; average area
	bsr	com_volanalize			; set Lch
	move.w	d0,OF_HI_LVOLR(a2)		; set Rch
ret_100:
	rts

;====================================================================
;	PCM play address search
;	/////  PCM stream play address search  /////
;	input	a4.L: Decoder work		top address
;		a5.L: Note ON Cue table		top address
;		a6.L: Sequence control block	top address
;	output	nothing
;	break	a0-a3,d0-d7
;====================================================================
	public	pcm_address_search

pcm_address_search:
	subq.b	#1,SY_PCMCNT(a4)		; counter -1
	bcc	pas20				;
	move.b	#SY_PCMWAIT_CNT-1,SY_PCMCNT(a4)	; counter init(2msx8=16ms)
;;;
;;;	bsr	pcm_address_log			; ***** FOR DEBUG *****
;;;
	movea.l	Mem_bs_PWKTP,a0			; a0: 68K Work Top address
	adda.l	#Pcm_Strm,a0			;     slot read area top
	movea.l	Mem_HOSTIF_PTR,a1		; a1: host I/F work top(700h)
	adda.l	#OF_HI_PCM0,a1			; play address set area top
	moveq.l	#0,d0				; d0: PCM play number initial
	moveq.l	#0,d1				; d1: bit map initial

	moveq.l	#PCM_MAX_NUM-1,d7		; MAX 8 PCM play(d7=7)
pas10:	bsr	com_pcm_control			; === SET play address ===
	adda.l	#2,a0				; next READ  area address
	adda.l	#2,a1				; next WRITE area address
	addq.b	#1,d0				; next PCM   play number
	dbra.w	d7,pas10			;

	movea.l	Mem_IFWK_PTR,a0			; a0: system IFwork top(480h)
	btst.b	#MCI_EN_BITPOS,OF_IPT_CTRL(a0)	; check interrupt enable bit
	beq	pas20				; bit7=0?  yes
	tst.b	d1				; check address up BIT map
	beq	pas20				; no change?  yes

	move.b	d1,OF_PCM_PLAYNUM(a0)		; >>> set play number bit map
	move.b	#MCI_MODE_PCM,OF_IPT_MODE(a0)	; >>> set interrupt mode(01h)
	lea	IO_SCSP,a1			; a1: SCSP register top
	move.w	#$20,RG_MCIEB(a1)		; >>>>> interrupt enable
	nop					; wait
	move.w	#$20,RG_MCIPD(a1)		; >>>>> interrupt request ON
pas20:	rts

;=======[ end of sddrv.asm ]=========================================
