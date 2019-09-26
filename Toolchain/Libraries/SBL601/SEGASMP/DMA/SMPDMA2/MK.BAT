@echo off
..\..\..\sh\bin\shc -sub=smpdma2.shc smpdma2.c
..\..\..\sh\bin\lnk -sub=smpdma2.lnk >lnk.err
..\..\..\sh\bin\rof2bin -q smpdma2.abs
