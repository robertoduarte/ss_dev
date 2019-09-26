@echo off
..\..\..\sh\bin\shc -sub=smppcm5.shc smppcm5.c
..\..\..\sh\bin\lnk -sub=smppcm5.lnk >lnk.err
..\..\..\sh\bin\rof2bin -q smppcm5.abs
