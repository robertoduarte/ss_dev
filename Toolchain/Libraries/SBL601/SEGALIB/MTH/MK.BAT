@echo off
IF EXIST sega_mth.lib del sega_mth.lib

..\..\sh\bin\shc -sub=mth.shc mth_curv.c
..\..\sh\bin\shc -sub=mth.shc mth_dsps.c
..\..\sh\bin\shc -sub=mth.shc mth_rand.c
..\..\sh\bin\asmsh mth_fixd.src -cpu=7600
..\..\sh\bin\asmsh mth_tri.src -cpu=7600
..\..\sh\bin\asmsh mth_mtrx.src -cpu=7600
..\..\sh\bin\asmsh mth_ps2d.src -cpu=7600
..\..\sh\bin\asmsh mth_norm.src -cpu=7600
..\..\sh\bin\lbr /sub=mth.lbr
