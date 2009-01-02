
ifeq ($(GAME_TETRIS),y)
  SUBDIRS += games/tetris
endif

ifeq ($(GAME_SPACE_INVADERS),y)
  SUBDIRS += games/space_invaders
endif

ifeq ($(GAME_SNAKE),y)
  SUBDIRS += games/snake
endif
