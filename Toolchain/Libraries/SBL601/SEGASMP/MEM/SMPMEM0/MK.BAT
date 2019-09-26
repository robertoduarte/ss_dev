@echo off
..\..\..\sh\bin\shc -sub=smpmem0.shc smpmem0.c
..\..\..\sh\bin\lnk -sub=smpmem0.lnk
..\..\..\sh\bin\rof2bin smpmem0.abs
