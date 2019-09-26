@echo off
..\..\..\sh\bin\shc -sub=smpscl3.shc smpscl3.c
..\..\..\sh\bin\shc -sub=smpscl3.shc scldata.c
..\..\..\sh\bin\lnk -sub=smpscl3.lnk
..\..\..\sh\bin\rof2bin -q smpscl3.abs
