LAP_ADDR       = 0x25

OBJ            = main.o can/spi.o can/can.o twi_master/twi_master.o can_handler.o netvar/netvar.o netvar/netvar_io.o util_lib/list.o io.o ds1631.o led_driver.o animationen.o temp_read.o

# Default values
OUT           ?= image
MCU_TARGET    ?= atmega8
MCU_CC        ?= avr-gcc
MCU_AS	      ?= avr-as
OPTIMIZE      ?= -Os
WARNINGS      ?= -Wall -Winline -pedantic -Wstrict-prototypes -Wextra
DEFS          ?= -DF_CPU=16000000UL -DEEPROM_LAP_ADDR=$(LAP_ADDR)
CFLAGS        += -mmcu=$(MCU_TARGET) $(OPTIMIZE) $(WARNINGS) $(DEFS) -I. -std=gnu99 -ffunction-sections -fdata-sections -mstrict-X -maccumulate-args
ASFLAGS	      += -mmcu=avr5
LDFLAGS        = -Wl,-Map,$(OUT).map,--relax,--gc-sections,--print-gc-sections

# External Tools
OBJCOPY       ?= avr-objcopy
OBJDUMP       ?= avr-objdump
FLASHCMD      ?= avrdude -p $(MCU_TARGET) -U $(OUT).hex
CANFLASHCMD    = lapcontrol -s wl flash $(LAP_ADDR) $(OUT).hex

#############################################################################
# Rules
all: $(OUT).elf lst text eeprom

clean:
	rm -rf $(OUT) *.o *.lst *.map *.hex *.bin *.srec can/*.o netvar/*.o util_lib/*.o twi_master/*.o
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

