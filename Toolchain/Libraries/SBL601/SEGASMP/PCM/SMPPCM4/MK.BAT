@echo off
..\..\..\sh\bin\shc -sub=smppcm4.shc smppcm4.c
..\..\..\sh\bin\lnk -sub=smppcm4.lnk >lnk.err
..\..\..\sh\bin\rof2bin -q smppcm4.abs
