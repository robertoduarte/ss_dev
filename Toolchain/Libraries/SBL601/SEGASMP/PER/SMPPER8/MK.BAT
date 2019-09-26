@echo off
IF EXIST smpper8.bin del smpper8.bin
..\..\..\sh\bin\shc -sub=smpper8.shc smpper8.c
..\..\..\sh\bin\shc -sub=smpper8.shc disp.c
..\..\..\sh\bin\shc -sub=smpper8.shc image.c
..\..\..\sh\bin\shc -sub=smpper8.shc move.c
..\..\..\sh\bin\shc -sub=smpper8.shc spr2.c
..\..\..\sh\bin\lnk -sub=smpper8.lnk
..\..\..\sh\bin\rof2bin smpper8.abs
