
OBJ = main.o uart.o

MCU_TARGET = atmega644

# Default values
OUT           ?= image
MCU_CC        ?= avr-gcc
OPTIMIZE      ?= -Os
WARNINGS      ?= -Wall
DEFS          ?= -DF_CPU=20000000
CFLAGS        += -MMD -g -mmcu=$(MCU_TARGET) $(OPTIMIZE) $(WARNINGS) $(DEFS)
ASFLAGS       ?=  
ASFLAGS       += -g $(DEFS) 
LDFLAGS        = -Wl,-Map,$(OUT).map
CANADDR       ?= XXX

# External Tools
OBJCOPY       ?= avr-objcopy
OBJDUMP       ?= avr-objdump

# Programming support using avrdude. Settings and variables.

AVRDUDE = avrdude
#AVRDUDE_PROGRAMMER = stk200
AVRDUDE_PROGRAMMER = avr109

AVRDUDE_PORT = /dev/ttyUSB0	-b 115200

AVRDUDE_WRITE_FLASH = -U flash:w:$(OUT).hex
#AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep

AVRDUDE_FLAGS = -p $(MCU_TARGET) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)

# Uncomment the following if you want avrdude's erase cycle counter.
# Note that this counter needs to be initialized first using -Yn,
# see avrdude manual.
#AVRDUDE_ERASE += -y

# Uncomment the following if you do /not/ wish a verification to be
# performed after programming the device.
AVRDUDE_FLAGS += -V




#############################################################################
# Rules
all: $(OUT).elf lst text eeprom

clean:
	rm -rf $(OUT) *.o *.d *.lst *.map *.hex *.bin *.srec
	rm -rf *.srec $(OUT).elf

# Program the device.  
program: $(OUT).hex
	launch-bootloader /dev/ttyUSB0 115200
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)


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
