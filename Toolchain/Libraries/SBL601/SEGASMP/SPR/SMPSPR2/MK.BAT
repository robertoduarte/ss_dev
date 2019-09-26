@echo off
..\..\..\sh\bin\shc -sub=smpspr20.shc smpspr20.c
..\..\..\sh\bin\lnk -sub=smpspr20.lnk
..\..\..\sh\bin\rof2bin smpspr20.abs
