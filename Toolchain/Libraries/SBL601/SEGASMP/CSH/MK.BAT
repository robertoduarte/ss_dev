@echo off
..\..\sh\bin\shc -sub=smpcsh.shc smpcsh.c
..\..\sh\bin\lnk -sub=smpcsh.lnk
..\..\sh\bin\rof2bin -q smpcsh.abs
