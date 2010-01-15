FEMTOOS_HEADER_DIR:=$(FEMTOOS_SOURCE)/../include

LD=$(FEMTOOS_AVR_TOOLDIR)/bin/avr-ld
GCC=$(FEMTOOS_AVR_TOOLDIR)/bin/avr-gcc
CC=$(FEMTOOS_AVR_TOOLDIR)/bin/avr-gcc
AR=$(FEMTOOS_AVR_TOOLDIR)/bin/avr-ar

FEMTOOS_C_SRC=femtoos_app.c femtoos_core.c femtoos_port.c femtoos_shared.c
FEMTOOS_ASM_SRC=femtoos_startup.s

all: spi mcp2515 femtoos can twi_master

twi_master: twi_master.o
	@echo $(FEMTOOS_LIB_DIR)
	$(AR) -crs $(FEMTOOS_LIB_DIR)/twi_master.a twi_master.o
	@echo ""
	@echo "########### twi_master done"
	@echo ""


spi: spi.o
	@echo $(FEMTOOS_LIB_DIR)
	$(AR) -crs $(FEMTOOS_LIB_DIR)/spi.a spi.o
	@echo ""
	@echo "########### spi done"
	@echo ""

femtoos: femtoos_core.o femtoos_port.o femtoos_shared.o femtoos_startup.o
	$(AR) -crs $(FEMTOOS_LIB_DIR)/femtoos.a femtoos_core.o femtoos_port.o femtoos_shared.o 
	cp femtoos_startup.o $(FEMTOOS_LIB_DIR)/
	@echo ""
	@echo "########### femtoos done"
	@echo ""

can: can.o
	$(AR) -crs $(FEMTOOS_LIB_DIR)/can_base.a can.o
	@echo ""
	@echo "########### can done"
	@echo ""

#mcp2515: mcp2515.o
#	@echo ""
#	@echo "########### mcp2515 done"
#	@echo ""

femtoos_core.o: headers
	$(GCC) $(CFLAGS) -c femtoos_core.c -o femtoos_core.o

femtoos_port.o: headers
	$(GCC) $(CFLAGS) -c femtoos_port.c -o femtoos_port.o

femtoos_shared.o: headers
	$(GCC) $(CFLAGS) -c femtoos_shared.c -o femtoos_shared.o

can.o: headers $(FEMTOOS_HEADER_DIR)/can.h
	$(GCC) $(CFLAGS) -c can.c -o can.o

spi.o: headers $(FEMTOOS_HEADER_DIR)/spi.h
	$(GCC) $(CFLAGS) -c spi.c -o spi.o

twi_master.o: headers $(FEMTOOS_HEADER_DIR)/twi_master.h
	$(GCC) $(CFLAGS) -c twi_master.c -o twi_master.o

#mcp2515.o: headers $(FEMTOOS_HEADER_DIR)/mcp2515.h
#	$(GCC) -DCANADDR=${CANADDR} $(CFLAGS) -c mcp2515.c -o mcp2515.o

femtoos_startup.o: headers
	$(GCC) $(ASMFLAGS) -c femtoos_startup.s -o femtoos_startup.o

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
	rm -f spi.o
	rm -f spi.i
	rm -f spi.s
	rm -f twi_master.o
	rm -f twi_master.i
	rm -f twi_master.s
	rm -f can.o
	rm -f can.i
	rm -f can.s
	rm -f mcp2515.o
	rm -f mcp2515.i
	rm -f mcp2515.s
	rm -f $(FEMTOOS_LIB_DIR)/spi.a
	rm -f $(FEMTOOS_LIB_DIR)/can_base.a
	rm -f $(FEMTOOS_LIB_DIR)/twi_master.a
	rm -f $(FEMTOOS_LIB_DIR)/femtoos.a



headers: $(FEMTOOS_HEADER_DIR)/femtoos_check.h $(FEMTOOS_HEADER_DIR)/femtoos_code.h $(FEMTOOS_HEADER_DIR)/femtoos_config.h $(FEMTOOS_HEADER_DIR)/femtoos_constants.h $(FEMTOOS_HEADER_DIR)/femtoos_core.h $(FEMTOOS_HEADER_DIR)/femtoos_device.h $(FEMTOOS_HEADER_DIR)/femtoos_globals.h $(FEMTOOS_HEADER_DIR)/femtoos_locals.h $(FEMTOOS_HEADER_DIR)/femtoos_order.h $(FEMTOOS_HEADER_DIR)/femtoos_port.h $(FEMTOOS_HEADER_DIR)/femtoos_shared.h $(FEMTOOS_HEADER_DIR)/femtoos_types.h

$(FEMTOOS_HEADER_DIR)/can.h: $(FEMTOOS_HEADER_DIR)/spi.h $(FEMTOOS_HEADER_DIR)/mcp2510.h

$(FEMTOOS_HEADER_DIR)/spi.h:

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

