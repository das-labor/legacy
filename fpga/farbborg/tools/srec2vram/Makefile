CC   ?= gcc
CFLAGS ?= -O2 -Wall

all: srec2vram

srec2vram: srec2vram.c
	$(CC) $(CFLAGS) -o srec2vram srec2vram.c

clean:
	rm -f srec2vram
