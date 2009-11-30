DEVICE  = atmega168
F_CPU   = 16000000	# in Hz
FUSE_L  = 0x9f  # see below for fuse values for particular devices
FUSE_H  = 0xc9
AVRDUDE = avrdude -c usbasp -p $(DEVICE) # edit this line for your programmer

CFLAGS  = -I. -DDEBUG_LEVEL=0 -I../include
OBJECTS =  main.o twi_slave/twi_slave.o pc_init.o fkt.o

COMPILE = avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE) -std=c99

hex: main.hex

help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make hex ....... to build main.hex (default)"
	@echo "make program ... to flash fuses and firmware"
	@echo "make fuse ...... to flash the fuses"
	@echo "make flash ..... to flash the firmware (use this on metaboard)"
	@echo "make clean ..... to delete objects and hex file"

program: flash fuse

fkt.h:

PowerCommander_matrix.h:

PowerCommander.h:

pc_init.h:

twi_slave/twi_slave.h:

fkt.c: fkt.h PowerCommander.h PowerCommander_matrix.h

pc_init.c: pc_init.h

main.c: twi_slave/twi_slave.h PowerCommander.h pc_init.h fkt.h 

main.o: main.c twi_slave/twi_slave.h PowerCommander.h pc_init.h fkt.h 
	avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE) -std=c99 -o main.o -c main.c

twi_slave/twi_slave.o: twi_slave/twi_slave.h PowerCommander.h
	avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE) -std=c99 -o twi_slave/twi_slave.o -c twi_slave/twi_slave.c

pc_init.o: pc_init.c pc_init.h
	avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE) -std=c99 -o pc_init.o -c pc_init.c

fkt.o: fkt.c fkt.h PowerCommander.h PowerCommander_matrix.h
	avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE) -std=c99 -o fkt.o -c fkt.c


# rule for programming fuse bits:
fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
	$(AVRDUDE) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m

# rule for uploading firmware:
flash: main.hex
	$(AVRDUDE) -U flash:w:main.hex:i

# rule for deleting dependent files (those which can be built by Make):
clean:
	rm -f main.hex main.lst main.obj main.cof main.list main.map main.eep.hex main.elf *.o 
# Generic rule for compiling C files:
.c.o:
	$(COMPILE) -c $< -o $@

# Generic rule for assembling Assembler source files:
.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

# Generic rule for compiling C to assembler, used for debugging only.
.c.s:
	$(COMPILE) -S $< -o $@
# file targets:

main.elf: twi_slave/twi_slave.o main.o pc_init.o fkt.o	# usbdrv dependency only needed because we copy it
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex main.eep.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size main.hex


