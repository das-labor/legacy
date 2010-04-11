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

ifeq ($(ANIMATION_STONEFLY),y) 
  SRC += stonefly.c 
endif 

ifeq ($(ANIMATION_FLYINGDOTS),y) 
  SRC += flyingdots.c 
endif 

ifeq ($(ANIMATION_GAMEOFLIFE),y)
  SRC += gameoflife.c
endif

ifeq ($(ANIMATION_BREAKOUT),y)
  SRC += breakout_demo.c
endif

ifeq ($(ANIMATION_LTN_ANT),y)
  SRC += ltn_ant.c
endif

include $(TOPDIR)/rules.mk
