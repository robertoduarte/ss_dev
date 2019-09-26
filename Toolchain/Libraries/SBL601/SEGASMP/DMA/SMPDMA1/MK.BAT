@echo off
..\..\..\sh\bin\shc -sub=smpdma1.shc smpdma1.c
..\..\..\sh\bin\lnk -sub=smpdma1.lnk >lnk.err
..\..\..\sh\bin\rof2bin -q smpdma1.abs
