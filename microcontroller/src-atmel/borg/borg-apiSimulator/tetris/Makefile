CC = gcc
LD = ld
OPTIMIZE = -O2
override CFLAGS = -g -Wall $(OPTIMIZE)

all: tetris.o

#tetris: tetris.o
#	$(CC) $(CFLAGS) tetris.o -o tetris

tetris.o: piece.o playfield.o view.o logic.o
	$(LD) -r piece.o playfield.o view.o logic.o -o tetris.o

piece.o: piece.h piece.c
	$(CC) $(CFLAGS) -c piece.c -o piece.o
	
playfield.o: ../config.h piece.h playfield.h playfield.c
	$(CC) $(CFLAGS) -c playfield.c -o playfield.o

view.o: view.h view.c
	$(CC) $(CFLAGS) -c view.c -o view.o

logic.o: logic.c logic.h piece.h playfield.h
	$(CC) $(CFLAGS) -c logic.c -o logic.o

clean:
	rm -rf *.o tetris
