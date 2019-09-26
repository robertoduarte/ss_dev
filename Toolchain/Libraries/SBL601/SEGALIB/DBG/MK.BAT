@echo off
IF EXIST sega_dbg.lib del sega_dbg.lib
..\..\sh\bin\shc -sub=dbg.shc dbg_disp.c
..\..\sh\bin\shc -sub=dbg.shc dbg_moni.c
..\..\sh\bin\asmsh dbg_intr.src -cpu=7600
..\..\sh\bin\lbr /sub=dbg.lbr
