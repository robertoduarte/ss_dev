@echo off
..\..\..\sh\bin\shc -sub=per_x.shc per_x1.c
..\..\..\sh\bin\shc -sub=per_x.shc per_x2.c
..\..\..\sh\bin\asmsh per_x3.src -cpu=7600
