@echo off
IF EXIST sega_spr.lib del sega_spr.lib
..\..\sh\bin\shc -sub=spr.shc spr_slv.c
..\..\sh\bin\shc -sub=spr.shc spr_1c.c
..\..\sh\bin\shc -sub=spr.shc spr_2c.c
..\..\sh\bin\shc -sub=spr.shc spr_3c0.c
..\..\sh\bin\shc -sub=spr.shc spr_3c1.c
..\..\sh\bin\shc -sub=spr.shc spr_3dsp.c
..\..\sh\bin\asmsh spr_2a.src -cpu=7600
..\..\sh\bin\asmsh spr_3a0.src -cpu=7600
..\..\sh\bin\asmsh spr_3a1.src -cpu=7600
..\..\sh\bin\asmsh spr_3tri.src -cpu=7600
..\..\sh\bin\lbr /sub=spr.lbr
