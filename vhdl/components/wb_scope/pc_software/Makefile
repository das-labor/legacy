CFLAGS=-O2
SERIAL?=/dev/ttyS0

all: scope2vcd

clean:
	rm -f *.o
	rm -f scope2vcd

sample: scope2vcd
	./scope2vcd $(SERIAL) > scope1.vcd

view:
	gtkwave scope1.vcd scope1.sav

