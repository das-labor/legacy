
PRG            = borg
OBJ            = main.o util.o pixel.o programm.o borg_can.o scrolltext3.o fonts.o
MCU_OBJ        = borg_hw.o spi.o can.o
SIM_OBJ        = simulator.o

MCU_TARGET     = atmega32
MCU_CLFAGS     = -Os -DF_CPU=16000000

CANADDR        = 0x24

include ../../make/avr.mk


