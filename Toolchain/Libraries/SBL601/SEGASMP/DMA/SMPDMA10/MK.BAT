@echo off
..\..\..\sh\bin\shc -sub=smpdma10.shc smpdma10.c
..\..\..\sh\bin\lnk -sub=smpdma10.lnk >lnk.err
..\..\..\sh\bin\rof2bin -q smpdma10.abs
