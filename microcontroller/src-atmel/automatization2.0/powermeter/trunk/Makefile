OBJ = main.o adc_driver.o can/can.o can/spi.o can_handler.o netvar/netvar.o util_lib/list.o

# Default values
OUT           ?= main
MCU_TARGET    ?= atxmega192a3
MCU_CC        ?= avr-gcc
MCU_AS	      ?= avr-as
OPTIMIZE      ?= -Os
WARNINGS      ?= -Wall -Winline
DEFS          ?= -DF_CPU=32000000
CFLAGS        += -mmcu=$(MCU_TARGET) $(OPTIMIZE) $(WARNINGS) $(DEFS) -I. -std=c99
ASFLAGS	      += -mmcu=avr5
LDFLAGS        = -Wl,-Map,$(OUT).map

# External Tools
OBJCOPY       ?= avr-objcopy
OBJDUMP       ?= avr-objdump
FLASHCMD      ?= avrdude -c usbasp -p $(MCU_TARGET) -U flash:w:image.hex
ERASECMD      ?= avrdude -c usbasp -p $(MCU_TARGET) -e

#############################################################################
# Rules
all: $(OUT).elf lst text eeprom

clean:
	rm -rf $(OUT) *.o *.lst *.map *.hex *.bin *.srec can/*.o
	rm -rf *.srec $(OUT).elf

flash: $(OUT).hex
	$(ERASECMD)
	$(FLASHCMD)

#############################################################################
# Building Rules 
$(OUT).elf: $(OBJ)
	$(MCU_CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(MCU_CC) $(CFLAGS) -c $< -o $@ 

%.o: %.S
	$(MCU_AS) $(ASFLAGS) -o $@ $<

lst: $(OUT).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

# Rules for building the .text rom images
text: hex bin srec

hex:  $(OUT).hex
bin:  $(OUT).bin
srec: $(OUT).srec

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

# Rules for building the .eeprom rom images

eeprom: ehex ebin esrec

ehex:  $(OUT)_eeprom.hex
ebin:  $(OUT)_eeprom.bin
esrec: $(OUT)_eeprom.srec

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

%_eeprom.srec: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@

%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@

