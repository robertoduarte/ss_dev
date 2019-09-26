@echo off
IF EXIST smpper12.bin del smpper12.bin
..\..\..\sh\bin\shc -sub=smpper12.shc smpper12.c
..\..\..\sh\bin\shc -sub=smpper12.shc image.c
..\..\..\sh\bin\shc -sub=smpper12.shc move.c
..\..\..\sh\bin\shc -sub=smpper12.shc spr2.c
..\..\..\sh\bin\shc -sub=smpper12.shc test.c
..\..\..\sh\bin\lnk -sub=smpper12.lnk
..\..\..\sh\bin\rof2bin smpper12.abs
