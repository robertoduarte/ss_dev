@echo off
..\..\..\sh\bin\shc -sub=smpsnd6.shc smpsnd6.c
..\..\..\sh\bin\lnk -sub=smpsnd6.lnk >lnk.err
..\..\..\sh\bin\rof2bin -q smpsnd6.abs
