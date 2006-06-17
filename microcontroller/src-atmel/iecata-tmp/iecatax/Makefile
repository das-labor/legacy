
AVRXHOME       = ../lib/AvrX2.6f/avrx/
SRC            = main.c iec_hw.c iec.c fifo.c AvrXBufferedSerial.c dos-init.c dos-file.c dos-dir.c ata.c iecata.c
MCU_TARGET     = atmega162

FLASHCMD      = myavrdude -p m162 -c xa1541 -D -U flash:w:$(OUT).hex
ERASECMD      = myavrdude -p m162 -c xa1541 -e
DEFS          = -DF_CPU=8000000

LDFLAGS       = -Wl,--section-start=.bss=0x800500,-Map,$(PRG).map

include ../make/avrx.mk
