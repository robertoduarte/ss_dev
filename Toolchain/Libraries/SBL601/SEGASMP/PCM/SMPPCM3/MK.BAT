@echo off
..\..\..\sh\bin\shc -sub=smppcm3.shc smppcm3.c
..\..\..\sh\bin\lnk -sub=smppcm3.lnk >lnk.err
..\..\..\sh\bin\rof2bin -q smppcm3.abs
