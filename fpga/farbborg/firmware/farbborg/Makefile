LM32_CC=lm32-elf-gcc
LM32_LD=lm32-elf-gcc
LM32_OBJCOPY=lm32-elf-objcopy
LM32_OBJDUMP=lm32-elf-objdump
OBJ=crt0ram.o main.o api.o slh.o ff.o mmc.o fileParser.o colorMatrix.o colorSnake.o \
    anim_Benny.o anim_took.o anim_police.o testAnim.o displayLoop.o plasmaAnims.o spike_hw.o util.o
CPU_FLAGS=-mbarrel-shift-enabled -mmultiply-enabled -msign-extend-enabled -mdivide-enabled
#CPU_FLAGS=-mbarrel-shift-enabled -mmultiply-enabled
CFLAGS=-MMD -O2 -ffunction-sections -g2 -g -Wall -D__lm32__ -fomit-frame-pointer $(CPU_FLAGS) 
LDFLAGS=$(CPU_FLAGS) -Tram.ld -lm -lc -lgcc -Wl,--gc-sections
SEGMENTS = -j .text -j .data -j .rodata

all: image.bin image.lst image.srec Makefile

%.o: %.S
	$(LM32_CC) $(CFLAGS) -c $<

%.o: %.c
	$(LM32_CC) $(CFLAGS) -c $<

image: $(OBJ)
	$(LM32_LD) $(LDFLAGS) -o image $(OBJ)

image.lst: image
	$(LM32_OBJDUMP) -h -S $< > $@

image.bin: image
	$(LM32_OBJCOPY) $(SEGMENTS) -O binary image image.bin

image.srec: image image.lst
	$(LM32_OBJCOPY) $(SEGMENTS) -O srec image image.srec


sdcard: image.bin
	cp image.bin /Volumes/NO\ NAME/sys/firmware.bin
#	umount /Volumes/NO\ NAME

upload: image.srec
	../../tools/uploader/upload.rb -s /dev/tty.usbserial -t $<

clean:
	rm -f srectovhdl image image.lst image.bin image.srec image.map  *.o *.d

DEPS := $(wildcard *.d)
ifneq ($(DEPS),)
include $(DEPS)
endif

