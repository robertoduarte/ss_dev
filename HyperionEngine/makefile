ifeq ($(strip $(YAUL_INSTALL_ROOT)),)
  $(error Undefined YAUL_INSTALL_ROOT (install root directory))
endif

include $(YAUL_INSTALL_ROOT)/share/pre.common.mk

SH_PROGRAM:= build
SH_SRCS+= $(patsubst ./%,%,$(shell find src/ -name '*.c'))

ROMDISK_SYMBOLS= root
ROMDISK_DIRS= romdisk

SH_LIBRARIES:= 
SH_CFLAGS= -O3 -std=c17 -I.

IP_VERSION:= V1.000
IP_RELEASE_DATE:= 20211001
IP_AREAS:= JTUBKAEL
IP_PERIPHERALS:= JAMKST
IP_TITLE:= HYPERION ENGINE
IP_MASTER_STACK_ADDR:= 0x06004000
IP_SLAVE_STACK_ADDR:= 0x06001000
IP_1ST_READ_ADDR:= 0x06004000

M68K_PROGRAM:=
M68K_OBJECTS:=

include $(YAUL_INSTALL_ROOT)/share/post.common.mk
