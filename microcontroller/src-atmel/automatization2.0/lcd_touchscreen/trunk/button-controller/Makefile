DEFS = -DF_CPU=8000000l
OBJ = main.o uart/uart.o


# Default values
OUT           ?= image
MCU_TARGET    ?= attiny2313
MCU_CC        ?= avr-gcc
OPTIMIZE      ?= -Os
WARNINGS      ?= -Wall
CFLAGS        += -mmcu=$(MCU_TARGET) $(OPTIMIZE) $(WARNINGS) $(DEFS)
LDFLAGS        = -Wl,-Map,$(OUT).map
CANADDR       ?= XXX

# External Tools
OBJCOPY       ?= avr-objcopy
OBJDUMP       ?= avr-objdump
FLASHCMD      ?= avrdude -c usbasp -p $(MCU_TARGET) -U flash:w:$(OUT).hex
FUSECMD        = avrdude -c usbasp -p $(MCU_TARGET) -B 100 -U lfuse:w:0xe4:m -U hfuse:w:0x99:m -U efuse:w:0xff:m

LAPFLASHCMD   ?= lapcontrol -s rl


LAUNCH_BOOTLOADER = launch-bootloader
SERIAL = /dev/ttyUSB0
BAUDRATE = 115200



#############################################################################
# Rules
all: $(OUT).elf lst text

clean:
	rm -rf $(OUT) *.o *.lst *.map *.hex *.bin *.srec uart/*.o
	rm -rf *.srec $(OUT).elf

flash: $(OUT).hex
	$(ERASECMD)
	$(FLASHCMD)

fuse:
	$(FUSECMD)

reset:
	$(RESETCMD)

canflash: $(OUT).hex
	$(LAPFLASHCMD) flash $(CANADDR) $(OUT).hex


sflash: $(OUT).hex
	$(LAUNCH_BOOTLOADER) $(SERIAL) $(BAUDRATE)
	avrdude -p m16 -b $(BAUDRATE) -u -c avr109 -P $(SERIAL) -U f:w:$(OUT).hex -F
	echo X > $(SERIAL)



#############################################################################
# Building Rules 
$(OUT).elf: $(OBJ)
	$(MCU_CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(MCU_CC) $(CFLAGS) -c $< -o $@


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
	
%.o: %.S
	avr-gcc -c $(CFLAGS) -o $@ $< 


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

