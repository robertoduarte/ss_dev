shc gfs.c -sub=gfs.shc
shc gfs_buf.c -sub=gfs.shc 
shc gfs_cdb.c -sub=gfs.shc 
shc gfs_cdc.c -sub=gfs.shc 
shc gfs_cdf.c -sub=gfs.shc 
shc gfs_dir.c -sub=gfs.shc 
shc gfs_trn.c -sub=gfs.shc 
del sega_gfs.lib
lbr -sub=gfs.lbr
shc gfs.c -ob=gfsd.obj -def=DEBUG_LIB -sub=gfs.shc 
shc gfs_buf.c -ob=gfsd_buf.obj -def=DEBUG_LIB  -sub=gfs.shc 
shc gfs_cdb.c -ob=gfsd_cdb.obj -def=DEBUG_LIB  -sub=gfs.shc 
shc gfs_cdc.c -ob=gfsd_cdc.obj -def=DEBUG_LIB  -sub=gfs.shc 
shc gfs_cdf.c -ob=gfsd_cdf.obj -def=DEBUG_LIB  -sub=gfs.shc 
shc gfs_dir.c -ob=gfsd_dir.obj -def=DEBUG_LIB  -sub=gfs.shc 
shc gfs_mmb.c -ob=gfsd_mmb.obj -def=DEBUG_LIB  -sub=gfs.shc 
shc gfs_mmc.c -ob=gfsd_mmc.obj -def=DEBUG_LIB  -sub=gfs.shc 
shc gfs_mmf.c -ob=gfsd_mmf.obj -def=DEBUG_LIB  -sub=gfs.shc 
shc gfs_sfb.c -ob=gfsd_sfb.obj -def=DEBUG_LIB  -sub=gfs.shc 
shc gfs_sff.c -ob=gfsd_sff.obj -def=DEBUG_LIB  -sub=gfs.shc 
shc gfs_trn.c -ob=gfsd_trn.obj -def=DEBUG_LIB  -sub=gfs.shc 
del segadgfs.lib
lbr -sub=gfsd1.lbr
lbr -sub=gfsd2.lbr
