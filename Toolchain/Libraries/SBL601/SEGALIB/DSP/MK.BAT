@echo off
IF EXIST sega_dsp.lib del sega_dsp.lib
..\..\sh\bin\shc -sub=dsp.shc dsp_ctrl.c
..\..\sh\bin\lbr /sub=dsp.lbr
