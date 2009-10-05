PRG            = bootloader
OBJ            = bootloader.o util.o can.o
MCU            = atmega644
OPTIMIZE       = -Os -fshort-enums
SIZE = avr-size

# this is for atmega128
#BOOTLOADER_START = 0x1E000

# this is for atmega64
#BOOTLOADER_START = 0xE000

# this is for atmega32
#BOOTLOADER_START = 
#this is for atmega644 0x7C00 * 2
BOOTLOADER_START = 0xf800
PROGDATA_START = 0xfc6e
PROGDATA2_START = 0xfc6e

FLASHCMD       = uisp -dprog=bsd -dlpt=/dev/parport0 --upload if=$(PRG).hex
ERASECMD       = uisp -dprog=bsd -dlpt=/dev/parport0 --erase
FUSECMD        = uisp -dprog=bsd --wr_fuse_h=0xcc

DEFS           = -DF_CPU=16000000UL
LIBS           =

# You should not have to change anything below here.

CC             = avr-gcc

# Override is only needed by avr-lib build system.

override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU) $(DEFS) 
override LDFLAGS       = -Wl,-Map,$(PRG).map,--section-start=.text=$(BOOTLOADER_START)

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(PRG).elf lst text eeprom
	@echo "==============================="
	@echo "compiled for: $(MCU)"
	@echo -n "bootloader size is: "
	@$(SIZE) -A $(PRG).hex | grep "\.sec1" | tr -s " " | cut -d" " -f2
	@echo "==============================="

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak 
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

fct_table:
	./mk_function_table.pl *.h >function_table.c

flash:
	$(ERASECMD)
	sleep 1
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
