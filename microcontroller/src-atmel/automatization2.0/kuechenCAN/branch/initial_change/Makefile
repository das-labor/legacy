FEMTOOS_AVR_TOOLDIR=/home/alex/labor/femtoos-toolchain/

FEMTOOS_SOURCE=./
FEMTOOS_APPLICATION_CONFIG_DIR=./
TARGET=atmega8

################ can address and port
MY_CANADDR=0x23
MY_CANPORT=0x0f

all: main.hex

include Makefile.femtoos

main.elf: femtoos spi can_base application kuechenCAN.o 
	 $(GCC) $(LNKFLAGS) -o main.elf  \
	kuechenCAN.o \
	$(FEMTOOS_LIB_DIR)/femtoos_startup.o $(FEMTOOS_LIB_DIR)/femtoos.a $(FEMTOOS_LIB_DIR)/spi.a $(FEMTOOS_LIB_DIR)/can_base.a $(FEMTOOS_LIB_DIR)/application.a

kuechenCAN.o: headers 
	$(GCC) $(CFLAGS) -c kuechenCAN.c -o kuechenCAN.o

clean:
	rm -f kuechenCAN.o
	rm -f kuechenCAN.i
	rm -f kuechenCAN.s
	rm -f main.elf
	rm -f main.map
	rm -f main.hex
	make FEMTOOS_LIB_DIR=../$(FEMTOOS_LIB_DIR) FEMTOOS_AVR_TOOLDIR=$(FEMTOOS_AVR_TOOLDIR) FEMTOOS_SOURCE=$(FEMTOOS_SOURCE) TARGET=$(TARGET) CFLAGS='$(CFLAGS)' ASMFLAGS='$(ASMFLAGS)' -C $(FEMTOOS_DIR) clean
	make FEMTOOS_LIB_DIR=../$(FEMTOOS_LIB_DIR) FEMTOOS_AVR_TOOLDIR=$(FEMTOOS_AVR_TOOLDIR) FEMTOOS_SOURCE=$(FEMTOOS_SOURCE) TARGET=$(TARGET) CFLAGS='$(CFLAGS)' ASMFLAGS='$(ASMFLAGS)' -C $(FEMTOOS_APPLICATIONS_DIR) clean


