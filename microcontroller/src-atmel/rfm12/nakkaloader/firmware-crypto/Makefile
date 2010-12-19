# Makefile for foodloader

TARGET = nl_main
OBJECTS = rfm12.o nl_main.o

# microcontroller and project specific settings (everything can be overridden using config.mk)
F_CPU = 8000000UL
MCU = atmega8
#CFLAGS += -DBOOTLOADER_DDR=DDRC
#CFLAGS += -DBOOTLOADER_PORT=PORTC
#CFLAGS += -DBOOTLOADER_PIN=PINC
#CFLAGS += -DBOOTLOADER_PINNUM=PINC0

# include avr-generic makefile configuration
include avr.mk

# bootloader section start
# (see datasheet)
ifeq ($(MCU),atmega8)
	# atmega8 with 1024 words bootloader:
	# bootloader section starts at 0xc00 (word-address) == 0x1800 (byte-address)
#	BOOT_SECTION_START = 0x00
	BOOT_SECTION_START = 0x1800
#	BOOT_SECTION_START = 0x0800
	#BOOT_SECTION_START = 0xc00
	#
	# atmega8 with 512 words bootloader:
	# bootloader section starts at 0xe00 (word-address) == 0x1c00 (byte-address)
	# BOOT_SECTION_START = 0x1c00
	#Fuses for Xtal/16MHz, bootloader at 0x1c00
	HFUSE = 0xc8
	LFUSE = 0x9f
else ifeq ($(MCU),atmega88)
	# atmega88 with 1024 words bootloader:
	# bootloader section starts at 0xc00 (word-address) == 0x1800 (byte-address)
	#BOOT_SECTION_START = 0x1800
	#
	# atmega88 with 512 words bootloader:
	# bootloader section starts at 0xe00 (word-address) == 0x1c00 (byte-address)
	BOOT_SECTION_START = 0x1c00
else ifeq ($(MCU),atmega168)
	# atmega168 with 1024 words bootloader:
	# bootloader section starts at 0x1c00 (word-address) == 0x3800 (byte-address)
	BOOT_SECTION_START = 0x3800
	#
	# atmega168 with 512 words bootloader:
	# bootloader section starts at 0x1e00 (word-address) == 0x3c00 (byte-address)
	#BOOT_SECTION_START = 0x3c00
else ifeq ($(MCU),atmega32)
	# atmega32 with 2048 words bootloader:
	# bootloader section starts at 0x3800 (word-address) == 0x7000 (byte-address)
	BOOT_SECTION_START = 0x7000
	#BOOTSZ1, BOOTSZ0 -> 0 -> SET, BOOTRST -> 0 -> SET
	HFUSE = 0xc8
	LFUSE = 0x9f
	#
	# atmega32 with 1024 words bootloader:
	# bootloader section starts at 0x3c00 (word-address) == 0x7800 (byte-address)
	#BOOT_SECTION_START = 0x7800
	#
	# atmega32 with 512 words bootloader:
	# bootloader section starts at 0x3e00 (word-address) == 0x7c00 (byte-address)
	#BOOT_SECTION_START = 0x7c00	
	#
	#Fuses for Xtal/16MHz, bootloader at 0x1c00
else ifeq ($(MCU),atmega644)
	# atmega644 with 1024 words bootloader:
	# bootloader section starts at 0x7c00 (word-address) == 0xf800 (byte-address)
	BOOT_SECTION_START = 0xf800
	#
	# atmega644 with 512 words bootloader:
	# bootloader section starts at 0x7e00 (word-address) == 0xfc00 (byte-address)
	#BOOT_SECTION_START = 0xfc00
else ifeq ($(MCU),atmega162)
	# atmega162 with 1024 words bootloader:
	# bootloader section starts at 0x1c00 (word-address) == 0x3800 (byte-address)
	#BOOT_SECTION_START = 0x3800
	#
	# atmega168 with 512 words bootloader:
	# bootloader section starts at 0x1e00 (word-address) == 0x3c00 (byte-address)
	BOOT_SECTION_START = 0x3c00
	#Fuses for Xtal/16MHz, bootloader at 0x3c00
	HFUSE = 0xd2
	LFUSE = 0xef
else ifeq ($(MCU),atmega128)
	# atmega128 with 1024 words bootloader:
	# bootloader section starts at 0xfc00 (word-address) == 0x1f800 (byte-address)
	BOOT_SECTION_START = 0x1f800
	#
	# atmega168 with 512 words bootloader:
	# bootloader section starts at 0xfe00 (word-address) == 0x1fC00 (byte-address)
	#BOOT_SECTION_START = 0x1fc00
	#Fuses for Xtal/16MHz, bootloader at 0x1f800
	HFUSE = 0xc4
	LFUSE = 0xef
endif

LDFLAGS += -Wl,-Map=nakkaflash.map,--section-start=.text=$(BOOT_SECTION_START)
CFLAGS += -DBOOT_SECTION_START=$(BOOT_SECTION_START)

# won't do on win32
# OBJECTS += $(patsubst %.c,%.o,$(shell echo *.c))
HEADERS += $(shell echo *.h)
CFLAGS += -Werror -g -Os
LDFLAGS += -L/usr/local/avr/avr/lib


.PHONY: all 

all: $(TARGET).hex $(TARGET).lss
	@echo "==============================="
	@echo "compiled for: $(MCU)"
	@echo -n "bootloader size is: "
	@$(SIZE) -A $< | grep "\.sec1" | tr -s " " | cut -d" " -f2
	@echo "==============================="

$(TARGET): $(OBJECTS) # $(TARGET).o

%.o: $(HEADERS)

.PHONY: install

install: program-isp-$(TARGET)
	@echo "don't forget: the lock-bits have been cleaned by the chip-erase!"

.PHONY: clean clean-$(TARGET)

clean: clean-$(TARGET) clean-uploadtest

clean-$(TARGET):
	rm -f *.o *.map $(TARGET)

clean-uploadtest:
	rm -f datatestfile.raw

datatestfile.raw:
	dd if=/dev/urandom of=datatestfile.raw bs=1 count=$$((1024*14))

uploadtest: datatestfile.raw
	@echo "uploading datatestfile"
	$(AVRDUDE) $(AVRDUDE_FLAGS) -c $(SERIAL_PROG) -s -u -P $(SERIAL_DEV) -U f:w:$<:r

#lock:
#	$(AVRDUDE) $(AVRDUDE_FLAGS) -c $(ISP_PROG) -P $(ISP_DEV) -U lock:w:0x2f:m
	
fuse:
	$(AVRDUDE) $(AVRDUDE_FLAGS) -c $(ISP_PROG) -U hfuse:w:$(HFUSE):m
	$(AVRDUDE) $(AVRDUDE_FLAGS) -c $(ISP_PROG) -U lfuse:w:$(LFUSE):m

