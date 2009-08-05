
OBJ = main.o spike_hw.o mmc.o ff.o mylib.o

LM32_CC=lm32-elf-gcc
LM32_LD=lm32-elf-gcc
LM32_OBJCOPY=lm32-elf-objcopy
LM32_OBJDUMP=lm32-elf-objdump

SREC2VRAM ?= ../../tools/srec2vram/srec2vram
VRAMFILE=image.ram

CPU_FLAGS=-mbarrel-shift-enabled -mmultiply-enabled -msign-extend-enabled \
          -mdivide-enabled

CFLAGS=-MMD -Os -g2 -Wall -s -fomit-frame-pointer $(CPU_FLAGS)

#CFLAGS=-MMD -O2 -Wall -g -s -fomit-frame-pointer -mbarrel-shift-enabled   -mmultiply-enabled -mdivide-enabled -msign-extend-enabled
#LDFLAGS=-nostdlib -nodefaultlibs -Tlinker.ld 
#LDFLAGS=$(CPU_FLAGGS) -Tlinker.ld -O1 -lm -lgcc -lc -lnosys -Wl,--gc-sections
LDFLAGS=$(CPU_FLAGGS) -Tram.ld -lc

SEGMENTS = -j .text -j .rodata -j .data

all: image.srec

crt0ram.o: crt0ram.S
	$(LM32_CC) $(CFLAGS) -c crt0ram.S

main.o: main.c
	$(LM32_CC) $(CFLAGS) -c main.c

%.o: %.c
	$(LM32_CC) $(CFLAGS) -c $<

image: crt0ram.o $(OBJ) ram.ld Makefile
	$(LM32_LD) $(LDFLAGS) -o image crt0ram.o $(OBJ)

#	$(LM32_LD) $(LDFLAGS) -o image crt0ram.o main.o spike_hw.o

image.lst: image
	$(LM32_OBJDUMP) -h -S $< > $@

image.bin: image
	$(LM32_OBJCOPY) $(SEGMENTS) -O binary image image.bin

image.srec: image image.lst
	$(LM32_OBJCOPY) $(SEGMENTS) -O srec image image.srec

$(VRAMFILE): image.srec
	$(SREC2VRAM) image.srec 0x40000000 0x10000 > $(VRAMFILE)

clean:
	rm -f image image.lst image.bin image.srec image.map  *.o *.d

upload: image.srec
	../../tools/uploader/upload.rb -s /dev/ttyS0 -t $<

DEPS := $(wildcard *.d)
ifneq ($(DEPS),)
include $(DEPS)
endif
