TARGET = libsmallanimations.a
TOPDIR = ..

include $(TOPDIR)/defaults.mk

ifeq ($(SMALLANIMATION_ROWWALK),y)
  SRC += rowwalk.c
endif
ifeq ($(SMALLANIMATION_COLWALK),y)
  SRC += colwalk.c
endif

ifeq ($(SMALLANIMATION_ROWBOUNCE),y)
  SRC += rowbounce.c
endif

ifeq ($(SMALLANIMATION_COLBOUNCE),y)
  SRC += colbounce.c
endif


include $(TOPDIR)/rules.mk
