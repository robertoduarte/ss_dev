Readme.txt
------------

Why new implementations of printf() & scanf()
----------------------------------------------
The existing implementations of scanf() and printf() embed calls to malloc.
This increases the code size as well as the RAM requirements. These lite 
versions of printf() & scanf() do not make use of calls to malloc and help 
reducing the resource.
There are two implementation of printf() provided in app1 and app2 folders.


Limitation of the new implementation.
-------------------------------------
Implementation in app1:

These print() and scan() functions support only integers and not floating 
point representation. Only 3 values can be printed or read at a time (Please
refer to the prototype definitions of the same). Following are the supported 
functions and formats.
List of functions.
  print()
  sprint()
  scan()
  sscan()

The supported formats are:
  decimal:        %d / %4d  / %04d / %-04d
  decimal:        %u / %4u  / %04u / %-04u
  hexidecimal:    %x / %4x  / %04x / %-04x
  string:         %s / %.4s / %-4s
  character:      %c

Implementation in app2:

In the folder app2 you will find an other implementation of printf() using 
stdargs. Currently scanf() is not implemented.

How to use print.c
-------------------
Implementation in app1:

Replace printf() & scanf() by print() & scan() respectively,  and compile 
applications along with print.c. Here usage of print() & scan() prototype 
is discouraged. Please remember if you do not use the function prototype 
then your application may not conform to the ANSI standard. To follow strict 
ANSI standard you should include print.h in your code and pass NULL('\0') 
for unused parameters. 
You may however use macros like 

	#define printf2(a,b) print(a,(int)b,'\0','\0')

for direct implementation. A few of them are already defined in print.h.
You may refer to the sample code provided in sample.c file.

Implementation in app2:

You may use the l_printf() and l_sprintf() defined in app2 folder exactly as
printf() and sprintf().

