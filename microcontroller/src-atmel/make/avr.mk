
# Default values
OUT           ?= image
MCU_TARGET    ?= atmega32
MCU_CC        ?= avr-gcc
OPTIMIZE      ?= -Os
WARNINGS      ?= -Wall
DEFS          ?= -DF_CPU=16000000
CFLAGS        += -MMD -g -mmcu=$(MCU_TARGET) $(OPTIMIZE) $(WARNINGS) $(DEFS)
ASFLAGS       ?=  
ASFLAGS       += -g $(DEFS) 
LDFLAGS        = -Wl,-Map,$(OUT).map
CANADDR       ?= XXX

# External Tools
OBJCOPY       ?= avr-objcopy
OBJDUMP       ?= avr-objdump
FLASHCMD      ?= uisp -dprog=bsd --upload if=$(OUT).hex 
ERASECMD      ?= uisp -dprog=bsd --erase 
FLASHUSBCMD   ?= avrdude -c avr910 -p m32 -P $(AVRPROGDEV) -e -U flash:w:image.hex
LAPFLASHCMD   ?= lapcontrol -s rl

#############################################################################
# Rules
all: $(OUT).elf lst text eeprom

clean:
	rm -rf $(OUT) *.o *.d *.lst *.map $(OUT).hex $(OUT)_eeprom.hex *.bin *.srec
	rm -rf *.srec $(OUT).elf

flash: $(OUT).hex
	$(ERASECMD)
	$(FLASHCMD)

flashusb: $(OUT).hex
	$(FLASHUSBCMD)

canflash: $(OUT).hex
	$(LAPFLASHCMD) flash $(CANADDR) $(OUT).hex

#############################################################################
# Building Rules 
$(OUT).elf: $(OBJ)
	$(MCU_CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(MCU_CC) $(CFLAGS) -c $<

%.o: %.S
	$(MCU_CC) -mmcu=$(MCU_TARGET) $(ASFLAGS) -c $<

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

DEPS := $(wildcard *.d)
ifneq ($(DEPS),)
include $(DEPS)
endif
