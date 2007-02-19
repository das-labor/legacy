PRG = borgSim
#OBJ = main.o pixel.o programm.o util.o  scrolltext3.o  trackball.o  breakpoint.o tetris/tetris.o
OBJ = main.o pixel.o programm.o util.o  scrolltext3.o  trackball.o  breakpoint.o

OPTIMIZE = -O2
LIBS = -lglut -lpthread -lGL -lGLU
CC = gcc
LD = ld

OPTIMIZE = -O2
override CFLAGS        = -g -Wall $(OPTIMIZE)
$(DEFS)
override LDFLAGS       = -Wl,-Map,$(PRG).map

all: $(PRG) 

$(PRG): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

.PHONY: tetris/tetris.o
tetris/tetris.o:
	$(MAKE) "CC=$(CC)" "LD=$(LD)" "CFLAGS=$(CFLAGS)" --directory=tetris

clean:
	rm -rf *.o $(PRG) $(PRG).map
	$(MAKE) "CC=$(CC)" "LD=$(LD)" "CFLAGS=$(CFLAGS)" --directory=tetris clean