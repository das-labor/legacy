# $Id: Makefile,v 1.1 2006/10/28 12:40:42 rschaten Exp $

CC              = gcc
LIBUSB_CONFIG   = libusb-config
# Make sure that libusb-config is in the search path or specify a full path. On
# Windows, there is no libusb-config and you must configure the options below
# manually. See examples.

CFLAGS          = `$(LIBUSB_CONFIG) --cflags` -O -Wall -I../common

USBLIBS         = `$(LIBUSB_CONFIG) --libs`
XSPLIBS         = -lX11 -lm

all: usb-servo 

usb-servo: usb-servo.o usbdrv.o
	$(CC) $(CFLAGS) -o usb-servo usb-servo.o usbdrv.o $(USBLIBS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c -o $*.o

clean:
	rm -f usb-servo *.o
