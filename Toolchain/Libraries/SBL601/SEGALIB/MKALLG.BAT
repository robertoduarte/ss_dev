rem @echo off
IF NOT "SEGALIBAWK"=="" goto gccenv
echo SEGALIBAWK not defined.
goto end
:gccenv
IF NOT "LIBRARY_PATH"=="" goto work
call ..\gccsh\setenv
:work

IF NOT EXIST ADP\NUL goto l00
go32 ..\gccsh\bin\make.exe PROJ=ADP _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l00

IF NOT EXIST BPL\NUL goto l01
go32 ..\gccsh\bin\make.exe PROJ=BPL _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l01

IF NOT EXIST CDC\NUL goto l02
go32 ..\gccsh\bin\make.exe PROJ=CDC _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l02

IF NOT EXIST CMP\NUL goto l03
go32 ..\gccsh\bin\make.exe PROJ=CMP _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l03

IF NOT EXIST CSH\NUL goto l04
go32 ..\gccsh\bin\make.exe PROJ=CSH _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l04

IF NOT EXIST DBG\NUL goto l05
go32 ..\gccsh\bin\make.exe PROJ=DBG _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l05

IF NOT EXIST DMA\NUL goto l06
go32 ..\gccsh\bin\make.exe PROJ=DMA _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l06

IF NOT EXIST DSP\NUL goto l07
go32 ..\gccsh\bin\make.exe PROJ=DSP _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l07

IF NOT EXIST FLD\NUL goto l08
go32 ..\gccsh\bin\make.exe PROJ=FLD _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l08

IF NOT EXIST GFS\NUL goto l09
go32 ..\gccsh\bin\make.exe PROJ=GFS _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l09

IF NOT EXIST MEM\NUL goto l10
go32 ..\gccsh\bin\make.exe PROJ=MEM _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l10

IF NOT EXIST MPG\NUL goto l11
go32 ..\gccsh\bin\make.exe PROJ=MPG _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l11

IF NOT EXIST MTH\NUL goto l12
go32 ..\gccsh\bin\make.exe PROJ=MTH _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l12

IF NOT EXIST PCM\NUL goto l13
go32 ..\gccsh\bin\make.exe PROJ=PCM _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l13

IF NOT EXIST PER\NUL goto l14
go32 ..\gccsh\bin\make.exe PROJ=PER _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l14

IF NOT EXIST SCL\NUL goto l15
go32 ..\gccsh\bin\make.exe PROJ=SCL _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l15

IF NOT EXIST SHC\NUL goto l16
go32 ..\gccsh\bin\make.exe PROJ=SHC _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l16

IF NOT EXIST SND\NUL goto l17
go32 ..\gccsh\bin\make.exe PROJ=SND _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l17

IF NOT EXIST SPR\NUL goto l18
go32 ..\gccsh\bin\make.exe PROJ=SPR _CFLAGS=-O0
IF ERRORLEVEL 1 GOTO end
:l18

IF NOT EXIST STM\NUL goto l19
go32 ..\gccsh\bin\make.exe PROJ=STM "_CFLAGS=-O2 -IGFS"
IF ERRORLEVEL 1 GOTO end
:l19

IF NOT EXIST INT\NUL goto l20
go32 ..\gccsh\bin\make.exe PROJ=INT _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l20

IF NOT EXIST SYS\NUL goto l21
go32 ..\gccsh\bin\make.exe PROJ=SYS _CFLAGS=-O2
IF ERRORLEVEL 1 GOTO end
:l21

IF NOT EXIST SGL\NUL goto l22
cd sgl
call mkallg
cd ..
IF ERRORLEVEL 1 GOTO end
:l22

:end
