@echo off
IF EXIST sega_cmp.lib del sega_cmp.lib
..\..\sh\bin\shc -sub=cmp.shc cmplrun.c
..\..\sh\bin\shc -sub=cmp.shc cmplrunb.c
..\..\sh\bin\shc -sub=cmp.shc cmplrunw.c
..\..\sh\bin\shc -sub=cmp.shc cmplrund.c
..\..\sh\bin\shc -sub=cmp.shc cmpltoolc
..\..\sh\bin\lbr /sub=cmp.lbr
