This distribution contains source code and makefiles for a simple 
Saturn SGL program. 

Before trying to build the code, check the SGLDIR, SBLDIR, and GCCDIR 
variables in the makefile to ensure that they point to the correct
directories containing the SGL 2.0 libraries, the SBL 6.0 libraries, and 
the GNU compiler.

With all the contents of this ZIP archive in one directory, invoke 
the GNU make utility. It should make SL.COF and SL.BIN. Load SL.COF in
the SNASM debugger and execute it.

When the program starts, there should be a hovering flying saucer and
several target objects floating in the air and a curved surface beneath.

Flying the saucer:

The D-pad controls facing and forward/back throttle. X and A increase and
decrease the altitude of the saucer. The index finger buttons "sideslip". 
The B button fires. The start button returns the saucer to its starting 
point.

Targets can be destroyed either by ramming or by shooting them.





CONTENTS
-----------
makefile	- makefile, object module list file, and linkage map
objects
sl.lnk

cinit.c
actor.c
ai.c
ai.h
aictrl.c
ailand.c
aisaucer.c
collide.c
collide.h
demo.c
satrend.c
render.h
tree.c
actor.h
assert.h
demo.h
enigma.h
generic.h
saturn.h
texture.h
tree.h
vector.h
mem_mgr.c
list.c
list.h
main.c
saturn.c
master.h
mem_mgr.h
vector.c
texture.c
aitarget.c

test.bgc

test.tga
tile.tga
land0.tga
ball.tga
blip.tga
spark.tga
spark16.tga
target.tga

test.dat

snbugsat.ini

