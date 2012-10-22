# ethersex project specific defaults, each can be overridden in config.mk
F_CPU = 20000000UL
MCU = atmega644

CC=avr-gcc
AR=avr-ar
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AS = avr-as
CP = cp
RM = rm -f
AVRDUDE = avrdude
AVRDUDE_BAUDRATE = 115200
SIZE = avr-size
STRIP = avr-strip

HOSTCC := gcc
export HOSTCC

LIBS = -lm

# flags for the compiler
CFLAGS ?= -Wall -W -Wno-unused-parameter -Wno-sign-compare
CFLAGS += -g -Os -std=gnu99 -fgnu89-inline -D_XOPEN_SOURCE=600 -DNDEBUG

# flags for the linker
LDFLAGS += -T ./avr5.x -Wl,-Map,image.map -mmcu=$(MCU)


#############################################################################
#Settings for Simulator build

OSTYPE = $(shell echo $$OSTYPE)
MACHINE = $(shell uname -m)
#$(info $(OSTYPE))

ifeq ($(OSTYPE),cygwin)  
  CFLAGS_SIM  = -g -Wall -pedantic -std=c99 -O0 -D_WIN32 -D_XOPEN_SOURCE=600
  LDFLAGS_SIM = -T simulator/i386pe.x
  LIBS_SIM    = -lgdi32 -lwinmm -lm
else
  CFLAGS_SIM  = -g -Wall -pedantic -std=c99 -O0 -D_XOPEN_SOURCE=600
  ifeq ($(MACHINE),x86_64)
    LDFLAGS_SIM = -g -T simulator/elf_x86_64.x
  else
    LDFLAGS_SIM = -T simulator/elf_i386.x
  endif
  LIBS_SIM    = -lglut -lpthread -lGL -lGLU -lm
endif

##############################################################################
# the default target
$(TARGET):

##############################################################################
# include user's config.mk file

$(TOPDIR)/config.mk: 
	@echo "# Put your own config here!" > $@
	@echo "#F_CPU = $(F_CPU)" >> $@
	@echo "#MCU = $(MCU)" >> $@
	@echo "created default config.mk, tune your settings there!"
-include $(TOPDIR)/config.mk


##############################################################################
# configure load address for bootloader, if enabled
#
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),mrproper)
ifneq ($(MAKECMDGOALS),menuconfig)  

include $(TOPDIR)/.config

CPPFLAGS += -DF_CPU=$(FREQ)UL -mmcu=$(MCU)

endif # MAKECMDGOALS!=menuconfig
endif # MAKECMDGOALS!=mrproper
endif # MAKECMDGOALS!=clean

ifeq ($(BOOTLOADER_SUPPORT),y)  
LDFLAGS += -Wl,--section-start=.text=0xE000
CFLAGS  += -mcall-prologues
endif


%.s: %.c
	$(CC) -o $@ -O0 $(CPPFLAGS) -S $<

%.E: %.c
	$(CC) -o $@ -O0 $(CPPFLAGS) -C -E -dD $<

