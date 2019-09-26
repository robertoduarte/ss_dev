@echo off
..\..\..\sh\bin\shc -sub=smpscl5.shc smpscl5.c
..\..\..\sh\bin\shc -sub=smpscl5.shc asciifon.c
..\..\..\sh\bin\shc -sub=smpscl5.shc textur.c
..\..\..\sh\bin\lnk -sub=smpscl5.lnk
..\..\..\sh\bin\rof2bin -q smpscl5.abs
