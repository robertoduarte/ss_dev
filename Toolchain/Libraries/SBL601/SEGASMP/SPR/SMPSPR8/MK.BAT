@echo off
..\..\..\sh\bin\shc -sub=smpspr80.shc smpspr80.c
..\..\..\sh\bin\lnk -sub=smpspr80.lnk
..\..\..\sh\bin\rof2bin smpspr80.abs
