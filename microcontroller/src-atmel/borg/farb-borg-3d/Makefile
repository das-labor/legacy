
OBJ            = farbe.o uart.o util.o writeram.o
CANADDR        = 0x3d

FLASHCMD = avrdude -c avr910 -p m32 -P /dev/tty.SLAB_USBtoUART -e -U flash:w:image.hex
ERASECMD =

include ../../make/avr.mk

