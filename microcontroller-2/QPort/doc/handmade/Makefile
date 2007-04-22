PRG            = qport

FLASHCMD       = avrdude -p $(MCU_TARGET) -P /dev/ttyUSB0 -c avr911 -U flash:w:$(PRG).hex  -U eeprom:w:$(PRG)_eeprom.hex
#uisp -dprog=bsd -dlpt=/dev/parport1 --upload if=$(PRG).hex
ERASECMD       = 

#uisp -dprog=bsd -dlpt=/dev/parport1 --erase

DEFS           = -DF_CPU=16000000
SERVER_DEFS	   = -DQP_SERVER
SERVER_DEFS	   = -DQP_TERMINAL


LIBS           =

# You should not have to change anything below here.

CC             = avr-gcc

DOT		= dot


# Override is only needed by avr-lib build system.



override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS) -morder1
# -fnew-ra
override LDFLAGS       = -Wl,-Map,$(PRG).map
override ASFLAGS	   = -mmcu=$(MCU_TARGET)


OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(PRG).elf lst text eeprom



$(PRG)_server.elf:

$(PRG)_terminal.elf:

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf *.gif *.png *.ps *.svg
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak 
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

flash:
	$(ERASECMD)
	$(FLASHCMD)

lst:  $(PRG).lst

%.gif: %.dot
	$(DOT) -Tgif -o $@ $<

%.png: %.dot
	$(DOT) -Tpng -o $@ $<

%.svg: %.dot
	$(DOT) -Tsvg -o $@ $<

%.ps: %.dot
	$(DOT) -Tps -o $@ $<


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

