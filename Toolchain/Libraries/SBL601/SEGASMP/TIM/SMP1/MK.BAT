@echo off
..\..\..\sh\bin\shc -sub=smptim1.shc smptim1.c
..\..\..\sh\bin\lnk -sub=smptim1.lnk
..\..\..\sh\bin\rof2bin -q smptim1.abs
