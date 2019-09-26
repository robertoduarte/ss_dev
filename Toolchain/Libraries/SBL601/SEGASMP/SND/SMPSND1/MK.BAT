@echo off
..\..\..\sh\bin\shc -sub=smpsnd1.shc smpsnd1.c
..\..\..\sh\bin\lnk -sub=smpsnd1.lnk >lnk.err
..\..\..\sh\bin\rof2bin -q smpsnd1.abs
