PRG            = bootloader
SRC            = canboot_lib/bootloader.c canboot_lib/util.c canboot_lib/can.c canboot_lib/main.c $(USER_SRC)
OPTIMIZE      ?= -Os -fshort-enums -ffunction-sections -fdata-sections -mstrict-X -maccumulate-args -fno-tree-loop-optimize -fno-move-loop-invariants

FLASHCMD       = avrdude -p $(MCU_TARGET) -c usbasp -V -U bootloader.hex
FUSECMD        = avrdude -p $(MCU_TARGET) -c usbasp -U hfuse:w:$(HFUSE):m -U lfuse:w:$(LFUSE):m -U efuse:w:$(EFUSE):m
LOCKCMD        = avrdude -p $(MCU_TARGET) -c usbasp -U lock:w:$(LOCKBYTE):m

DEFS           = -DF_CPU=16000000
LIBS           =

# You should not have to change anything below here.

CC             = avr-gcc

SIZE	       = avr-size

# Override is only needed by avr-lib build system.

override CFLAGS        = -g -Wall -pedantic $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS) -std=gnu99
override LDFLAGS       = -T ./canboot_lib/avr5.x -Wl,-Map,$(PRG).map,--section-start=.bootloader=$(BOOTLOADER_START),--relax,--gc-sections,--print-gc-sections

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

# Display size of file.
HEXSIZE = $(SIZE) --target=ihex $(PRG).hex
ELFSIZE = $(SIZE) -A $(PRG).elf

all: sizebefore $(PRG).elf lst text eeprom sizeafter

$(PRG).elf: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

#image.o: $(SRC)
#	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

flash:
	$(FLASHCMD)

fuse:
	$(FUSECMD)

lock:
	$(LOCKCMD)

lst:  $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@


sizebefore:
	@if test -f $(PRG).elf; then echo; echo Size before:; $(ELFSIZE); \
	echo; fi

sizeafter:
	@if test -f $(PRG).elf; then echo; echo Size after:; $(ELFSIZE); \
	echo; fi


# Rules for building the .text rom images

text: hex bin srec

hex:  $(PRG).hex
bin:  $(PRG).bin
srec: $(PRG).srec

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -j .bootloader -j .progdata -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -j .bootloader -j .progdata -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -j .bootloader -j .progdata -O binary $< $@

# Rules for building the .eeprom rom images

eeprom: ehex ebin esrec

ehex:  $(PRG)_eeprom.hex
ebin:  $(PRG)_eeprom.bin
esrec: $(PRG)_eeprom.srec

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

%_eeprom.srec: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@

%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@

# Every thing below here is used by avr-libc's build system and can be ignored
# by the casual user.

FIG2DEV                 = fig2dev
EXTRA_CLEAN_FILES       = *.hex *.bin *.srec

dox: eps png pdf

eps: $(PRG).eps
png: $(PRG).png
pdf: $(PRG).pdf

%.eps: %.fig
	$(FIG2DEV) -L eps $< $@

%.pdf: %.fig
	$(FIG2DEV) -L pdf $< $@

%.png: %.fig
	$(FIG2DEV) -L png $< $@
