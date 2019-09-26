@echo off
..\..\..\sh\bin\shc -sub=smpscl1.shc smpscl1.c
..\..\..\sh\bin\lnk -sub=smpscl1.lnk
..\..\..\sh\bin\rof2bin -q smpscl1.abs
