PRG            = main-huffman-test
OBJ            =  main-huffman-test.o cli-stub.o cli-basics.o cli-hexdump.o string-extras-asm.o \
                  uart.o avr-huffman-decode-asm.o dump-cstub.o dump-asm.o \
                  hexdigit_tab.o data/test.txt.hfm.elf \
                  data/hacker_manifesto_borg.txt.hfm.elf data/COPYING.gpl3.hfm.elf
                 
MCU_TARGET     = atmega644
OPTIMIZE       = 
PROGRAMMER     = stk500v2
FLASHCMD       = avrdude -p $(MCU_TARGET) -P /dev/ttyUSB0 -c $(PROGRAMMER) -U flash:w:$(PRG).hex  
ERASECMD       = 
EEPROM_FLASHCMD = avrdude -p $(MCU_TARGET) -P /dev/ttyUSB0 -c $(PROGRAMMER) -U eeprom:w:$(PRG)_eeprom.hex

#uisp -dprog=bsd -dlpt=/dev/parport1 --erase

DEFS           = -DF_CPU=16000000 -DATMEGA644

LIBS           =

# You should not have to change anything below here.

CC             = avr-gcc
AS             = avr-as

# Override is only needed by avr-lib build system.



override CFLAGS        = -g -pedantic -std=c99 -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS) -morder1
# -fnew-ra
override LDFLAGS       = -Wl,-Map,$(PRG).map
override ASFLAGS       = -mmcu=$(MCU_TARGET)

HUFFMAN_ENC    = ./host/huffman-encode
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
	$(EEPROM_FLASHCMD)

eeflash:
	$(EEPROM_FLASHCMD)


lst:  $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

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

%.hfm: %
	$(HUFFMAN_ENC) $<

%.hfm.elf: %.hfm
	$(OBJCOPY) --rename-section .data=.progmem.data -I binary -O elf32-avr $< $@
	
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
