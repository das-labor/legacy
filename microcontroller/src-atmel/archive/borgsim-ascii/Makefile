PRG            = borg
OPTIMIZE       = -Os
CC            = gcc 
LIBS          = curses
# Override is only needed by avr-lib build system.

all:
	$(CC) $(CFLAGS) $(LDFLAGS) -o ${PRG} -l$(LIBS) main.c simpixel.c programm.c util.c

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak 
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)
	rm borg

