;====================================================================
;       /////  SATURN Sound Driver  /////
;       [ System debug module : SDDBG.ASM ] Program Source
;       Ver 1.28 1994/12/13
;       DM/SOUND K.Fujishima
;====================================================================

;====================================================================
;       DELTA TIME to COUNT TIME
;       Function  [ Exchange delta time to count time(usec) ]
;       Input     d0: Delta time(24bit)
;                 d1: 1 count time(16bit: 208-62500)
;       Output    d0: Total count time(32bit)
;       Break     nothing
;====================================================================
        public  com_delta_count

com_delta_count:
        movem.l   d1/d2,-(sp)           ; Push Register
        move.l    d1,d2                 ; copy to D2
        mulu.w    d0,d1                 ; low word --> D1
        swap      d0                    ;
        mulu.w    d0,d2                 ; high word --> D2
        swap      d2                    ; shift 16bit(d2x65536)
        clr.w     d2                    ;
        add.l     d2,d1                 ; Make Total(High+Low)
        move.l    d1,d0                 ; Set to D0
        movem.l   (sp)+,d1/d2           ; Pop Register
dct10:  rts

;====================================================================
;       GET MAP DATA
;       Function [ get sound area map data ]
;       Input   d0.b: data ID & ID number
;                     0xh - tone bank data (x=0-F)
;                     1xh - sequence data  (x=0-F)
;                     2xh - DSP program    (x=0-F)
;                     3xh - DSP work RAM   (x=0-F)
;       Output  a0.l: map data top address
;               d1.l: area start address (d1=0: ID not found)
;               d2.l: area size
;       Break   nothing
;====================================================================
        public  com_get_mapdata

com_get_mapdata:
        movea.l   MEM_MAPCRNT_PTR,a0      ; a0: sound area map CRNT work
        clr.l     d1                      ; 0=not found
gmd10:  btst.b    #MAP_END_BITPOS,(a0)    ;
        bne       gmd90                   ; end bit ON?  yes
        cmp.b     (a0),d0                 ;
        beq       gmd20                   ; ID match?  yes
        addq.l    #MAP_SIZE,a0            ; next address
        bra.b     gmd10                   ;
gmd20:  move.l    MAP_DATA_ID(a0),d1      ; get address data
        move.l    MAP_DATA_LBIT(a0),d2    ; get size data
        andi.l    #MAP_ADRS_MASK,d1       ; mask d1=00000-FFFFF
        andi.l    #MAP_SIZE_MASK,d2       ; mask d2=00000-FFFFF
gmd90:  rts

;====================================================================
;       GET PRIORITY INFORMATION
;       Function  [ check current priority and get priority information ]
;       Input     a4: decoder work           top address
;                 a5: note on cue table      top address
;                 a6: sequence control block top address
;       Output    d0: 0-7   MAX priority level
;                     FFh   no active sequence
;                 d1: 0-7   sequence priority level
;       Break     
;====================================================================
        public    com_get_priority

com_get_priority:
        movem.l   a4/d7,-(sp)             ; save
        adda.l    #SQ_TOP,a4              ; sequence control area top address
        move.w    #SQ_NUM-1,d7            ; max 8 sequence
        move.b    #$ff,d0                 ; priority initial
gpr100: move.b    (a4),d1                 ;
        andi.b    #$e0,d1                 ; mask CTL/PS/MT bit
        cmpi.b    #$80,d1                 ;
        bne       gpr200                  ; CTLon/PSoff/MToff ?  no
        cmp.b     CB_PR_LEV(a4),d0        ; compare priority(d0-src)
        bcs       gpr200                  ;
        move.b    CB_PR_LEV(a4),d0        ; ===== get MAX priority =====
gpr200: adda.l    #SQ_SIZ,a4              ; next address
        dbra.w    d7,gpr100               ;
        move.b    CB_PR_LEV(a6),d1        ; ===== get sequence priority =====
        movem.l   (sp)+,a4/d7             ; restore
        rts

