:	mk.bat -- PC �p �T���v���쐬�o�b�`�t�@�C��
:	Copyright(c) 1994 SEGA
:	Written by M.K on 1994-11-11 Ver.1.10
:	Modified by K.K on 1995-09-20
@echo off
if "%1"=="--" goto test
if not "%1"=="-?" goto job
    echo Usage: %0 [-option]
    echo  (ex.) %0 -debug
    goto exit
:job
    ..\..\sh\bin\asmsh sys_id.src -cpu=7600 %1 %2 %3
    ..\..\sh\bin\shc -sub=smpsys.shc smpsys.c
:test
    ..\..\sh\bin\lnk -sub=smpsys.sub
    ..\..\sh\bin\rof2bin -q ip_sh.abs
    del ip_sh.abs
:exit
