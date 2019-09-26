@echo off
..\..\..\sh\bin\shc -sub=smpmpg1.shc smpmpg1.c
..\..\..\sh\bin\lnk -sub=smpmpg1.lnk >lnk.err
..\..\..\sh\bin\rof2bin -q smpmpg1.abs
