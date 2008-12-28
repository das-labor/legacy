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

# flags for the compiler
CFLAGS ?= -Wall -W -Wno-unused-parameter -Wno-sign-compare
CFLAGS += -g -Os -std=gnu99 -fgnu89-inline

# flags for the linker
LDFLAGS += -mmcu=$(MCU)


#############################################################################
#Settings for Simulator build

OSTYPE = $(shell echo $$OSTYPE)
#$(info $(OSTYPE))

ifeq ($(OSTYPE),cygwin)  
  CFLAGS_SIM  = -g -Wall -pedantic -std=c99 -O2 -D_WIN32 -mno-cygwin
  LDFLAGS_SIM = -Wl -mno-cygwin -T simulator/i386pe.x
	LIBS_SIM    = -lglut32 -lglu32 -lopengl32
else
  CFLAGS_SIM  = -g -Wall -pedantic -std=c99 -O2
  LDFLAGS_SIM = -Wl
	LIBS_SIM    = -lglut -lpthread -lGL -lGLU
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

