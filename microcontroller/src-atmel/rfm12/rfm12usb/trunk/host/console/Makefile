CC = gcc
PROGNAME = rfmconsole

LIBS = -lusb

all:
	$(CC) -o $(PROGNAME) ../common/opendevice.c ../CDriver/RfmUsb.c dump.c termio.c console.c $(LIBS)

clean:
	rm -rf *.o

