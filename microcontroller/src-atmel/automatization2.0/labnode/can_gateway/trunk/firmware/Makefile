
OBJ            = main.o uart/uart.o canlib/can.o canlib/spi.o usbdrv/usbdrv.o usbdrv/usbdrvasm.o usbdrv/oddebug.o

# Default values
OUT           ?= image
MCU_TARGET    ?= atmega32
MCU_CC        ?= avr-gcc
MCU_AS        ?= avr-as
OPTIMIZE      ?= -Os
WARNINGS      ?= -Wall -Winline -Wextra -Wpedantic
SVN_REVISION  ?= `svn --trust-server-cert --non-interactive info | grep "Revision:" | cut -d : -f 2 | tr -d '\n '`
ID_STRING     ?=  \"`pwd | awk -F / '{print $$NF}' | cut -c 1-20 | tr -d '\n '`\"
DEFS          ?= -DF_CPU=16000000UL -DFW_SVNREVISION=$(SVN_REVISION) -DFW_IDSTRING=$(ID_STRING)
CFLAGS        += -mmcu=$(MCU_TARGET) $(OPTIMIZE) $(WARNINGS) $(DEFS) -I. -std=gnu99 -ffunction-sections -fdata-sections -mstrict-X -maccumulate-args
ASFLAGS       += -mmcu=avr5
LDFLAGS        = -Wl,-Map,$(OUT).map,--relax,--gc-sections,--print-gc-sections

# External Tools
OBJCOPY       ?= avr-objcopy
OBJDUMP       ?= avr-objdump
FLASHCMD      ?= avrdude -p $(MCU_TARGET) -U $(OUT).hex

#############################################################################
# Rules
all: $(OUT).elf lst text eeprom

clean:
	rm -rf $(OUT) *.o *.lst *.map *.hex *.bin *.srec canlib/*.o uart/*.o usbdrv/*.o
	rm -rf *.srec $(OUT).elf

flash: $(OUT).hex
	$(FLASHCMD)

#############################################################################
# Building Rules
$(OUT).elf: $(OBJ)
	$(MCU_CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(MCU_CC) $(CFLAGS) -c $< -o $@

#%.o: %.S
#	$(MCU_AS) $(ASFLAGS) -o $@ $<
#

# Generic rule for assembling Assembler source files:
%.o: %.S
	$(MCU_CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.



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

