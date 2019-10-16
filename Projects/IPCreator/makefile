#
#   SEGA SATURN Graphic library make file for GNU

# macro
CC = sh-coff-gcc
AS = sh-coff-as
LD = ld
RM = rm
CONV = sh-coff-objcopy

# directory
SGLDIR = $(TOOLCHAIN_DIR)/Libraries/SGL_302j
SGLIDR = $(SGLDIR)/INC
SGLLDR = $(SGLDIR)/LIB

OBJECTS = ./objects

# option
CCFLAGS = -O2 -m2 -g -c -I$(SGLIDR)
ASFLAGS =

# -m2 must be specified in LDFLAGS so the linker will search the SH2 lib dirs
# Specify path of libsgl.a by using "-L" option

LDFLAGS = -m2 -Xlinker -T$(LDFILE) -nostartfiles -Xlinker -Map \
          -Xlinker $(MPFILE) -nostdlib
          
DFLAGS =
# source_program
include $(OBJECTS)

TARGET   = ip.coff
TARGET1  = ip.bin
LDFILE	 = ip_linker.s
MPFILE   = $(TARGET1:.bin=.map)
MAKEFILE = Makefile

all: $(TARGET) $(TARGET1)

# Use gcc to link so it will automagically find correct libs directory

$(TARGET) : $(OBJS) $(MAKEFILE) $(OBJECTS) $(LDFILE)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(TARGET1) : $(OBJS) $(MAKEFILE) $(LDFILE)
	$(CONV) -O binary $(TARGET) $(TARGET1)

# suffix
.SUFFIXES: .asm

%.o: %.src
	$(AS) $< $(ASFLAGS) $(_ASFLAGS) -o $@

%.o: %.c
	$(CC) $< $(DFLAGS) $(CCFLAGS) -o $@

clean:
	$(shell find . -type f -name '*.o' -delete)
	