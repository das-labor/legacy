CC = gcc
PROGNAME = alarmctl

LIBS = -lusb

all:
	$(CC) -o $(PROGNAME) $(LIBS) config.c \
		../../rfm12usb/trunk/host/common/opendevice.c \
		../../rfm12usb/trunk/common/usb_id.h \
		../../rfm12usb/trunk/host/CDriver/RfmUsb.c \
		main.c


clean:
	rm -rf *.o

