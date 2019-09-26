: \bin\tc\tcc -v- -f- -k- -G- -O -Z -I\bin\tc\include\ -L\bin\tc\lib timestmp.c
set temp=a:\tmp\
: \hitech\C -o -v -f -i.\ -mtimestmp.map timestmp.c
\hitech\C -o -v timestmp.c
@echo off
ls -l timestmp.exe
pause
if exist tags del tags
echo ###timestmp -? >>tags
timestmp -?>>tags
echo ###timestmp>>tags
timestmp >>tags
echo ###timestmp "test "TEST -------" >>tags
timestmp "test "TEST ----------" >>tags
echo ###timestmp "test "TEST -----Yen>>tags
timestmp "test "TEST ----------\" >>tags
type tags
