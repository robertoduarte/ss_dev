@echo off
..\..\..\sh\bin\shc -sub=smpsnd4.shc smpsnd4.c
..\..\..\sh\bin\lnk -sub=smpsnd4.lnk >lnk.err
..\..\..\sh\bin\rof2bin -q smpsnd4.abs
