OBJ = main.o uart_handler.o io.o uart/uart.o

# Default values
OUT           ?= image
MCU_TARGET    ?= atmega8
MCU_CC        ?= avr-gcc
MCU_AS	      ?= avr-as
OPTIMIZE      ?= -Os
WARNINGS      ?= -Wall -Winline -Wextra
DEFS          ?= -DF_CPU=8000000
CFLAGS        += -mmcu=$(MCU_TARGET) $(OPTIMIZE) $(WARNINGS) $(DEFS) -I. -std=gnu99 -ffunction-sections -fdata-sections -mstrict-X -maccumulate-args
ASFLAGS	      += -mmcu=avr5
LDFLAGS        = -Wl,-Map,$(OUT).map,--gc-sections,--relax,--print-gc-sections

# External Tools
OBJCOPY       ?= avr-objcopy
OBJDUMP       ?= avr-objdump
FLASHCMD      ?= avrdude -p $(MCU_TARGET) -U $(OUT).hex

#############################################################################
# Rules
all: $(OUT).elf lst text eeprom

clean:
	rm -rf $(OUT) *.o *.lst *.map *.hex *.bin *.srec uart/*.o
	rm -rf *.srec $(OUT).elf

flash: $(OUT).hex
	$(FLASHCMD)

canflash: $(OUT).hex
	$(CANFLASHCMD)

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

