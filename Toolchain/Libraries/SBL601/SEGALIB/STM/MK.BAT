..\..\sh\bin\shc -sub=stm.shc stm_init.c -ob=stm_init.obj
..\..\sh\bin\shc -sub=stm.shc stm_svr.c -ob=stm_svr.obj
..\..\sh\bin\shc -sub=stm.shc stm_trn.c -ob=stm_trn.obj
..\..\sh\bin\shc -sub=stm.shc stm_acc.c -ob=stm_acc.obj
..\..\sh\bin\shc -sub=stm.shc stm_loc.c -ob=stm_loc.obj
IF EXIST sega_stm.lib del sega_stm.lib
..\..\sh\bin\lbr -sub=stm.lbr
