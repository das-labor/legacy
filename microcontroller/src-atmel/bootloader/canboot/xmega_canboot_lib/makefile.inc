
DIRECTORY      = xmega_canboot_lib
PRG            = bootloader
SRC            = $(DIRECTORY)/bootloader.c $(DIRECTORY)/util.c $(DIRECTORY)/can.c $(DIRECTORY)/main.c $(DIRECTORY)/spi.c 
ASM_OBJ        = $(DIRECTORY)/sp_driver.o
OPTIMIZE       = -Os -fshort-enums

FLASHCMD       = avrdude -p $(MCU_TARGET) -c usbasp -V -U bootloader.hex
FUSECMD        = avrdude -p $(MCU_TARGET) -c usbasp -U hfuse:w:$(HFUSE):m -U lfuse:w:$(LFUSE):m -U efuse:w:$(EFUSE):m

DEFS           = -DF_CPU=16000000
LIBS           =

# You should not have to change anything below here.

CC             = avr-gcc

# Override is only needed by avr-lib build system.

override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS       = -T ./$(DIRECTORY)/avr5.x -Wl,-Map,$(PRG).map,--section-start=.bootloader=$(BOOTLOADER_START)

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(PRG).elf lst text eeprom

$(PRG).elf: $(SRC) $(ASM_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.s
	$(CC) $(CFLAGS) -x assembler-with-cpp -c -o $@ $^

#image.o: $(SRC)
#	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(DIRECTORY)/*.o $(PRG).elf *.eps *.png *.pdf *.bak 
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

flash:
	$(FLASHCMD)

fuse:
	$(FUSECMD)
	
lst:  $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

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
