PRG = borgSim
TETRIS_OBJ = tetris/piece.o tetris/playfield.o tetris/view.o tetris/logic.o
OBJ = main.o pixel.o programm.o util.o  scrolltext3.o  trackball.o  breakpoint.o

OPTIMIZE = -O2
LIBS = -lglut -lpthread -lGL -lGLU
CC = gcc

OPTIMIZE = -O2
override CFLAGS        = -g -Wall $(OPTIMIZE)
$(DEFS)
override LDFLAGS       = -Wl,-Map,$(PRG).map

all: $(PRG) 

$(PRG): $(OBJ) $(TETRIS_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

.PHONY: $(TETRIS_OBJ)
$(TETRIS_OBJ):
	$(MAKE) "CC=$(CC)" "CFLAGS=$(CFLAGS)" --directory=tetris

clean:
	rm -rf *.o $(PRG) $(PRG).map
	$(MAKE) "CC=$(CC)" "CFLAGS=$(CFLAGS)" --directory=tetris clean
	