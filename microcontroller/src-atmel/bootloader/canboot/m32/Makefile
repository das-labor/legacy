
MCU_TARGET       = atmega32
DEFS             = -DF_CPU=16000000
BOOTLOADER_START = 0x7C00

HFUSE = 0xc4
LFUSE = 0x9f
#lock bootloader against self-overwriting
LOCKBYTE = 0x2F

-include canboot_lib/makefile.inc

