FEMTOOS_HEADER_DIR=$(FEMTOOS_SOURCE)/../include

LD=$(FEMTOOS_AVR_TOOLDIR)/bin/avr-ld
GCC=$(FEMTOOS_AVR_TOOLDIR)/bin/avr-gcc
CC=$(FEMTOOS_AVR_TOOLDIR)/bin/avr-gcc
AR=$(FEMTOOS_AVR_TOOLDIR)/bin/avr-ar

CFLAGS+=-I$(FEMTOOS_HEADER_DIR)

all: can_thread twi_mthread
	$(AR) -crs $(FEMTOOS_LIB_DIR)/application.a can_thread.o twi_mthread.o

can_thread: headers 
	$(GCC) $(CFLAGS) -c can_thread.c -o can_thread.o

twi_mthread: headers 
	$(GCC) $(CFLAGS) -c twi_mthread.c -o twi_mthread.o

clean:
	rm -f can_thread.o
	rm -f can_thread.i
	rm -f can_thread.s
	rm -f twi_mthread.o
	rm -f twi_mthread.i
	rm -f twi_mthread.s
	rm -f $(FEMTOOS_LIB_DIR)/application.a


headers: $(FEMTOOS_HEADER_DIR)/femtoos_check.h $(FEMTOOS_HEADER_DIR)/femtoos_code.h $(FEMTOOS_HEADER_DIR)/femtoos_config.h $(FEMTOOS_HEADER_DIR)/femtoos_constants.h $(FEMTOOS_HEADER_DIR)/femtoos_core.h $(FEMTOOS_HEADER_DIR)/femtoos_device.h $(FEMTOOS_HEADER_DIR)/femtoos_globals.h $(FEMTOOS_HEADER_DIR)/femtoos_locals.h $(FEMTOOS_HEADER_DIR)/femtoos_order.h $(FEMTOOS_HEADER_DIR)/femtoos_port.h $(FEMTOOS_HEADER_DIR)/femtoos_shared.h $(FEMTOOS_HEADER_DIR)/femtoos_types.h $(FEMTOOS_HEADER_DIR)/twi_master.h 

$(FEMTOOS_HEADER_DIR)/can.h: $(FEMTOOS_HEADER_DIR)/spi.h $(FEMTOOS_HEADER_DIR)/mcp2510.h

$(FEMTOOS_HEADER_DIR)/spi.h:

$(FEMTOOS_HEADER_DIR)/twi_master.h:

$(FEMTOOS_HEADER_DIR)/mcp2510.h: 

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

