
DEFS = -DF_CPU=16000000l
OBJ = main.o dc_com.o adc.o backlight.o touchscreen.o \
      menu_browser.o main_window.o calibrate_touch.o \
      lap_button.o lap_slider.o \
      can/spi.o can/can.o can_handler.o netvar/netvar.o \
      gui_lib/rectangle.o gui_lib/gui_container.o gui_lib/gui_button.o \
      gui_lib/icons/icons.o gui_lib/gui_slider.o \
      util_lib/list.o


# Default values
OUT           ?= image
MCU_TARGET    ?= atmega128
MCU_CC        ?= avr-gcc
OPTIMIZE      ?= -Os
WARNINGS      ?= -Wall -pedantic
CFLAGS        += -mmcu=$(MCU_TARGET) $(OPTIMIZE) $(WARNINGS) $(DEFS) -std=gnu99
LDFLAGS        = -Wl,-Map,$(OUT).map
CANADDR       ?= XXX

# External Tools
OBJCOPY       ?= avr-objcopy
OBJDUMP       ?= avr-objdump
FLASHCMD      ?= avrdude -c usbasp -p m128 -V -U flash:w:$(OUT).hex
RESETCMD      ?= avrdude -c usbasp -p m128
LAPFLASHCMD   ?= lapcontrol -s rl


LAUNCH_BOOTLOADER = launch-bootloader
SERIAL = /dev/ttyUSB0
BAUDRATE = 115200



#############################################################################
# Rules
all: $(OUT).elf lst text

clean:
	rm -f $(OUT) *.lst *.map *.hex *.bin *.srec
	rm -f $(OBJ)
	rm -rf *.srec $(OUT).elf

flash: $(OUT).hex
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

