

cd bup
go32 ..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..

cd csh
go32 ..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..

cd dbg\smpdbg1
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd dma\smpdma0
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd dma\smpdma1
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd dma\smpdma10
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd dma\smpdma12
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd dma\smpdma2
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd dual
go32 ..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..

cd gfs\smpgfs1a
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd gfs\smpgfs1b
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd gfs\smpgfs2
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd mem\smpmem0
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd mem\smpmem1
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd mpg\smpmpg1
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd pcm\smppcm13
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd pcm\smppcm2
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd pcm\smppcm3
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd pcm\smppcm4
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd pcm\smppcm5
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd per\smpper0
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd per\smpper1
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd per\smpper7
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd per\smpper8
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd per\smpper12
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd scl\smpscl1
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd scl\smpscl3
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd scl\smpscl5
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd snd\smpsnd1
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd snd\smpsnd4
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd snd\smpsnd5
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd snd\smpsnd6
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd spr\smpspr2
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd spr\smpspr7
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd spr\smpspr8
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd stm\smpstm1
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd stm\smpstm2
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd tim\smp0
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd tim\smp1
go32 ..\..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..\..

cd sys
go32 ..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..

cd game
go32 ..\..\gccsh\bin\make.exe
IF ERRORLEVEL 1 GOTO end
cd ..

:end
