@echo off
..\..\..\sh\bin\shc -sub=smpmem1.shc smpmem1.c
..\..\..\sh\bin\lnk -sub=smpmem1.lnk
..\..\..\sh\bin\rof2bin smpmem1.abs
