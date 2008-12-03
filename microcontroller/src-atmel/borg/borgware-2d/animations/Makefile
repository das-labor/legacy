TARGET = libanimations.a
TOPDIR = ..

include $(TOPDIR)/defaults.mk

SRC = programm.c

ifeq ($(ANIMATION_SNAKE),y)
  SRC += snake.c
endif

ifeq ($(ANIMATION_MATRIX),y)
  SRC += matrix.c
endif

ifeq ($(ANIMATION_GAMEOFLIFE),y)
  SRC += gameoflife.c
endif


include $(TOPDIR)/rules.mk
