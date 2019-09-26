@echo off
..\..\..\sh\bin\shc -sub=smpdma12.shc smpdma12.c
..\..\..\sh\bin\lnk -sub=smpdma12.lnk >lnk.err
..\..\..\sh\bin\rof2bin -q smpdma12.abs
