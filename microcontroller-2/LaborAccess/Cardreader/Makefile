
AVRXHOME       = ../../lib/AvrX2.6f/avrx/
SRC            = main.c hd44780.c reader.c i2csw.c foo.c panel.c AvrXBufferedSerial.c console.c function_table.c
MCU_TARGET     = atmega8
DEFS           = -DF_CPU=2000000

INDEX_H        = foo.h


include ../../make/avrx.mk

function_table.c:
	./mk_function_table.pl ${INDEX_H} >function_table.c
