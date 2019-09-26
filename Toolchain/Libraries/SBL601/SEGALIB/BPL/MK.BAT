@echo off
IF EXIST sega_bpl.lib del sega_bpl.lib
..\..\sh\bin\shc -sub=bpl.shc bpl_main.c
..\..\sh\bin\lbr /sub=bpl.lbr
