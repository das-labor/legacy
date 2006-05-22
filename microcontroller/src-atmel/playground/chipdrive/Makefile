
OBJ            = main.o i2csw.c console.o uart.o foo.o function_table.o
CANADDR        = 0

include ../../make/avr.mk

function_table.c:
	./mk_function_table.pl *.h >function_table.c
