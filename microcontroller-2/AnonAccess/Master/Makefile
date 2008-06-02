include gmsl

PRG	    = anonaccess-test
OBJ	    = keys.o main.o debug_tools.o uart.o debug.o 24CBlockDev.o 24Cxxx.o \
  i2c.o i2c_tools.o comm.o sha256-asm.o hmac-sha256.o reqvalidator.o \
  action.o shabea.o ticketDB.o flmDB.o fairrnd.o rtc.o enc2E24C.o hwrnd.o \
  lop.o lop_debug.o reset_counter.o base64_enc.o selfdestruct.o i2c_printer.o \
  iDP3540.o qport.o xtea-asm.o entropium.o
MCU_TARGET     = atmega644
OPTIMIZE       = -Os -mno-tablejump
DEBUG	   = -gstabs
DEFS	   = -D$(call uc,$(MCU_TARGET))
LIBS	   =

# You should not have to change anything below here.

CC	     = avr-gcc

# Override is only needed by avr-lib build system.

override LDFLAGS       = -Wl,-Map,$(PRG).map
override CFLAGS        = -MMD -g -pedantic -std=gnu99 -Wall -Wstrict-prototypes $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS) -morder1
# -fnew-ra
override LDFLAGS       = -Wl,-Map,$(PRG).map
override ASFLAGS	   = -mmcu=$(MCU_TARGET)

OBJCOPY	= avr-objcopy
OBJDUMP	= avr-objdump


ERASECMD       = 
#FLASHCMD       = avrdude -p $(MCU_TARGET) -c usbasp -U flash:w:$(PRG).hex  -U eeprom:w:$(PRG)_eeprom.hex
FLASHCMD       = avrdude -p $(MCU_TARGET)  -P /dev/ttyUSB0 -c avr911 -U flash:w:$(PRG).hex  -U eeprom:w:$(PRG)_eeprom.hex
EEPROM_FLASHCMD = avrdude -p $(MCU_TARGET) -P /dev/ttyUSB0 -c avr911 -U eeprom:w:$(PRG)_eeprom.hex
LOCK_CMD = avrdude -p $(MCU_TARGET) -P /dev/ttyUSB0 -c avr911 -U lock:w:0xfc:m


all: $(PRG).elf lst text eeprom

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

host-test:
	gcc -o sha256_host -g -D__LITTLE_ENDIAN__ -DHOST_TEST main.c sha256.c

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak 
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

lst:  $(PRG).lst

flash: $(PRG).hex $(PRG)_eeprom.hex
	$(ERASECMD)
	$(FLASHCMD)

eeflash:
	$(EEPROM_FLASHCMD)

lock:
	$(LOCK_CMD)
	
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

# Every thing below here is used by avr-libc's build system and can be ignored
# by the casual user.

FIG2DEV		 = fig2dev
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

DEPS := $(wildcard *.d)
ifneq ($(DEPS),)
include $(DEPS)
endif


