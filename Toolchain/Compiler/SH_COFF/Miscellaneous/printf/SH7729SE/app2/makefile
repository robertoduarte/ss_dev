#****************************************************************
#KPIT Cummins Infosystems Ltd, Pune, India. - 1-Feb-2004.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#****************************************************************

PREFIX=sh-coff

DEBUG_FLAGS=-g

# Specify compiler to be used
CC = $(PREFIX)-gcc

# Specify Assembler to be used
AS = $(PREFIX)-as

# Specify linker to be used
LD = $(PREFIX)-ld

# Specify objcopy to be used
OBJCOPY = $(PREFIX)-objcopy

# Specify CPU flag
CPU = -m3 

# Specify ENDIAN 
ENDIAN = -mb 

# Common compiler flags
CFLAGS = -O2 -DSH7729SE 
STARTOPTS= -nostartfiles

ALL_FLAGS = $(CFLAGS) $(DEBUG_FLAGS) $(CPU) $(ENDIAN) $(STARTOPTS)

# Application name
APPNAME = sample

# Linker script
LNK = $(APPNAME).ld

all : $(APPNAME).mot

SIM = printf.c sample.c 
SOURCES = hwinit.c start.S uart.c syscalls.c $(SIM)

sim: $(SIM)
	$(CC) $(CFLAGS) $(CPU) $(ENDIAN) $(DEBUG_FLAGS) -DSIM -Xlinker -Map -Xlinker $(APPNAME)-sim.map -o $(APPNAME)-sim.out $(SIM) 

$(APPNAME).mot: $(APPNAME).out
	$(OBJCOPY) -O srec $< $@

$(APPNAME).out : $(SOURCES) $(LNK) makefile
	$(CC) $(ALL_FLAGS) -Xlinker -Map -Xlinker $(APPNAME).map -T$(LNK) -o $@ $(SOURCES) 

clean:
	rm -f *.o *.out *.map *.mot


