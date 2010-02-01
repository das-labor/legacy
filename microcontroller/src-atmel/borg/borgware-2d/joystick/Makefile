TARGET =
TOPDIR = ..

include $(TOPDIR)/defaults.mk

SRC_SIM =

ifeq ($(NES_PAD_SUPPORT), y)
  SRC = nes_pad.c
endif
ifeq ($(PARALLEL_JOYSTICK_SUPPORT), y)
  SRC  = joystick.c
endif

include $(TOPDIR)/rules.mk
