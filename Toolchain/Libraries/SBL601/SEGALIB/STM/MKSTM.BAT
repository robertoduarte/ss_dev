shc -sub=stm.shc stm_init.c -ob=stm_init.obj
shc -sub=stm.shc stm_svr.c -ob=stm_svr.obj
shc -sub=stm.shc stm_trn.c -ob=stm_trn.obj
shc -sub=stm.shc stm_acc.c -ob=stm_acc.obj
shc -sub=stm.shc stm_loc.c -ob=stm_loc.obj
del sega_stm.lib
lbr -sub=stm.lbr
