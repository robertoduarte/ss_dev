@echo off
echo ---------------------------------------------------------------------------
echo   main.bat -- PC �p �T���v���쐬�R�}���h�t�@�C��
echo   Copyright(c) 1994 SEGA
echo   Written by H.E on 1994-02-15 Ver.0.80
echo   Updated by K.M on 1994-05-10 Ver.0.80
echo ---------------------------------------------------------------------------
rem set SHC_LIB=..\..\SH\bin
rem set SHC_INC=..\..\SH\INCLUDE
rem set SHC_TMP=..\..\sh\bin

del game.abs
del *.obj
shc -sub=mk.cfg title.c
shc -sub=mk.cfg demo.c
shc -sub=mk.cfg logo.c
shc -sub=mk.cfg v_blank.c
shc -sub=mk.cfg polygon.c
shc -sub=mk.cfg game_sub.c
shc -sub=mk.cfg act.c
shc -sub=mk.cfg game.c
shc -sub=mk.cfg data.c
shc -sub=mk.cfg sequence.c
lnk /sub=main.sub
rof2bin game.abs
rem end of file
