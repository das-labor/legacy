# Valid parameters for mcu:
#   at90s2323
#   at90s8515
#   at90s8535
#   atmega161

#CC=avr-gcc -mmcu=at90s2323
CC=avr-gcc -mmcu=at90s8515
#CC=avr-gcc -mmcu=at90s8535
#CC=avr-gcc -mmcu=atmega161
AS=avr-as
OBJCOPY=avr-objcopy
SIZE=avr-size

CFLAGS=-Wall -I../../src -Os -mcall-prologues
ASFLAGS=-Wall -I../../src

CSRC= \
	a3a8-opt.c

SSRC=

OBJ=$(CSRC:.c=.o) $(SSRC:.S=.o)

all: $(OBJ)
	$(CC) -S $(CFLAGS) a3a8-opt.c
	$(SIZE) $(OBJ)

clean:
	rm -rf core $(OBJ) a3a8-opt.s

