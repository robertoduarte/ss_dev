@echo off
IF EXIST sega_pcm.lib del sega_pcm.lib
..\..\sh\bin\shc -sub=pcm.shc pcm_lib.c
..\..\sh\bin\shc -sub=pcm.shc pcm_gfs.c
..\..\sh\bin\shc -sub=pcm.shc pcm_stm.c
..\..\sh\bin\shc -sub=pcm.shc pcm_mp.c
..\..\sh\bin\shc -sub=pcm.shc pcm_time.c
..\..\sh\bin\shc -sub=pcm.shc pcm_audi.c
..\..\sh\bin\shc -sub=pcm.shc pcm_aif.c
..\..\sh\bin\shc -sub=pcm.shc pcm_aif2.c
..\..\sh\bin\shc -sub=pcm.shc pcm_drv.c
..\..\sh\bin\shc -sub=pcm.shc pcm_etc.c
..\..\sh\bin\shc -sub=pcm.shc pcm_adp.c
..\..\sh\bin\lbr /sub=pcm.lbr
