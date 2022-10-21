ifeq ($(strip $(YAUL_INSTALL_ROOT)),)
  $(error Undefined YAUL_INSTALL_ROOT (install root directory))
endif

include $(YAUL_INSTALL_ROOT)/share/build.pre.mk

SH_PROGRAM:= build
SH_SRCS+= $(patsubst ./%,%,$(shell find src/ -name '*.c'))
SH_SRCS+= $(patsubst ./%,%,$(shell find src/ -name '*.cxx'))

SH_LIBRARIES:= 
SH_CXXFLAGS= -O3 -std=c++23 -I. -save-temps=obj -ggdb3 -Wno-unused-variable -Wno-unused-parameter -Wno-array-bounds -Wno-error=volatile -Wno-volatile -Wno-register

IP_VERSION:= V1.000
IP_RELEASE_DATE:= 20160101
IP_AREAS:= JTUBKAEL
IP_PERIPHERALS:= JAMKST
IP_TITLE:= VDP2 DBGIO menu test
IP_MASTER_STACK_ADDR:= 0x06004000
IP_SLAVE_STACK_ADDR:= 0x06001E00
IP_1ST_READ_ADDR:= 0x06004000
IP_1ST_READ_SIZE:= 0

include $(YAUL_INSTALL_ROOT)/share/build.post.iso-cue.mk
