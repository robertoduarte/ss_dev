@echo off
..\..\sh\bin\shc -sub=smpdual.shc slvsamp.c
..\..\sh\bin\shc -sub=smpdual.shc slavesh.c
..\..\sh\bin\shc -sub=smpdual.shc spr_xpt.c
..\..\sh\bin\asmsh slv_mtx3.src -cpu=7600
..\..\sh\bin\asmsh spr_mtx3.src -cpu=7600
..\..\sh\bin\asmsh spr_fmat.src -cpu=7600
..\..\sh\bin\asmsh spr_sinc.src -cpu=7600
..\..\sh\bin\lnk -sub=smpdual.lnk >lnk.err
..\..\sh\bin\rof2bin -q smpdual.abs
