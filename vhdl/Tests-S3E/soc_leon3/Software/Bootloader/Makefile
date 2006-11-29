
SPARC_CC=sparc-rtems-gcc
SPARC_LD=sparc-rtems-ld
SPARC_OBJCOPY=sparc-rtems-objcopy
SPARC_OBJDUMP=sparc-rtems-objdump
SRECTOVHDL=~/bin/srectovhdl

CFLAGS=-O2 -Wall -s
LDFLAGS=-Tlinkcmd
SEGMENTS = -j .text -j .data

all: test.vhdl

test: traptable.o startup.o spike_hw.o main.o
	$(SPARC_LD) $(LDFLAGS) -e_trap_table -Map test.map -N -o test traptable.o startup.o main.o spike_hw.o

test.lst: test
	$(SPARC_OBJDUMP) --disassemble test > test.lst

test.bin: test
	$(SPARC_OBJCOPY) $(SEGMENTS) -O srec test test.bin

test.srec: test
	$(SPARC_OBJCOPY) $(SEGMENTS) -O srec test test.srec

test.vhdl: test.srec
	$(SRECTOVHDL) test.srec test.vhdl > /dev/null


traptable.o: traptable.S
	$(SPARC_CC) $(CFLAGS) -c traptable.S

startup.o: startup.S
	$(SPARC_CC) $(CFLAGS) -c startup.S

spike_hw.o: spike_hw.c
	$(SPARC_CC) $(CFLAGS) -c spike_hw.c


main.o: main.c
	$(SPARC_CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o
	rm -f test test.map test.bin test.srec test.lst test.vhdl

