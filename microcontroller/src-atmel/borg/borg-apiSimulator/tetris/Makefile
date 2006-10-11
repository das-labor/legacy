CC = gcc
OPTIMIZE = -O2
override CFLAGS = -g -Wall $(OPTIMIZE)

all: piece.o playfield.o view.o logic.o
	
piece.o: piece.h piece.c
	$(CC) $(CFLAGS) -c piece.c
	
playfield.o: ../config.h piece.h playfield.h playfield.c
	$(CC) $(CFLAGS) -c playfield.c

view.o: view.h view.c
	$(CC) $(CFLAGS) -c view.c

logic.o: logic.h logic.c
	$(CC) $(CFLAGS) -c logic.c

clean:
	rm -rf *.o
