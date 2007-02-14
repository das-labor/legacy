LM32_CC=lm32-elf-gcc
LM32_LD=lm32-elf-ld
LM32_OBJCOPY=lm32-elf-objcopy
LM32_OBJDUMP=lm32-elf-objdump

SRECTOVHDL=~/bin/srectovhdl

CFLAGS=-O2 -Wall -s -fomit-frame-pointer -mbarrel-shift-enabled -mmultiply-enabled -mdivide-enabled -msign-extend-enabled
LDFLAGS=-Tlinkcmd
SEGMENTS = -j .text -j .data

all: image.vhdl

crt0ram.o: crt0ram.S
	$(LM32_CC) $(CFLAGS) -c crt0ram.S

main.o: main.c
	$(LM32_CC) $(CFLAGS) -c main.c

spike_hw.o: spike_hw.c
	$(LM32_CC) $(CFLAGS) -c spike_hw.c

image: crt0ram.o main.o spike_hw.o
	$(LM32_LD) $(LDFLAGS) -Map image.map -N -o image crt0ram.o main.o spike_hw.o

image.lst: image
	$(LM32_OBJDUMP) --disassemble image > image.lst

image.bin: image
	$(LM32_OBJCOPY) $(SEGMENTS) -O srec image image.bin

image.srec: image
	$(LM32_OBJCOPY) $(SEGMENTS) -O srec image image.srec

image.vhdl: image.srec
	$(SRECTOVHDL) image.srec image.vhdl > /dev/null

clean:
	rm -f image image.lst image.bin image.srec image.vhdl *.o

