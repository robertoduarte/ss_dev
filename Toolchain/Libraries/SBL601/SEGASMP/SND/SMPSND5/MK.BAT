@echo off
..\..\..\sh\bin\shc -sub=smpsnd5.shc smpsnd5.c
..\..\..\sh\bin\lnk -sub=smpsnd5.lnk >lnk.err
..\..\..\sh\bin\rof2bin -q smpsnd5.abs
