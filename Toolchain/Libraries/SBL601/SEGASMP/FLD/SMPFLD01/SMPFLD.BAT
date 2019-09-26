shc -i=..\..\..\segalib\include -op=1 -cp=7600 -p=0 -st=c -sh=(noso) smpfld01.c
shc -i=..\..\..\segalib\include -op=1 -cp=7600 -p=0 -c=a -st=c -sh=(noso) exit.c
asmsh exit.src -cpu=7600
lnk -sub=smpfld.lnk
