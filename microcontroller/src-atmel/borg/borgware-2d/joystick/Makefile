TARGET =
TOPDIR = ..

include $(TOPDIR)/defaults.mk

SRC  = joystick.c
SRC_SIM =

ifeq ($(JOYSTICK_TYPE), JOYSTICK_NES)
  SRC = nes_pad.c
endif


include $(TOPDIR)/rules.mk
