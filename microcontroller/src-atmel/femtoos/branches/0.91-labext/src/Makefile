FEMTOOS_AVR_TOOLDIR=/home/alex/labor/femtoos-toolchain/
FEMTOOS_HEADER_DIR=../include

LNKFLAGS=$(BASEFLAGS)
LNKFLAGS+=-Wl,-Map=main.map

FEMTOOS_C_SRC=femtoos_app.c femtoos_core.c femtoos_port.c femtoos_shared.c
FEMTOOS_ASM_SRC=femtoos_startup.s

all: femtoos_core.o femtoos_port.o femtoos_shared.o femtoos_startup.o

femtoos_core.o: headers
	$(FEMTOOS_AVR_TOOLDIR)/bin/avr-gcc $(CFLAGS) -c femtoos_core.c -o femtoos_core.o
femtoos_port.o: headers
	$(FEMTOOS_AVR_TOOLDIR)/bin/avr-gcc $(CFLAGS) -c femtoos_port.c -o femtoos_port.o
femtoos_shared.o: headers
	$(FEMTOOS_AVR_TOOLDIR)/bin/avr-gcc $(CFLAGS) -c femtoos_shared.c -o femtoos_shared.o
femtoos_startup.o: headers
	$(FEMTOOS_AVR_TOOLDIR)/bin/avr-gcc $(ASMFLAGS) -c femtoos_startup.s -o femtoos_startup.o

clean:
	rm -f femtoos_startup.o
	rm -f femtoos_startup.i
	rm -f femtoos_shared.o
	rm -f femtoos_shared.s
	rm -f femtoos_shared.i
	rm -f femtoos_port.o
	rm -f femtoos_port.i
	rm -f femtoos_port.s
	rm -f femtoos_core.o
	rm -f femtoos_core.i
	rm -f femtoos_core.s
	rm -f femtoos_app.o
	rm -f femtoos_app.i
	rm -f femtoos_app.s


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

