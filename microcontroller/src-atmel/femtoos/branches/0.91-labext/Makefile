FEMTOOS_AVR_TOOLDIR=/home/alex/labor/femtoos-toolchain/
FEMTOOS_HEADER_DIR=./include
FEMTOOS_APPLICATION_CONFIG_DIR=./

TARGET=atmega8

MY_CANADDR=0x25
MY_CANPORT=0x0f

LD=$(FEMTOOS_AVR_TOOLDIR)/bin/avr-ld
GCC=$(FEMTOOS_AVR_TOOLDIR)/bin/avr-gcc
CC=$(FEMTOOS_AVR_TOOLDIR)/bin/avr-gcc
OBJCOPY=$(FEMTOOS_AVR_TOOLDIR)/bin/avr-objcopy

BASEFLAGS= -save-temps
BASEFLAGS+= -mint8
BASEFLAGS+= -Wl,--defsym=__stack=xOS+xOSstackShift
BASEFLAGS+= --param inline-call-cost=2
BASEFLAGS+= -ffunction-sections
BASEFLAGS+= -Wl,--gc-sections
BASEFLAGS+= -Wl,--relax
BASEFLAGS+= -Wall
BASEFLAGS+= -Wno-main
BASEFLAGS+= -Winline
BASEFLAGS+= -Wundef
BASEFLAGS+= -gdwarf-2
BASEFLAGS+= -Os
BASEFLAGS+= -funsigned-char
BASEFLAGS+= -fomit-frame-pointer
BASEFLAGS+= -fpack-struct
BASEFLAGS+= -fshort-enums
BASEFLAGS+= -mmcu=$(TARGET)


CFLAGS=$(BASEFLAGS)
#includes
CFLAGS+= -DdefExtReduceProEpilogue=cfgBikini 
CFLAGS+= -DUSE_FEMTOOS
CFLAGS+= -DF_CPU=16000000
CFLAGS+= -DCANADDR=$(MY_CANADDR)
CFLAGS+= -DCANPORT=$(MY_CANPORT)
CFLAGS+= -I./include -I./src -I./devices
CFLAGS+= -I../include -I../src -I../devices
CFLAGS+= -I$(FEMTOOS_APPLICATION_CONFIG_DIR) -I../$(FEMTOOS_APPLICATION_CONFIG_DIR)

ASMFLAGS=$(CFLAGS)
ASMFLAGS+=-x assembler-with-cpp 
ASMFLAGS+=-I./include -I./src -I./devices
ASMFLAGS+=-I../include -I../src -I../devices
ASMFLAGS+= -I$(FEMTOOS_APPLICATION_CONFIG_DIR) -I../$(FEMTOOS_APPLICATION_CONFIG_DIR)

LNKFLAGS=$(BASEFLAGS)
LNKFLAGS+=-Wl,-Map=main.map

all: main.hex

main.hex: main.elf
	$(OBJCOPY) -O ihex -R .eeprom  main.elf main.hex

main.elf: femtoos spi can mcp2515 treppenblink.o can_thread.o 
	 $(GCC) $(LNKFLAGS) -o main.elf  ./src/femtoos_shared.o ./src/femtoos_core.o ./src/femtoos_port.o ./src/femtoos_startup.o ./src/can.o ./src/spi.o treppenblink.o can_thread.o
#./src/spi.o ./src/xcan.o ./src/mcp2515.o

treppenblink.o: headers
	$(GCC) $(CFLAGS) -c treppenblink.c -o treppenblink.o

can_thread.o: headers
	$(GCC) $(CFLAGS) -c can_thread.c -o can_thread.o

clean:
	rm -f treppenblink.o
	rm -f treppenblink.i
	rm -f treppenblink.s
	rm -f can_thread.o
	rm -f can_thread.i
	rm -f can_thread.s
	rm -f main.elf
	rm -f main.map
	rm -f main.hex
	make TARGET=$(TARGET) -C ./src clean

femtoos:
	make TARGET=$(TARGET) CFLAGS='$(CFLAGS)' ASMFLAGS='$(ASMFLAGS)' -C ./src femtoos

spi:
	make TARGET=$(TARGET) CFLAGS='$(CFLAGS)' ASMFLAGS='$(ASMFLAGS)' -C ./src spi

can:
	make TARGET=$(TARGET) CFLAGS='$(CFLAGS)' ASMFLAGS='$(ASMFLAGS)' -C ./src can

mcp2515:
#	make CANADDR=${CANADDR} TARGET=$(TARGET) CFLAGS='$(CFLAGS)' ASMFLAGS='$(ASMFLAGS)' -C ./src mcp2515

headers: $(FEMTOOS_HEADER_DIR)/femtoos_check.h $(FEMTOOS_HEADER_DIR)/femtoos_code.h $(FEMTOOS_HEADER_DIR)/femtoos_config.h $(FEMTOOS_HEADER_DIR)/femtoos_constants.h $(FEMTOOS_HEADER_DIR)/femtoos_core.h $(FEMTOOS_HEADER_DIR)/femtoos_device.h $(FEMTOOS_HEADER_DIR)/femtoos_globals.h $(FEMTOOS_HEADER_DIR)/femtoos_locals.h $(FEMTOOS_HEADER_DIR)/femtoos_order.h $(FEMTOOS_HEADER_DIR)/femtoos_port.h $(FEMTOOS_HEADER_DIR)/femtoos_shared.h $(FEMTOOS_HEADER_DIR)/femtoos_types.h


$(FEMTOOS_HEADER_DIR)/femtoos_check.h:

$(FEMTOOS_HEADER_DIR)/femtoos_code.h:

$(FEMTOOS_HEADER_DIR)/femtoos_config.h:

$(FEMTOOS_HEADER_DIR)/femtoos_constants.h:

$(FEMTOOS_HEADER_DIR)/femtoos_core.h:

$(FEMTOOS_HEADER_DIR)/femtoos_device.h:

$(FEMTOOS_HEADER_DIR)/femtoos_globals.h:

$(FEMTOOS_HEADER_DIR)/femtoos_locals.h:

$(FEMTOOS_HEADER_DIR)/femtoos_order.h:

$(FEMTOOS_HEADER_DIR)/femtoos_port.h:

$(FEMTOOS_HEADER_DIR)/femtoos_shared.h:

$(FEMTOOS_HEADER_DIR)/femtoos_types.h:

