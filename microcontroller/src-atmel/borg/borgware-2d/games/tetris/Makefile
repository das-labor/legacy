TARGET =
TOPDIR = ../..

include $(TOPDIR)/defaults.mk

ifeq ($(GAME_BASTET),y)
  SRC = piece.c playfield.c view.c logic.c input.c highscore.c bast.c
else
  SRC = piece.c playfield.c view.c logic.c input.c highscore.c
endif

include $(TOPDIR)/rules.mk
