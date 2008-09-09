include gmsl

PRG	    = anonaccess-test
OBJ	    = keys.o main.o debug_tools.o uart.o debug.o 24CBlockDev.o 24Cxxx.o \
  i2c.o i2c_tools.o comm.o sha256-asm.o hmac-sha256.o reqvalidator.o \
  action.o shabea.o ticketDB.o flmDB.o fairrnd.o rtc.o enc2E24C.o hwrnd.o \
  lop.o lop_debug.o reset_counter.o base64_enc.o selfdestruct.o i2c_printer.o \
  iDP3540.o qport.o xtea-asm.o entropium.o memxor.o system_stats.o hexdigit_tab.o \
  noekeon_asm.o
  
MCU_TARGET     = atmega644
OPTIMIZE       = -Os -mno-tablejump
DEBUG	   = -gstabs
DEFS	   = -D$(call uc,$(MCU_TARGET))
LIBS	   =
DEP_DIR    = .deps/
BIN_DIR    = bin/
DBG_DIR    = dbg/

# You should not have to change anything below here.

CC	     = avr-gcc

# Override is only needed by avr-lib build system.

override LDFLAGS       = -Wl,-Map,$(PRG).map
override CFLAGS        = -MMD -MF$(DEP_DIR)$(patsubst %.c,%.d,$<) -g -pedantic -std=gnu99 -Wall -Wstrict-prototypes $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS) -morder1
# -fnew-ra
override LDFLAGS       = -Wl,-Map,$(PRG).map -mmcu=$(MCU_TARGET) -morder1 -Wall
override ASFLAGS	   = -mmcu=$(MCU_TARGET)

OBJCOPY	= avr-objcopy
OBJDUMP	= avr-objdump


ERASECMD       = 
#FLASHCMD       = avrdude -p $(MCU_TARGET) -c usbasp -U flash:w:$(BIN_DIR)$(PRG).hex  -U eeprom:w:$(BIN_DIR)$(PRG)_eeprom.hex
FLASHCMD       = avrdude -p $(MCU_TARGET)  -P /dev/ttyUSB0 -c avr911 -U flash:w:$(BIN_DIR)$(PRG).hex  -U eeprom:w:$(BIN_DIR)$(PRG)_eeprom.hex
EEPROM_FLASHCMD = avrdude -p $(MCU_TARGET) -P /dev/ttyUSB0 -c avr911 -U eeprom:w:$(BIN_DIR)$(PRG)_eeprom.hex
LOCK_CMD = avrdude -p $(MCU_TARGET) -P /dev/ttyUSB0 -c avr911 -U lock:w:0xfc:m


all: $(BIN_DIR)$(PRG).elf lst text eeprom

listings: $(patsubst %.o,$(DBG_DIR)%.lst, $(OBJ))

$(BIN_DIR)$(PRG).elf: $(patsubst %,$(BIN_DIR)%, $(OBJ))
	@echo "[link] $@"
	@$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

$(BIN_DIR)%.o: %.c
	@echo [gcc] $@
	@$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)%.o: %.S
	@echo [gas] $@
	@$(CC) $(ASFLAGS) -c -o $@ $<

clean:
	rm -rf *.o $(BIN_DIR)$(PRG).elf $(BIN_DIR)* *.eps *.png *.pdf *.bak 
	rm -rf $(DBG_DIR)*.lst *.map $(EXTRA_CLEAN_FILES)

depclean: clean
	rm -rf $(DEP_DIR)*.d

lst:  $(DBG_DIR)$(PRG).lst

flash: $(BIN_DIR)$(PRG).hex $(BIN_DIR)$(PRG)_eeprom.hex
	$(ERASECMD)
	$(FLASHCMD)

eeflash:
	$(EEPROM_FLASHCMD)

lock:
	$(LOCK_CMD)
	
$(DBG_DIR)%.lst: $(BIN_DIR)%.elf
	@echo "[objdump] $@"
	@$(OBJDUMP) -h -S $< > $@

$(DBG_DIR)%.lst: $(BIN_DIR)%.o
	@echo "[objdump] $@"
	@$(OBJDUMP) -h -S $< > $@

# Rules for building the .text rom images

text: hex bin srec

hex:  $(BIN_DIR)$(PRG).hex
bin:  $(BIN_DIR)$(PRG).bin
srec: $(BIN_DIR)$(PRG).srec

%.hex: %.elf
	@echo "[objcopy] $@"
	@$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	@echo "[objcopy] $@"
	@$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	@echo "[objcopy] $@"
	@$(OBJCOPY) -j .text -j .data -O binary $< $@

# Rules for building the .eeprom rom images

eeprom: ehex ebin esrec

ehex:  $(BIN_DIR)$(PRG)_eeprom.hex
ebin:  $(BIN_DIR)$(PRG)_eeprom.bin
esrec: $(BIN_DIR)$(PRG)_eeprom.srec

%_eeprom.hex: %.elf
	@echo "[objcopy] $@"
	@$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

%_eeprom.srec: %.elf
	@echo "[objcopy] $@"
	@$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@

%_eeprom.bin: %.elf
	@echo "[objcopy] $@"
	@$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@

DEPS := $(wildcard $(DEP_DIR)*.d)
ifneq ($(DEPS),)
include $(DEPS)
endif


