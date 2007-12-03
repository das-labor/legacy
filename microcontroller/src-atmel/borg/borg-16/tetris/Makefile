LD = avr-ld

all: tetris.o

tetris.o: piece.o playfield.o view.o logic.o input.o
	$(LD) -r piece.o playfield.o view.o logic.o input.o -o tetris.o

piece.o: piece.c piece.h
	$(MCU_CC) $(CFLAGS) -c piece.c -o piece.o

playfield.o: playfield.c playfield.h piece.h
	$(MCU_CC) $(CFLAGS) -c playfield.c -o playfield.o

view.o: view.c view.h logic.h piece.h playfield.h ../config.h ../pixel.h \
  ../util.h ../scrolltext.h
	$(MCU_CC) $(CFLAGS) -c view.c -o view.o

logic.o: logic.c logic.h piece.h playfield.h input.h view.h
	$(MCU_CC) $(CFLAGS) -c logic.c -o logic.o

input.o: input.c input.h ../joystick.h ../util.h
	$(MCU_CC) $(CFLAGS) -c input.c -o input.o

clean:
	rm -rf *.o *.d
