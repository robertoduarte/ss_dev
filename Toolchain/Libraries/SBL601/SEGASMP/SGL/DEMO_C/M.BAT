@echo off
echo ---------------------------------------------------------------------------
echo   main.bat -- PC �p �T���v���쐬�R�}���h�t�@�C��
echo   Copyright(c) 1994 SEGA
echo   Written by H.E on 1994-02-15 Ver.0.80
echo   Updated by K.M on 1994-05-10 Ver.0.80
echo ---------------------------------------------------------------------------
rem set SHC_LIB=d:\SH\bin
rem set SHC_INC=d:\SH\INCLUDE
rem set SHC_TMP=d:\sh\bin

del main.abs
del *.obj
shc -sub=..\mk.cfg main.c
shc -sub=..\mk.cfg aki_ashi.c
shc -sub=..\mk.cfg aki_dou.c
shc -sub=..\mk.cfg aki_kao.c
shc -sub=..\mk.cfg aki_kata.c
shc -sub=..\mk.cfg aki_ude.c
shc -sub=..\mk.cfg aki_walk.c
lnk /sub=main.sub
rof2bin main.abs
rem end of file
