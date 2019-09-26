@echo off
..\..\..\sh\bin\shc -sub=smppcm13.shc smppcm13.c
..\..\..\sh\bin\lnk -sub=smppcm13.lnk >lnk.err
..\..\..\sh\bin\rof2bin -q smppcm13.abs
