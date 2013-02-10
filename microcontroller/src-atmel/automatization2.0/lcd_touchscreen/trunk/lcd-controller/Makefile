DEFS = -DF_CPU=16000000l
OBJ = main.o laborlogo.o gui_lib/graphics.o fonts/font_uni53.o lcd_hardware.o com.o draw_pixmap.o hypnotoad.c hypnotoad_data.c


# Default values
OUT           ?= image
MCU_TARGET    ?= atmega162
MCU_CC        ?= avr-gcc
OPTIMIZE      ?= -Os
WARNINGS      ?= -Wall
CFLAGS        += -mmcu=$(MCU_TARGET) $(OPTIMIZE) $(WARNINGS) $(DEFS) -std=gnu99 -ffunction-sections -fdata-sections
LDFLAGS        = -Wl,-Map,$(OUT).map,--gc-sections,--relax,--print-gc-sections
CANADDR       ?= XXX

# External Tools
OBJCOPY       ?= avr-objcopy
OBJDUMP       ?= avr-objdump
FLASHCMD      ?= avrdude -c usbasp -p m162 -V -U flash:w:$(OUT).hex
RESETCMD      ?= avrdude -c usbasp -p m128

LAPFLASHCMD   ?= lapcontrol -s rl


LAUNCH_BOOTLOADER = launch-bootloader
SERIAL = /dev/ttyUSB0
BAUDRATE = 115200



#############################################################################
# Rules
all: $(OUT).elf lst text

clean:
	rm -rf $(OUT) *.o *.lst *.map *.hex *.bin *.srec
	rm -rf *.srec $(OUT).elf

flash: $(OUT).hex
	$(ERASECMD)
	$(FLASHCMD)

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

