@echo off
..\..\..\sh\bin\shc -sub=smpdbg10.shc smpdbg10.c
..\..\..\sh\bin\lnk -sub=smpdbg10.lnk >lnk.err
..\..\..\sh\bin\rof2bin -q smpdbg10.abs
