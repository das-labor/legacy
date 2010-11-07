BINNAME = laborlichtctrl

CFILES = main.c usbstuff.c \
		../../rfm12usb/trunk/host/common/opendevice.c \
		../../rfm12usb/trunk/common/usb_id.h \
		../../rfm12usb/trunk/host/CDriver/RfmUsb.c


all:
	gcc -Wall -g -o $(BINNAME) $(CFILES) -lusb `pkg-config --cflags --libs gtk+-2.0` -export-dynamic
clean:
	rm -f $(BINNAME) *.o
