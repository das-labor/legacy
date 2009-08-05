LM32_CC=lm32-elf-gcc
LM32_LD=lm32-elf-gcc
LM32_OBJCOPY=lm32-elf-objcopy
LM32_OBJDUMP=lm32-elf-objdump
OBJ=crt0ram.o main.o testAnim.o spike_hw.o api.o util.o
CPU_FLAGS=-mbarrel-shift-enabled -mmultiply-enabled -msign-extend-enabled \
	   -mdivide-enabled
CFLAGS=-MMD -O1 -ffunction-sections -g2 -w -g -s -fomit-frame-pointer $(CPU_FLAGS) 
LDFLAGS=$(CPU_FLAGGS) -Tlinker.ld -O1 -lm -lgcc -lc -lnosys -Wl,--gc-sections
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


upload: image.srec
	../../tools/uploader/upload.rb -s /dev/ttyUSB0 -t $<

clean:
	rm -f srectovhdl image image.lst image.bin image.srec image.map  *.o *.d

DEPS := $(wildcard *.d)
ifneq ($(DEPS),)
include $(DEPS)
endif
