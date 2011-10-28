TARGET = objects
TOPDIR = ..

include $(TOPDIR)/defaults.mk

SRC = scrolltext3.c

SRC += $(shell echo $(SCROLLTEXT_FONT) | tr A-Z a-z).c

include $(TOPDIR)/rules.mk
