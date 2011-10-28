TARGET = libcan.a
TOPDIR = ..

include $(TOPDIR)/defaults.mk

ifeq ($(CAN_SUPPORT),y)
  SRC = can.c
  SRC += borg_can.c
  SRC += lap.c
  SRC += spi.c
endif

include $(TOPDIR)/rules.mk
