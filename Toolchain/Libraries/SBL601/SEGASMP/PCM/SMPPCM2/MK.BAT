@echo off
..\..\..\sh\bin\shc -sub=smppcm2.shc smppcm2.c
..\..\..\sh\bin\lnk -sub=smppcm2.lnk
..\..\..\sh\bin\rof2bin -q smppcm2.abs
