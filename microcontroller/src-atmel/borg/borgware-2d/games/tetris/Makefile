TARGET =
TOPDIR = ../..

include $(TOPDIR)/defaults.mk

SRC = tetris_main.c piece.c bucket.c view.c input.c highscore.c

ifeq ($(GAME_TETRIS),y)
  SRC += variant_std.c
endif

ifeq ($(GAME_BASTET),y)
  SRC += variant_bastet.c
endif

ifeq ($(GAME_TETRIS_FP),y)
  ifneq ($(GAME_TETRIS),y)
    SRC += variant_std.c
  endif
  SRC += variant_fp.c
endif

include $(TOPDIR)/rules.mk
