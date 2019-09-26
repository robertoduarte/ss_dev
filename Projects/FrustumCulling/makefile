IPFILE = ip.bin
LDFILE = ip.linker

BUILD_TARGET = build
BUILD_FOLDER = $(BUILD_TARGET)/cd
BUILD_COFF = $(BUILD_TARGET)/build.coff
BUILD_ISO = $(BUILD_COFF:.coff=.iso)
BUILD_MAP = $(BUILD_COFF:.coff=.map)

# Find all sources with ".c" extension.
SOURCES = $(patsubst ./%,%,$(shell find src/ -name '*.c'))
OBJECTS = $(SOURCES:.c=.o)

SGLDIR = $(TOOLCHAIN_DIR)/Libraries/SGL_302j
SGLLDIR = $(SGLDIR)/lib
SGLIDIR = $(SGLDIR)/inc

SBLDIR = $(TOOLCHAIN_DIR)/Libraries/sbl601/segalib
SBLLDIR = $(SBLDIR)/lib
SBLIDIR = $(SBLDIR)/include

LIBS = $(SGLLDIR)/SEGA_SYS.A $(SGLLDIR)/LIBCD.A $(SGLLDIR)/LIBSGL.A

CCFLAGS = -W -m2 -std=gnu99 -c -O0 -I$(SGLIDIR)

LDFLAGS = -T$(LDFILE) -Wl,-Map,$(BUILD_MAP),-e,___Start -nostartfiles

%.o : %.c 
	sh-coff-gcc $< $(CCFLAGS) -o $@

compile_objects : $(OBJECTS)
	sh-coff-gcc $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(BUILD_COFF)
	
convert_binary : compile_objects
	sh-coff-objcopy -O binary $(BUILD_COFF) $(BUILD_FOLDER)/0.bin

create_iso : convert_binary
	mkisofs -quiet -sysid "SEGA SATURN" -volid "Demo" -volset "Demo" -publisher "HOMEBREW DEVELOPMENT" -preparer "HOMEBREW DEVELOPMENT" -appid "Demo" -abstract "ABS.TXT" -copyright "CPY.TXT" -biblio "BIB.TXT" -generic-boot $(IPFILE) -full-iso9660-filenames -o $(BUILD_ISO) $(BUILD_FOLDER)
	
clean:
	rm -f $(OBJECTS) $(BUILD_COFF) $(BUILD_ISO) $(BUILD_MAP) $(BUILD_FOLDER)/0.bin

build : create_iso
	
all: clean build
