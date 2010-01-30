CC = gcc
PROGNAME = alarmctl

LIBS = -lusb

all:
	$(CC) -o $(PROGNAME) $(LIBS) config.c \
		../../rfm12usb/host/common/opendevice.c \
		../../rfm12usb/firmware/usbconfig.h \
		../../rfm12usb/common/usb_id.h \
		../../rfm12usb/host/CDriver/RfmUsb.c \
		main.c


clean:
	rm -rf *.o

