@echo off
IF EXIST smpper7.bin del smpper7.bin
..\..\..\sh\bin\shc -sub=smpper7.shc smpper7.c
..\..\..\sh\bin\shc -sub=smpper7.shc disp.c
..\..\..\sh\bin\shc -sub=smpper7.shc image.c
..\..\..\sh\bin\shc -sub=smpper7.shc move.c
..\..\..\sh\bin\shc -sub=smpper7.shc spr2.c
..\..\..\sh\bin\lnk -sub=smpper7.lnk
..\..\..\sh\bin\rof2bin smpper7.abs
