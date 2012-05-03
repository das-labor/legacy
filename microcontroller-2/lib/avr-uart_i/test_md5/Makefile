PRG            = main-uart_i-test
OBJ            = main-uart_i-test.o uart_i-asm.o circularbytebuffer-asm.o md5-asm.o
                 
MCU_TARGET     = atmega644
OPTIMIZE       = 
PROGRAMMER     = avr911
FLASHCMD       = avrdude -p $(MCU_TARGET) -P /dev/ttyUSB0 -c $(PROGRAMMER) -U flash:w:$(PRG).hex  
ERASECMD       = 
EEPROM_FLASHCMD = avrdude -p $(MCU_TARGET) -P /dev/ttyUSB0 -c $(PROGRAMMER) -U eeprom:w:$(PRG)_eeprom.hex

#uisp -dprog=bsd -dlpt=/dev/parport1 --erase

DEFS           = -DF_CPU=20000000 

LIBS           =

# You should not have to change anything below here.

CC             = avr-gcc
AS             = avr-as

# Override is only needed by avr-lib build system.



override CFLAGS        = -g -pedantic -std=c99 -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS) -morder1
# -fnew-ra
override LDFLAGS       = -Wl,-Map,$(PRG).map
override ASFLAGS       = -mmcu=$(MCU_TARGET)

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(PRG).elf lst text eeprom


$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak 
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

flash: $(PRG).hex
	$(ERASECMD)
	$(FLASHCMD)
#	$(EEPROM_FLASHCMD)

eeflash:
	$(EEPROM_FLASHCMD)


lst:  $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@
%.s: %.c
	$(CC) $(CFLAGS) -S $<
%.s: %.S
	$(CC) $(CFLAGS) -o $@ -S $<

# Rules for building the .text rom images

text: hex bin srec

hex:  $(PRG).hex
bin:  $(PRG).bin
srec: $(PRG).srec

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@
	
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

docu:
	doxygen

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
