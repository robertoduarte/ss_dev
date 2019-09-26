rem @echo off

ECHO bup
IF EXIST bup\*.obj GOTO bup
cd bup
call mk
cd ..
:bup

ECHO csh
IF EXIST csh\*.obj GOTO csh
cd csh
call mk
cd ..
:csh

ECHO dbg\smpdbg1
IF EXIST dbg\smpdbg1\*.obj GOTO smpdbg1
cd dbg\smpdbg1
call mk
cd ..\..
:smpdbg1

ECHO dma\smpdma0
IF EXIST dma\smpdma0\*.obj GOTO smpdma0
cd dma\smpdma0
call mk
cd ..\..
:smpdma0

ECHO dma\smpdma1
IF EXIST dma\smpdma1\*.obj GOTO smpdma1
cd dma\smpdma1
call mk
cd ..\..
:smpdma1

ECHO dma\smpdma10
IF EXIST dma\smpdma10\*.obj GOTO smpdma10
cd dma\smpdma10
call mk
cd ..\..
:smpdma10

ECHO dma\smpdma12
IF EXIST dma\smpdma12\*.obj GOTO smpdma12
cd dma\smpdma12
call mk
cd ..\..
:smpdma12

ECHO dma\smpdma2
IF EXIST dma\smpdma2\*.obj GOTO smpdma2
cd dma\smpdma2
call mk
cd ..\..
:smpdma2

ECHO dual
IF EXIST dual\*.obj GOTO dual
cd dual
call mk
cd ..
:dual

rem goto SKIPGFS
ECHO gfs\smpgfs1a
IF EXIST gfs\smpgfs1a\*.obj GOTO gfs_smp1a
cd gfs\smpgfs1a
call mk
cd ..\..
:gfs_smp1a

ECHO gfs\smpgfs1b
IF EXIST gfs\smpgfs1b\*.obj GOTO gfs_smp1b
cd gfs\smpgfs1b
call mk
cd ..\..
:gfs_smp1b

ECHO gfs\smpgfs2
IF EXIST gfs\smpgfs2\*.obj GOTO gfs_smp2
cd gfs\smpgfs2
call mk
cd ..\..
:gfs_smp2

:SKIPGFS
ECHO mem
IF EXIST mem\*.obj GOTO mem
cd mem\smpmem0
call mk
cd ..\..
cd mem\smpmem1
call mk
cd ..\..
:mem

ECHO mpg\smpmpg1
IF EXIST mpg\smpmpg1\*.obj GOTO mpg_smp1
cd mpg\smpmpg1
call mk
cd ..\..
:mpg_smp1

ECHO pcm\smppcm13
IF EXIST pcm\smppcm13\*.obj GOTO smppcm13
cd pcm\smppcm13
call mk
cd ..\..
:smppcm13

ECHO pcm\smppcm2
IF EXIST pcm\smppcm2\*.obj GOTO smppcm2
cd pcm\smppcm2
call mk
cd ..\..
:smppcm2

ECHO pcm\smppcm3
IF EXIST pcm\smppcm3\*.obj GOTO smppcm3
cd pcm\smppcm3
call mk
cd ..\..
:smppcm3

ECHO pcm\smppcm4
IF EXIST pcm\smppcm4\*.obj GOTO smppcm4
cd pcm\smppcm4
call mk
cd ..\..
:smppcm4

ECHO pcm\smppcm5
IF EXIST pcm\smppcm5\*.obj GOTO smppcm5
cd pcm\smppcm5
call mk
cd ..\..
:smppcm5

ECHO per\smpper0
IF EXIST per\smpper0\*.obj GOTO smpper0
cd per\smpper0
call mk
cd ..\..
:smpper0

ECHO per\smpper1
IF EXIST per\smpper1\*.obj GOTO smpper1
cd per\smpper1
call mk
cd ..\..
:smpper1

ECHO per\smpclib
IF EXIST per\smpclib\*.obj GOTO smpclib
cd per\smpclib
call mk
cd ..\..
:smpclib

ECHO per\smpper7
IF EXIST per\smpper7\*.obj GOTO smpper7
cd per\smpper7
call mk
cd ..\..
:smpper7

ECHO per\smpper8
IF EXIST per\smpper8\*.obj GOTO smpper8
cd per\smpper8
call mk
cd ..\..
:smpper8

ECHO per\smpper12
IF EXIST per\smpper12\*.obj GOTO smpper12
cd per\smpper12
call mk
cd ..\..
:smpper12

ECHO scl\smpscl1
IF EXIST scl\smpscl1\*.obj GOTO smpscl1
cd scl\smpscl1
call mk
cd ..\..
:smpscl1

ECHO scl\smpscl3
IF EXIST scl\smpscl3\*.obj GOTO smpscl3
cd scl\smpscl3
call mk
cd ..\..
:smpscl3

ECHO scl\smpscl5
IF EXIST scl\smpscl5\*.obj GOTO smpscl5
cd scl\smpscl5
call mk
cd ..\..
:smpscl5

ECHO snd\smpsnd1
IF EXIST snd\smpsnd1\*.obj GOTO smpsnd1
cd snd\smpsnd1
call mk
cd ..\..
:smpsnd1

ECHO snd\smpsnd4
IF EXIST snd\smpsnd4\*.obj GOTO smpsnd4
cd snd\smpsnd4
call mk
cd ..\..
:smpsnd4

ECHO snd\smpsnd5
IF EXIST snd\smpsnd5\*.obj GOTO smpsnd5
cd snd\smpsnd5
call mk
cd ..\..
:smpsnd5

ECHO snd\smpsnd6
IF EXIST snd\smpsnd6\*.obj GOTO smpsnd6
cd snd\smpsnd6
call mk
cd ..\..
:smpsnd6

ECHO spr\smpspr2
IF EXIST spr\smpspr2\*.obj GOTO smpspr2
cd spr\smpspr2
call mk
cd ..\..
:smpspr2

ECHO spr\smpspr7
IF EXIST spr\smpspr7\*.obj GOTO smpspr7
cd spr\smpspr7
call mk
cd ..\..
:smpspr7

ECHO spr\smpspr8
IF EXIST spr\smpspr8\*.obj GOTO smpspr8
cd spr\smpspr8
call mk
cd ..\..
:smpspr8

ECHO stm\smpstm1
IF EXIST stm\smpstm1\*.obj GOTO smpstm1
cd stm\smpstm1
call mk
cd ..\..
:smpstm1

ECHO stm\smpstm2
IF EXIST stm\smpstm2\*.obj GOTO smpstm2
cd stm\smpstm2
call mk
cd ..\..
:smpstm2

ECHO tim\smp0
IF EXIST tim\smp0\*.obj GOTO tim_smp0
cd tim\smp0
call mk
cd ..\..
:tim_smp0

ECHO tim\smp1
IF EXIST tim\smp1\*.obj GOTO tim_smp1
cd tim\smp1
call mk
cd ..\..
:tim_smp1

ECHO sys
IF EXIST sys\*.obj GOTO sys
cd sys
call mk
cd ..
:sys

ECHO game
IF EXIST game\*.obj GOTO game
cd game
call mk
cd ..
:game

:end
