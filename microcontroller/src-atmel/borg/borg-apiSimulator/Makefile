PRG = borgSim
OBJ = main.o pixel.o joystick.o programm.o util.o  scrolltext3.o \
  trackball.o breakpoint.o menu.o invaders2.o invader_draw.o \
  invader_proc.o invader_init.o tetris/tetris.o font_arial8.o
#OBJ = main.o pixel.o joystick.o programm.o util.o  scrolltext3.o  trackball.o  breakpoint.o

OPTIMIZE = -O0
LIBS = -lglut -lpthread -lGL -lGLU
CC = gcc
LD = ld

OPTIMIZE = -O0
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