
# Default values
AVRXHOME      ?= ../../lib/AvrX2.6f/avrx/
OBJ           ?= $(SRC:.c=.o) $(ASRC:.S=.o)
OUT           ?= image
MCU_TARGET    ?= atmega32    
MCU_CC        ?= avr-gcc
OPTIMIZE      ?= -Os
WARNINGS      ?= -Wall
#DEFS          ?= -DF_CPU=16000000
CSTANDARD     ?= -std=gnu99
CFLAGS        += -mmcu=$(MCU_TARGET) $(OPTIMIZE) $(WARNINGS) $(DEFS) $(CSTANDARD)
#CFLAGS        += -fnew-ra
LDFLAGS        = -Wl,-Map,$(OUT).map
CANADDR       ?= XXX

# External Tools
OBJCOPY       ?= avr-objcopy
OBJDUMP       ?= avr-objdump
FLASHCMD      ?= uisp -dprog=bsd --upload if=$(OUT).hex 
ERASECMD      ?= uisp -dprog=bsd --erase 
LAPFLASHCMD   ?= lapcontrol -s rl

# AvrX defines
LIBS          += $(AVRXHOME)/avrx.a
CFLAGS        += -I$(AVRXHOME)
#CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
#CFLAGS += -funsigned-bitfields -fpack-struct -fshort-enums -fnew-ra
CFLAGS        += -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS        += -Wa,-adhlns=$(<:.c=.lst)




# Stuff
# Place -D or -U options here
CDEFS = -DF_CPU=$(CPUCLK) -DTICKRATE=$(TICKRATE) -DBAUDRATE=$(BAUDRATE)



#############################################################################
# Rules
all: $(OUT).elf lst text eeprom

clean:
	rm -rf $(OUT) *.o *.lst *.map *.hex *.bin *.srec
	rm -rf *.srec $(OUT).elf

flash: $(OUT).hex
	$(ERASECMD)
	$(FLASHCMD)

canflash: $(OUT).hex
	$(LAPFLASHCMD) flash $(CANADDR) $(OUT).hex


#############################################################################
# Building Rules 
$(OUT).elf: $(OBJ)
	$(MCU_CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(MCU_CC) $(CFLAGS) -c $<


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




#######################################################################################



TRUE	= 1
FALSE	= 0




# Minimalistic printf version
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires MATH_LIB = -lm below)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

PRINTF_LIB =

# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point + %[ scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt
SCANF_LIB =

MATH_LIB = -lm


# Linker flags.
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)








# Create final output files (.hex, .eep) from ELF output file.
#%.hex: %.elf
#	@echo
#	@echo $(MSG_FLASH) $@
#	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@
#
#%.eep: %.elf
#	@echo
#	@echo $(MSG_EEPROM) $@
#	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
#	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@
#
## Create extended listing file from ELF output file.
#%.lss: %.elf
#	@echo
#	@echo $(MSG_EXTENDED_LISTING) $@
#	$(OBJDUMP) -h -S $< > $@
#
## Create a symbol table from ELF output file.
#%.sym: %.elf
#	@echo
#	@echo $(MSG_SYMBOL_TABLE) $@
#	$(NM) -n $< > $@



# Link: create ELF output file from object files.
#%.elf: $(OBJ)
#	@echo
#	@echo $(MSG_LINKING) $@
#	$(CC) $(ALL_CFLAGS) $(OBJ) $(GCCLIB) --output $@ $(LDFLAGS)





# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex eep lss sym coff extcoff \
clean clean_list program
