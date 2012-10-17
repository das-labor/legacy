TARGET = libanimations.a
TOPDIR = ..

include $(TOPDIR)/defaults.mk

ifeq ($(OSTYPE),cygwin)  
	SRC_SIM = winmain.c eeprom.c
else
	SRC_SIM = main.c trackball.c eeprom.c
endif

include $(TOPDIR)/rules.mk
