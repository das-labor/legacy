CC = gcc
LD = ld
OPTIMIZE = -O0
override CFLAGS = -g -Wall -pedantic -std=c99 $(OPTIMIZE)

all: tetris.o

#tetris: tetris.o
#	$(CC) $(CFLAGS) tetris.o -o tetris

tetris.o: piece.o playfield.o view.o logic.o input.o
	$(LD) -r piece.o playfield.o view.o logic.o input.o -o tetris.o

piece.o: piece.c piece.h
	$(CC) $(CFLAGS) -c piece.c -o piece.o
	
playfield.o: playfield.c playfield.h piece.h
	$(CC) $(CFLAGS) -c playfield.c -o playfield.o

view.o: view.c view.h ../pixel.h
	$(CC) $(CFLAGS) -c view.c -o view.o

logic.o: logic.c logic.h piece.h playfield.h input.h view.h
	$(CC) $(CFLAGS) -c logic.c -o logic.o

input.o: input.c input.h ../joystick.h ../util.h
	$(CC) $(CFLAGS) -c input.c -o input.o

clean:
	rm -rf *.o
