CC = gcc
PROGNAME = nakkaflash

LIBS = -lusb

all:
	$(CC) -o $(PROGNAME) $(LIBS) ../common/opendevice.c main.c ../../../rfm12usb/host/CDriver/RfmUsb.c config.c

clean:
	rm -rf *.o

