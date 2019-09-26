 #----------------------------------------------------------------------------
 #  Makefile  
 #----------------------------------------------------------------------------

CC = gcc
AS = gasm

CFLAGS  = -g -O0 -m2 -DMODEL_S -I$(GCC)/include -I$(SEGALIB)/include -I$(SEGASMP)/per/smpclib
ASFLAGS =
LDFLAGS = -T $(LOCATE_FILE) -e $(ENTRY_POINT) -nostartfiles

SATURN  = ../../..
GCC     = $(SATURN)/gccsh
SEGASMP = $(SATURN)/segasmp
SEGALIB = $(SATURN)/segalib

LOCATE_FILE = $(SEGASMP)/lib/saturn.lnk
ENTRY_POINT = START

LIBS=	$(SEGALIB)/lib/sega_sgl.a\
		$(SEGALIB)/lib/sega_per.a\
		$(SEGALIB)/lib/sega_sat.a\
		$(SEGALIB)/lib/sega_cdc.a\
		$(SEGALIB)/lib/sega_adp.a

TARGET=	testtim0

OBJS=	$(SEGASMP)/lib/strt1_g.o\
		$(SEGASMP)/lib/strt2_g.o\
		$(SEGASMP)/v_blank/set_vb.o\
		$(SEGASMP)/v_blank/v_blank.o\
		$(SEGASMP)/per/smpclib/per_x1.o\
		$(SEGASMP)/per/smpclib/per_x2.o\
		$(SEGASMP)/per/smpclib/per_x3.o\
		palette.o \
		fade.o \
		vdp2.o \
		$(TARGET).o

.SUFFIXES:
.SUFFIXES: .cof .o .src .c

all: $(TARGET).cof

$(TARGET).cof:	$(OBJS)
	$(CC) $(LDFLAGS) $(_LDFLAGS) -o $(TARGET).cof -Xlinker -Map -Xlinker $(TARGET).map $(OBJS) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $(_CFLAGS) -o $@ $<

%.o: %.src
	$(AS) $< $(ASFLAGS) $(_ASFLAGS) -o $@

clean:
	del *.o

