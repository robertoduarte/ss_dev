# directory
SGLDIR = $(TOOLCHAIN_DIR)/Libraries/SGL_302j
SGLIDR = $(SGLDIR)/INC
SGLLDR = $(SGLDIR)/LIB

# source_program
SRCS = smpsys.c
ASRCS = sys_id.s

OBJS_AREA = $(SGLLDR)/sys_arej.o $(SGLLDR)/sys_aret.o $(SGLLDR)/sys_areu.o $(SGLLDR)/sys_aree.o

# objects code
OBJS_CODE = $(SGLLDR)/sys_sec.o $(OBJS_AREA)

# ip objects
OBJS = $(ASRCS:.s=.o) $(OBJS_CODE) $(SRCS:.c=.o)

TARGET = ip.coff
TARGET1 = ../Common/ip.bin
LDFILE	 = ip_linker.s
MPFILE = $(TARGET:.coff=.map)

CCFLAGS = -O3 -m2 -c -I$(SGLIDR)

# -m2 must be specified in LDFLAGS so the linker will search the SH2 lib dirs
# Specify path of libsgl.a by using "-L" option

LDFLAGS = -m2 -Xlinker -T$(LDFILE) -nostartfiles -Xlinker -Map -Xlinker $(MPFILE) -nostdlib

all: $(TARGET) $(TARGET1)

$(TARGET) : $(OBJS) $(LDFILE)
	sh-coff-gcc $(LDFLAGS) $(OBJS) -o $@

$(TARGET1) : $(OBJS) $(LDFILE)
	sh-coff-objcopy -O binary $(TARGET) $(TARGET1)

%.o : %.c
	sh-coff-gcc $< $(CCFLAGS) -o $@

%.o: %.s
	sh-coff-as $< -o $@

clean:
	rm -f $(ASRCS:.s=.o) $(SRCS:.c=.o) $(TARGET) $(MPFILE)
	