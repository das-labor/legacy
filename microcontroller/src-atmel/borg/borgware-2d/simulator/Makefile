TARGET = libanimations.a
TOPDIR = ..

include $(TOPDIR)/defaults.mk

ifeq ($(findstring CYGWIN,$(OSTYPE)),CYGWIN)
	SRC_SIM = winmain.c eeprom.c
else
	SRC_SIM = main.c trackball.c eeprom.c
endif

include $(TOPDIR)/rules.mk