;====================================================================
;       MIDI EVENT LOG for debug
;       Function [ log MIDI event to SOUND MEMORY ]
;       Input   a4:decoder work top address
;               d0:status
;               d1:delta time
;               d2:gate time
;               d3:MIDI command
;               d4:MIDI data1/2
;       Output  nothing
;       Break   nothing
;====================================================================
        public  com_eventlog

com_eventlog:
        btst.b    #0,(a4)               ;
        beq       evl90                 ; end bit ON?  yes
        movem.l   a0,-(sp)              ; save
        movea.l   16(a4),a0             ; get data set pointr
        move.w    d0,0(a0)              ; status
        move.l    d1,4(a0)              ; delta time
        move.l    d2,8(a0)              ; gate time
        move.b    d3,12(a0)             ; MIDI command
        move.b    d4,14(a0)             ; MIDI data 1
        swap      d4                    ;
        move.b    d4,13(a0)             ; MIDI data 2
        swap      d4                    ;
        add.l     #16,16(a4)            ; pointer up
        movem.l   (sp)+,a0              ; restore
evl90:  rts

;====================================================================
;       HOST COMMAND LOG for debug
;       Function [ log host command to SOUND MEMORY ]
;       Input   a0: command data top address
;       Output  nothing
;       Break   nothing
;====================================================================
        public  com_commandlog

com_commandlog:
        movem.l   a0/a1/a2,-(sp)        ; save
        movea.l   MEM_68KWORK_ADRS,a1   ; 68K Work Top address
        adda.l    #DEC_WORK_OFFSET,a1   ; Decoder Work Top address
        movea.l   #$800,a2              ; base=0800h
        adda.l    20(a1),a2             ; add pointer offset
        add.l     #16,20(a1)            ; pointer up
        cmp.l     #$800,20(a1)          ; area end?
        bne       cml10                 ;
        move.l    #0,20(a1)             ; pointer offset initial
cml10:  move.l    (a0)+,(a2)+           ; command data transfer
        move.l    (a0)+,(a2)+           ;
        move.l    (a0)+,(a2)+           ;
        move.l    (a0)+,(a2)+           ;
        movem.l   (sp)+,a0/a1/a2        ; restore
        rts

;====================================================================
;       GET MAP ADDRESS
;       Function [ get sound area map address ]
;       Input   d0: map number(0-255)
;       Output  a0: sound area map address
;       Break   nothing
;====================================================================
        public  com_get_mapaddress

com_get_mapaddress:
        move.l    d0,-(sp)                ;
        movea.l   MEM_MAPDATA_ADRS,a0     ; a0: sound area map top address
        tst.b     d0                      ;
        beq       gm_ret                  ; first map(d0=0)?  yes
gm_tst: btst.b    #MAP_END_BITPOS,(a0)    ;
        bne       gm_end                  ; data end bit ON?  yes
        adda.l    #MAP_SIZE,a0            ; next map address
        bra       gm_tst                  ;
gm_end: adda.l    #1,a0                   ; a0=next map data address
        subq.b    #1,d0                   ; map counter -1
        bne       gm_tst                  ; count end(=0)?  no
gm_ret: move.l    (sp)+,d0                ;
        rts

;====================================================================
;       PCM play address logging
;       /////  PCM stream play address logging  /////
;       input     a4.L: Decoder work           top address
;                 a5.L: Note ON Cue table      top address
;                 a6.L: Sequence control block top address
;       output    nothing
;       break     d0-d2
;====================================================================
        public    pcm_address_log

pcm_address_log:
        movem.l   a0/a1/d0/d7,-(sp)
        movea.l   #$007a0,a0
        movea.l   #$f0000,a1
        move.w    #8-1,d7
        move.w    16(a4),d0
al10:   move.b    (a0),(a1,d0.w)
        adda.l    #2,a0
        add.w     #1,d0
        dbra      d7,al10
        move.w    d0,16(a4)
        movem.l   (sp)+,a0/a1/d0/d7
        rts
