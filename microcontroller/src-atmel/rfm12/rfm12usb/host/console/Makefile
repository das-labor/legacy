CC = gcc
PROGNAME = rfmconsole

LIBS = -lusb

all:
	$(CC) -o $(PROGNAME) $(LIBS) ../common/opendevice.c dump.c termio.c console.c

clean:
	rm -rf *.o

