
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
AVRPROGDEV    ?= com7
FLASHUSBCMD   ?= avrdude -c stk500v2 -p $(MCU_TARGET) -P $(AVRPROGDEV) -e -U flash:w:image.hex
LAPFLASHCMD   ?= lapcontrol -s rl
AVRDUDE       ?= avrdude

#---------------- Programming Options (avrdude) ----------------

# Programming hardware: alf avr910 avrisp bascom bsd 
# dt006 pavr picoweb pony-stk200 sp12 stk200 stk500
#
# Type: avrdude -c ?
# to get a full listing.
#

# Fast: Can program Flash and EEPROM, but NOT FUSES 
# Normal: Should program Flash, EEPROM and FUSES, but slow and Flash isn't working.
AVRDUDE_PROGRAMMER = avr910
AVRDUDE_PROGRAMMER_FAST = butterfly

AVRDUDE_WRITE_FLASH = -U flash:w:$(OUT).hex
AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(OUT)_eeprom.hex
#AVRDUDE_WRITE_FUSES = -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m
AVRDUDE_WRITE_FUSES = -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m

# Uncomment the following if you want avrdude's erase cycle counter.
# Note that this counter needs to be initialized first using -Yn,
# see avrdude manual.
#AVRDUDE_ERASE_COUNTER = -y

# Uncomment the following if you do /not/ wish a verification to be
# performed after programming the device.
#AVRDUDE_NO_VERIFY = -V

# Increase verbosity level.  Please use this when submitting bug
# reports about avrdude. See <http://savannah.nongnu.org/projects/avrdude> 
# to submit bug reports.
#AVRDUDE_VERBOSE = -v -v

AVRDUDE_FLAGS = -p $(MCU_TARGET) -c $(AVRDUDE_PROGRAMMER) -P $(AVRPROGDEV)
AVRDUDE_FLAGS += $(AVRDUDE_NO_VERIFY)
AVRDUDE_FLAGS += $(AVRDUDE_VERBOSE)
AVRDUDE_FLAGS += $(AVRDUDE_ERASE_COUNTER)
AVRDUDE_FLAGS_FAST = -p $(MCU_TARGET) -c $(AVRDUDE_PROGRAMMER_FAST) -P $(AVRPROGDEV)
AVRDUDE_FLAGS_FAST += $(AVRDUDE_NO_VERIFY)
AVRDUDE_FLAGS_FAST += $(AVRDUDE_VERBOSE)
AVRDUDE_FLAGS_FAST += $(AVRDUDE_ERASE_COUNTER)

#############################################################################
# Rules
all: $(OUT).elf lst text eeprom

clean:
	rm -rf $(OUT) *.o *.d *.lst *.map *.hex *.bin *.srec
	rm -rf *.srec $(OUT).elf

flash: $(OUT).hex
	$(ERASECMD)
	$(FLASHCMD)

flashusb: $(OUT).hex
	$(FLASHUSBCMD)

canflash: $(OUT).hex
	$(LAPFLASHCMD) flash $(CANADDR) $(OUT).hex
	
# Program the flash of the device only
program: $(OUT).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS_FAST) $(AVRDUDE_WRITE_FLASH)

# Program fuses
progfuses: $(OUT).hex 
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FUSES)

# Program eeprom
progeeprom: $(OUT).hex 
	$(AVRDUDE) $(AVRDUDE_FLAGS_FAST) $(AVRDUDE_WRITE_EEPROM)	

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
