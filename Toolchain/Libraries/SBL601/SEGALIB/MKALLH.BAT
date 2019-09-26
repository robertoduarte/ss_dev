rem echo off
IF NOT "SHC_LIB"=="" goto work
call ..\sh\bin\setshc.bat
:work

IF NOT EXIST ADP\NUL goto l00
cd ADP
call mk
copy *.lib ..\lib
cd ..
:l00

IF NOT EXIST BPL\NUL goto l01
cd bpl
call mk
copy *.lib ..\lib
cd ..
:l01

IF NOT EXIST CDC\NUL goto l02
cd cdc
call mk
copy *.lib ..\lib
cd ..
:l02

IF NOT EXIST CMP\NUL goto l03
cd cmp
call mk
copy *.lib ..\lib
cd ..
:l03

IF NOT EXIST CSH\NUL goto l04
cd csh
call mk
copy *.lib ..\lib
cd ..
:l04

IF NOT EXIST DBG\NUL goto l05
cd dbg
call mk
copy *.lib ..\lib
cd ..
:l05

IF NOT EXIST DMA\NUL goto l06
cd dma
call mk
copy *.lib ..\lib
cd ..
:l06

IF NOT EXIST DSP\NUL goto l07
cd dsp
call mk
copy *.lib ..\lib
cd ..
:l07

IF NOT EXIST FLD\NUL goto l08
cd fld
call mk
call mkd
copy *.lib ..\lib
cd ..
:l08

IF NOT EXIST GFS\NUL goto l09
cd gfs
call mk
call mkd
copy *.lib ..\lib
cd ..
:l09

IF NOT EXIST MEM\NUL goto l10
cd mem
call mk
copy *.lib ..\lib
cd ..
:l10

IF NOT EXIST MPG\NUL goto l11
cd mpg
call mk
copy *.lib ..\lib
cd ..
:l11

IF NOT EXIST MTH\NUL goto l12
cd mth
call mk
copy *.lib ..\lib
cd ..
:l12

IF NOT EXIST PCM\NUL goto l13
cd pcm
call mk
copy *.lib ..\lib
cd ..
:l13

IF NOT EXIST PER\NUL goto l14
cd per
call mk
copy *.lib ..\lib
cd ..
:l14

IF NOT EXIST SCL\NUL goto l15
cd scl
call mk
copy *.lib ..\lib
cd ..
:l15

rem This library is for GCC.
rem IF NOT EXIST SHC\NUL goto l16
rem cd shc
rem call mk
rem copy *.lib ..\lib
rem cd ..
rem :l16

IF NOT EXIST SND\NUL goto l17
cd snd
call mk
copy *.lib ..\lib
cd ..
:l17

IF NOT EXIST SPR\NUL goto l18
cd spr
call mk
copy *.lib ..\lib
cd ..
:l18

IF NOT EXIST STM\NUL goto l19
cd stm
call mk
copy *.lib ..\lib
cd ..
:l19

IF NOT EXIST INT\NUL goto l20
cd int
call mk
copy *.lib ..\lib
cd ..
:l20

IF NOT EXIST SYS\NUL goto l21
cd sys
call mk
copy *.lib ..\lib
cd ..
:l21

IF EXIST lib\sega_sat.lib del lib\sega_sat.lib
..\sh\bin\lbr -sub=sat.lbr

IF NOT EXIST SGL\NUL goto l22
cd sgl
if exist sega_sgl.lib del sega_sgl.lib
call mkallh
copy *.lib ..\lib
cd ..
:l22

