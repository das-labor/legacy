
MCU_TARGET     = atmega8
OBJ            = main.o uart.o util.o can_raw.o can-encap.o spi.o can-uart.o

DEFS           = -DF_CPU=18432000

include make/avr.mk

fuse:
	uisp -dprog=bsd --wr_fuse_h=0xc9
	uisp -dprog=bsd --wr_fuse_l=0x9f

