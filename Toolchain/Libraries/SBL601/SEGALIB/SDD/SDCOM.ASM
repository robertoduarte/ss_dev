;====================================================================
;	/////  SATURN Sound Driver  /////
;	[ System Common Subroutine : SDCOM.ASM ] Program Source
;	Ver 1.28 1994/12/13
;	DM/SOUND K.Fujishima
;====================================================================
	include	SCSP.LIB	;sddrv.inc

;====================================================================
;	GET WORK ADDRESS
;	Function [ get decoder work address ]
;	Input	nothing
;	Output	a4: decoder work		top address
;		a5: note ON Cue table		top address
;		a6: sequence control block	top address
;	Break	nothing
;====================================================================
	public	com_get_workaddress

com_get_workaddress:
	movea.l	Mem_bs_PWKTP,a4		; A4: 68K Work Top address
	adda.l	#DEC_WORK_OFFSET,a4	; A4: Decoder Work Top address
	movea.l	a4,a5			; copy to a5/a6
	movea.l	a4,a6			;
	adda.l	#NC_TOP,a5		; A5: Note ON Cue table top address
	adda.l	#SQ_TOP,a6		; A6: Sequence control	top address
	rts

;====================================================================
;	SEQUENCE ALL NOTE OFF
;	Function  [ All note off output (inside 1 sequence) ]
;	Input	d7.B: 0=note OFF only
;		      1=note OFF and  control OFF
;		a5.L: note on cue table top address
;		a6.L: sequence control block top address
;	Output	nothing
;	Break	a0-a2/d0-d3
;====================================================================
	public	com_seq_noteoff

com_seq_noteoff:
	movem.l	a5/d6,-(sp)		; save
	move.w	#NC_NUM-1,d6		; max 32 cue
snf100:	btst.b	#NC_CTL_BITPOS,(a5)	;
	beq	snf300			; control bit OFF?  yes
	cmpa.l	NC_PTR(a5),a6		;
	bne	snf300			; sequence MATCH?  no
	tst.b	d7			; 1: note OFF & control OFF
	beq	snf200			;
	bclr.b	#NC_CTL_BITPOS,(a5)	; ===== Contrl Bit OFF =====
snf200:	move.b	NC_CMD(a5),d0		; MIDI command
	move.b	NC_S1(a5),d1		; MIDI data 1
	move.b	NC_S2(a5),d2		; MIDI data 2
	move.b	NC_CTL(a5),d3		; MIDI port#
	andi.b	#NC_PORT_MASK,d3	; port# mask
	andi.b	#MIDI_COM_CHMASK,d0	; channel mask
	ori.b	#MIDI_COM_NOFF,d0	; SET [NOTE OFF] command
	bsr	com_eventout		; ///// NOTE OFF event output /////
snf300:	adda.l	#NC_SIZ,a5		; next address
	dbra.w	d6,snf100		; cue end?  no
	movem.l	(sp)+,a5/d6		; restore
	rts

;====================================================================
;	SEQUENCE ALL NOTE ON
;	Function  [ All note on output (inside 1 sequence) ]
;	Input	a5.L: note on cue table top address
;		a6.L: sequence control block top address
;	Output	nothing
;	Break	a0-a2/d0-d3
;====================================================================
	public	com_seq_noteon

com_seq_noteon:
	movem.l	a5/d6,-(sp)		; save
	move.w	#NC_NUM-1,d6		; max 32 cue
sno100:	btst.b	#NC_CTL_BITPOS,(a5)	;
	beq	sno900			; control bit OFF?  yes
	cmpa.l	NC_PTR(a5),a6		;
	bne	sno900			; sequence MATCH?  no
sno200:	move.b	NC_CMD(a5),d0		; MIDI command
	move.b	NC_S1(a5),d1		; MIDI data 1
	move.b	NC_S2(a5),d2		; MIDI data 2
	move.b	NC_CTL(a5),d3		; MIDI port#
	andi.b	#NC_PORT_MASK,d3	; port# mask
	andi.b	#MIDI_COM_CHMASK,d0	; channel mask
	ori.b	#MIDI_COM_NON,d0	; SET [NOTE ON] command
	bsr	com_eventout		; ///// NOTE ON event output /////
sno900:	adda.l	#NC_SIZ,a5		; next address
	dbra.w	d6,sno100		; cue end?  no
	movem.l	(sp)+,a5/d6		; restore
	rts

;====================================================================
;	SET HOST INTERFACE
;	Function  [ Set sequence MODE/STATUS to host I/F work ]
;	Input	a6: Sequence control block top address
;	Output	nothing
;	Break	nothing
;====================================================================
	public	com_set_seqmode

