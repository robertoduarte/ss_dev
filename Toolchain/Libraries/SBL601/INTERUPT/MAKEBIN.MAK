 #----------------------------------------------------------------------------
 #  Makefile  
 #----------------------------------------------------------------------------

CC = gcc -c 
AS = as
LD = ld
AR = ar
CPP = cpp
RM = rm

SATURN         = ../../..
GCC            = $(SATURN)/gccsh
SEGASMP        = $(SATURN)/segasmp
SEGALIB        = $(SATURN)/segalib

GLOBAL_DEFS = -D_SH -DMODEL_S
OPTIMIZE_FLAGS = -finline-functions -fomit-frame-pointer
DEBUG_FLAGS = -g
EXTRA_CFLAGS = -m2 -O $(GLOBAL_DEFS)
EXTRA_ASFLAGS =

CFLAGS = $(EXTRA_CFLAGS)

INCLUDES =  -I$(GCC)/include -I$(SEGALIB)/include -I$(SEGASMP)/per/smpclib

ASFLAGS = $(EXTRA_ASFLAGS) 

LDFLAGS =

GNU_LIBS = -Lh:/gnu2.7/lib -lm -lgcc -lc -lgcc
SOJ_LIBS = $(SEGALIB)/lib/sega_sgl.a \
		$(SEGALIB)/lib/sega_sat.a \
		$(SEGALIB)/lib/sega_gfs.a \
		$(SEGALIB)/lib/sega_cdc.a \
		$(SEGALIB)/lib/sega_adp.a \
		$(SEGALIB)/lib/sega_per.a

TARGET= testtim0

OBJS = $(SEGASMP)/lib/strt1_g.o\
		$(SEGASMP)/lib/strt2_g.o\
		$(SEGASMP)/v_blank/set_vb.o\
		$(SEGASMP)/v_blank/v_blank.o\
		../../per/smpclib/per_x1.o\
		../../per/smpclib/per_x2.o\
		../../per/smpclib/per_x3.o\
		fade.o \
		palette.o \
		vdp2.o \
		$(TARGET).o

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.s
	$(AS) $(ASFLAGS) $(INCLUDES) -o $@ $<

%.sc: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -g -S -o $*.sc $<

%.lst: %.sc
	$(AS) $(ASFLAGS) $(INCLUDES) -ahld $< > $*.lst


all: testtim0

objects: $(OBJS)

testtim0: $(OBJS)
	$(LD) -o $@.sre -T lnk_sre.x $(OBJS) $(SOJ_LIBS) $(GNU_LIBS) -Map testtim0.map
	sretobin testtim0.sre testtim0.bin
clean:
	$(RM) $(OBJS)

