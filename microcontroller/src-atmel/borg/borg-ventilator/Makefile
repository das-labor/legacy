
OBJ            = borg_hw.o main.o util.o pixel.o programm.o  memxor.S noekeon_asm.S \
				 spi.o scrolltext3.o font_arial8.o joystick.o snake.o \
				 eeprom_reserve.o persistentCounter.o prng.o matrix.o
CANADDR        = 0x42

include make/avr.mk

#.PHONY: tetris/tetris.o
#tetris/tetris.o:
#	$(MAKE) "MCU_CC=$(MCU_CC)" "MCU_LD=$(MCU_LD)" "LDFLAGS=$(LDFLAGS)" \
#		"CFLAGS=$(CFLAGS) -DNDEBUG -pedantic -std=c99" --directory=tetris
