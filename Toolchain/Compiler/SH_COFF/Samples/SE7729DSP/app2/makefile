#/****************************************************************
#KPIT Cummins Infosystems Limited Pune, India. -01-Sept-2003.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
#*****************************************************************/

PREFIX=sh-coff

DEBUG_FLAGS=-g

# Specify compiler to be used
CC = $(PREFIX)-gcc

# Specify Assembler to be used
AS = $(PREFIX)-as

# Specify linker to be used
LD = $(PREFIX)-ld

# Specify CPU flag
CPU = -m3

# Common compiler flags
CFLAGS = -nostartfiles

ALL_FLAGS = $(CFLAGS) $(DEBUG_FLAGS) $(CPU)

# Application name
APPNAME = blink

# Specify all objects that you are going to link together
OBJS = start.o hwinit.o $(APPNAME).o

SOURCES = start.S hwinit.c $(APPNAME).c 

$(APPNAME).mot : $(APPNAME).out
	$(PREFIX)-objcopy -O srec $(APPNAME).out $@

# Define ROMSTART if compiling for ROM

$(APPNAME).out : $(SOURCES) $(APPNAME).lnk makefile
	$(CC) $(ALL_FLAGS) -DROMSTART -DRELEASE -T$(APPNAME).lnk -Xlinker -Map -Xlinker $(APPNAME).map -o $@ $(SOURCES) 

all: $(APPNAME).mot $(APPNAME)-ram.mot

rom: $(APPNAME).mot

ram: $(APPNAME)-ram.mot

$(APPNAME)-ram.mot : $(APPNAME)-ram.out
	$(PREFIX)-objcopy -O srec $(APPNAME)-ram.out $@

$(APPNAME)-ram.out : $(SOURCES) $(APPNAME).ram.lnk makefile
	$(CC) $(ALL_FLAGS) -DDEBUG -T$(APPNAME).ram.lnk -Xlinker -Map -Xlinker $(APPNAME)-ram.map -o $@ $(SOURCES) 


clean:
	rm -f *.o *.out *.mot *.map


