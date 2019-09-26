@echo off
echo ---------------------------------------------------------------------------
echo   main.bat -- PC �p �T���v���쐬�R�}���h�t�@�C��
echo   Copyright(c) 1994 SEGA
echo   Written by H.E on 1994-02-15 Ver.0.80
echo   Updated by K.M on 1994-05-10 Ver.0.80
echo ---------------------------------------------------------------------------
rem set SHC_LIB=..\..\..\sh\bin
rem set SHC_INC=..\..\..\sh\INCLUDE
rem set SHC_TMP=..\..\..\sh\bin

del sblsgl04.abs
del *.obj
shc -sub=..\mk.cfg sblsgl04.c
shc -sub=..\mk.cfg scl_data.c
shc -sub=..\mk.cfg sgl_func.c
lnk /sub=main.sub
rof2bin sblsgl04.abs
rem end of file