com_set_seqmode:
	movem.l	a0/d0/d1/d2,-(sp)	; Push Register

	movea.l	Mem_HOSTIF_PTR,a0	; a0: host I/F work top address(700h)
	lea	OF_HI_STAT1(a0),a0	; a0: status area top address

	move.b	CB_MODE(a6),d0		; D0: mode
	move.b	CB_STAT(a6),d1		; D1: status
	clr.l	d2			; clear D2
	move.b	CB_SQ_NUM(a6),d2	; Get play number(0-7)
	lsl.w	#1,d2			; D2 x 2(0-14)
	move.b	d0,0(a0,d2.w)		; >>> SET Mode   to Host I/F work
	move.b	d1,1(a0,d2.w)		; >>> SET Status to Host I/F work
	movem.l	(sp)+,a0/d0/d1/d2	; Pop Register
	rts

;====================================================================
;	NOTE ON CUE SET
;	Function [ set NoteOn event to NoteOn cue ]
;	Input	d0: MIDI command
;		d1: MIDI data1
;		d2: MIDI data2
;		d3: port# (0=portA 1=portB)
;		d4: gate time (usec)
;		a5: note on cue table	top address
;		a6: sequence control block top address
;	Output	CY: 0=normal end
;		    1=error (cue full)
;	Break	a0
;====================================================================
	public	com_notecue_set

com_notecue_set:
	movem.l	a5/d7,-(sp)		; push register
	moveq.l	#NC_NUM,d7		; max 32 cue
	int_di				; ===== interrupt DISABLE =====
	sr_push
ncs10:	btst.b	#NC_CTL_BITPOS,(a5)	;
	beq	ncs30			; empty(Bit=0)?  no
	adda.l	#NC_SIZ,a5		; set next cue address
	subq.b	#1,d7			;
	bne	ncs10			; cue end?  no
	stc				; CY=1: error
ncs20:	sr_pop				; ===== interrupt ENABLE =====
	movem.l	(sp)+,a5/d7		; pop register
	rts

ncs30:	move.b	d0,NC_CMD(a5)		; set MIDI command
	move.b	d1,NC_S1(a5)		; set MIDI data 1
	move.b	d2,NC_S2(a5)		; set MIDI data 2
	move.b	d3,NC_CTL(a5)		; set port number
	move.l	d4,NC_CNT(a5)		; set gate Time
	move.l	a6,NC_PTR(a5)		; set control block pointer
	bset.b	#NC_CTL_BITPOS,(a5)	; ===== contrl bit ON =====
	clc				; CY=0: normal end
	bra	ncs20

