@echo off
..\..\..\sh\bin\shc -sub=smpdma0.shc smpdma0.c
..\..\..\sh\bin\lnk -sub=smpdma0.lnk >lnk.err
..\..\..\sh\bin\rof2bin -q smpdma0.abs
