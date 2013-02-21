OBJ = main.o


# Default values
OUT           ?= main
MCU_TARGET    ?= attiny24
MCU_CC        ?= avr-gcc
MCU_AS	      ?= avr-as
OPTIMIZE      ?= -Os
WARNINGS      ?= -Wall -Winline
DEFS          ?= -DF_CPU=8000000
CFLAGS        += -mmcu=$(MCU_TARGET) $(OPTIMIZE) $(WARNINGS) $(DEFS) -I.
ASFLAGS	      += -mmcu=avr5
#LDFLAGS        = -Wl,-u,vfprintf,-Map,$(OUT).map
LDFLAGS        = -Wl,-Map,$(OUT).map

LIBS           = -lm -lprintf_flt


# External Tools
OBJCOPY       ?= avr-objcopy
OBJDUMP       ?= avr-objdump
FLASHCMD      ?= avrdude -c usbasp -p $(MCU_TARGET) -U flash:w:image.heh







#############################################################################
# Rules
all: $(OUT).elf lst text eeprom

clean:
	rm -rf $(OUT) *.o *.lst *.map *.hex *.bin *.srec uart/*.o
	rm -rf *.srec $(OUT).elf uart/uart.o


SERIAL_PORT = COM3
BAUDRATE = 128000

upload: $(OUT).hex
	#launch-bootloader $(SERIAL_PORT) $(BAUDRATE)
	avrdude -u -p $(MCU_TARGET) -c avr109 -P $(SERIAL_PORT) -b $(BAUDRATE) -V -U flash:w:$(OUT).hex

flash: $(OUT).hex
	avrdude -u -p $(MCU_TARGET) -c usbasp -V -U flash:w:$(OUT).hex



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