;====================================================================
;	MIDI EVENT OUTPUT
;	Function [ set MIDI event to driver's buffer ]
;	Input	d0.B: MIDI command
;		d1.B: MIDI data1
;		d2.B: MIDI data2
;		d3.B: Port# (0=PortA 1=PortB)
;		a4.L: Decoder work		top address
;		a5.L: Note ON Cue table	top address
;		a6.L: sequence control block top address
;		CB_SQ_NUM(a6): play numver(0-7)
;	Output	d0.B: MIDI command (xxxx0000b)
;	Break	a0-a2
;====================================================================
	public	com_eventout

com_eventout:
	movem.l	d5/d6/d7,-(sp)		; push register
	movea.l	Mem_bs_PWKTP,a0		; 68K Work Top address
	lea	_MIDI_RCV_WRPT(a0),a1	; a1:
	lea	_MIDI_RCV_BF(a0),a2	; a2:
	move.b	d0,d5			; save ORIGINAL command data to d5
	move.b	d0,d6			; save ORIGINAL command data to d6

	andi.b	#MIDI_COM_MASK,d0	; mask MIDI command(=ret status)
	cmpi.b	#MIDI_COM_NON,d0	; note ON command?
	bne	eo20			;
	move.b	CB_SQ_VOL(a6),d7	; /////  fade control  /////
	btst.b	#SQ_FADE_BITPOS,(a6)	;
	beq	eo10			; fade bit ON?  no
	move.b	CB_FD_VOL(a6),d7	; d7: fade volume
eo10:	sub.b	d7,d2			;
	bcc	eo20			; overflow?  no
	clr.b	d2			; velocity=0

eo20:	lsr.b	#4,d5			; /////  make 4byte MIDI  /////
	andi.b	#M4_COM_MASK,d5		; d5=4B MIDI command word
	andi.b	#MIDI_COM_CHMASK,d6	;
	lsl.b	#4,d3			;
	or.b	d3,d6			; d6=4B MIDI channel word
	move.b	CB_SQ_NUM(a6),d7	; d7=play number(0-7)
	lsl.b	#5,d7			; bit adjust
	or.b	d7,d6			; >>> d6: playnumber+MIDIch

	clr.l	d7			;
	sr_push
	int_di				; ===== Interrupt DISABLE =====
	move.b	(a1),d7			; Get data pointer
	addq.b	#1,(a1)			; restore data pointer
	add.w	d7,d7			; pointer x4
	add.w	d7,d7			;
	adda.l	d7,a2			;

	move.b	d5,(a2)+		; MIDI command set
	move.b	d6,(a2)+		; MIDI channel set
	move.b	d1,(a2)+		; MIDI data1 set
	move.b	d2,(a2)+		; MIDI data2 set

	sr_pop				; ===== Interrupt ENABLE =====
eo40:
	movem.l	(sp)+,d5/d6/d7		; pop register
	rts

;====================================================================
;	CD-DA Volume analize
;	Function [ Analize CD-DA input/3band level ]
;	Input	a0.L: analize data SCSP REGISTER address
;		a1.L: analize data set area address
;	Output	d0.W: Volume average data(0000-7FFF)
;	Break	nothing
;====================================================================
	public	com_volanalize

com_volanalize:
	movem.l	d1,-(sp)		; save register
	move.w	(a0),d0			; d0=16bit PCM data
	tst.w	d0			;
	bpl	van10			; d0=0000-7FFF(new data)
	neg.w	d0			;
van10:	move.w	(a1),d1			; d1=0000-7FFF(total average)
	add.w	d1,d0			;
	lsr.w	#1,d0			; make new TOTAL average
	move.w	d0,(a1)			; >>>>> set to work
	movem.l	(sp)+,d1		; restore register
	rts

;====================================================================
;	PCM stream play address search
;	Function [ Set pcm play address & Set host interrupt ]
;	Input	d0.l: PCM play number(0-7)
;		d1.l: PCM play address up BIT map
;		a0.l: PCM play slot number READ  area address
;		a1.l: PCM play address     WRITE area address
;	Output	d1.l: PCM play address up BIT map
;	Break	a2
;====================================================================
	public	com_pcm_control

com_pcm_control:
	btst.b	#PCM_CTL_BITPOS,(a0)	; check
	bne	cpc20			;
	move.b	#0,(a1)			; play address clear
cpc10:	rts

cpc20:	movea.l	#SCSP_DBG_REG,a2	; a2: debug register address(100408h)
	move.b	(a0),d2			;
	andi.b	#RG_SLOT_MASK,d2	; d2: slot#(0-31)
	lsl.w	#8,d2			;
	lsl.w	#3,d2			; bit15-11: MSLC(slot select)
	move.w	d2,(a2)			; WRITE  slot select

	move.w	#14,d2			; 94.10.13 wait SCSP 1 sycle(22usec)
cpc25:	nop				; 94.10.3
	dbra	d2,cpc25		; 94.10.3

	move.w	(a2),d2			; READ   CA/SGC/EG data
	lsr.w	#7,d2			;
	andi.b	#RG_CA_MASK,d2		; d2=PCM play address(0-15)
	cmp.b	(a1),d2			;
	beq	cpc10			; address no change?  yes
	move.b	d2,(a1)			; >>>>> set NEW play address
	bset.l	d0,d1			; >>>>> set BIT map
	bra	cpc10			; return

;====================================================================
;	MAKE FADE RATE
;	Function [ make new fade rate ]
;	Input	d0.b: CRNT volume (0-127)
;		d1.b: NEW  volume (0-127)
;		d2.b: fade rate   (0-255)
;	Output	d2.w: fade rate   (1-7fff)
;	Break	nothing
;====================================================================
	public	com_make_faderate

com_make_faderate:
	movem.l	d0/d1,-(sp)		; save
	cmp.b	d0,d1			; d1-d0
	bcc	mf10			; d1>d0?  yes
	sub.b	d1,d0			; d0=1-127
	move.b	d0,d1			;
	bra.b	mf20			;
mf10:	sub.b	d0,d1			; d1=1-127
mf20:	lsl.w	#8,d1			; byte-->word(x256)
	andi.l	#$7fff,d1		;
	andi.w	#$00ff,d2		; d2=1-255
	divu.w	d2,d1			; make fade rate
	move.w	d1,d2			;
	movem.l	(sp)+,d0/d1		; restore
	rts

;=======[ end of sdcom.asm ]=========================================
