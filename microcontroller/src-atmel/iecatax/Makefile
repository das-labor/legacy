
AVRXHOME       = ../lib/AvrX2.6f/avrx/
SRC            = main.c iec_hw.c iec.c fifo.c AvrXBufferedSerial.c
MCU_TARGET     = atmega162

FLASHCMD      = avrdude -p m162 -c xa1541 -D -U flash:w:$(OUT).hex
ERASECMD      = avrdude -p m162 -c xa1541 -e
DEFS          = -DF_CPU=8000000

include ../make/avrx.mk
