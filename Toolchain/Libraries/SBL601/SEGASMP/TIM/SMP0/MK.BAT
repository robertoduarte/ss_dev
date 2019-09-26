@echo off
..\..\..\sh\bin\shc -sub=smptim0.shc smptim0.c
..\..\..\sh\bin\lnk -sub=smptim0.lnk
..\..\..\sh\bin\rof2bin -q smptim0.abs
