@echo off
IF EXIST sega_snd.lib del sega_snd.lib
..\..\sh\bin\shc -sub=snd.shc snd_main.c
..\..\sh\bin\lbr /sub=snd.lbr
