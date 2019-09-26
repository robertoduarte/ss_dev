..\..\sh\bin\shc gfs.c -sub=gfs.shc
..\..\sh\bin\shc gfs_buf.c -sub=gfs.shc 
..\..\sh\bin\shc gfs_cdb.c -sub=gfs.shc 
..\..\sh\bin\shc gfs_cdc.c -sub=gfs.shc 
..\..\sh\bin\shc gfs_cdf.c -sub=gfs.shc 
..\..\sh\bin\shc gfs_dir.c -sub=gfs.shc 
..\..\sh\bin\shc gfs_trn.c -sub=gfs.shc 
IF EXIST sega_gfs.lib del sega_gfs.lib
..\..\sh\bin\lbr -sub=gfs.lbr
..\..\sh\bin\shc gfs.c -ob=gfsd.obj -def=DEBUG_LIB -sub=gfs.shc 
..\..\sh\bin\shc gfs_buf.c -ob=gfsd_buf.obj -def=DEBUG_LIB  -sub=gfs.shc 
..\..\sh\bin\shc gfs_cdb.c -ob=gfsd_cdb.obj -def=DEBUG_LIB  -sub=gfs.shc 
..\..\sh\bin\shc gfs_cdc.c -ob=gfsd_cdc.obj -def=DEBUG_LIB  -sub=gfs.shc 
..\..\sh\bin\shc gfs_cdf.c -ob=gfsd_cdf.obj -def=DEBUG_LIB  -sub=gfs.shc 
..\..\sh\bin\shc gfs_dir.c -ob=gfsd_dir.obj -def=DEBUG_LIB  -sub=gfs.shc 
..\..\sh\bin\shc gfs_mmb.c -ob=gfsd_mmb.obj -def=DEBUG_LIB  -sub=gfs.shc 
..\..\sh\bin\shc gfs_mmc.c -ob=gfsd_mmc.obj -def=DEBUG_LIB  -sub=gfs.shc 
..\..\sh\bin\shc gfs_mmf.c -ob=gfsd_mmf.obj -def=DEBUG_LIB  -sub=gfs.shc 
..\..\sh\bin\shc gfs_sfb.c -ob=gfsd_sfb.obj -def=DEBUG_LIB  -sub=gfs.shc 
..\..\sh\bin\shc gfs_sff.c -ob=gfsd_sff.obj -def=DEBUG_LIB  -sub=gfs.shc 
..\..\sh\bin\shc gfs_trn.c -ob=gfsd_trn.obj -def=DEBUG_LIB  -sub=gfs.shc 
IF EXIST segadgfs.lib del segadgfs.lib
..\..\sh\bin\lbr -sub=gfsd1.lbr
