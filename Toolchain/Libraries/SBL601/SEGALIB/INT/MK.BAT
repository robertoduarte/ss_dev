@echo off
IF EXIST sega_int.lib del sega_int.lib
..\..\sh\bin\shc -sub=int.shc int.c
..\..\sh\bin\asmsh int_trmp.src -cpu=7600
..\..\sh\bin\lbr /sub=int.lbr
