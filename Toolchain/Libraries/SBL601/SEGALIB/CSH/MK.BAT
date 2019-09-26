@echo off
IF EXIST sega_csh.lib del sega_csh.lib
..\..\sh\bin\shc -sub=csh.shc csh_main.c
..\..\sh\bin\lbr /sub=csh.lbr
