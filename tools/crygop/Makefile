PRG	    = crygop
OBJ	    = main-crygop.o crygop.o bitfunctions.o exteuclid.o sha256.o shabea.o fairrnd.o
OPTIMIZE    = -O2 
LIBS	    =
DEFS        = -DCRYGOP_SUPPORT_INTERIMRESULTS

CC	     = gcc
OBJDUMP      = objdump
SED          = sed
HEXDUMP      = hexdump
RANDDEV      = /dev/urandom

override LDFLAGS       = -Wl,-Map,$(PRG).map
override CFLAGS        = -MMD -g -pedantic -std=gnu99 -Wall -Wstrict-prototypes $(OPTIMIZE) $(DEFS)
# -fnew-ra
override LDFLAGS       = -Wl,-Map,$(PRG).map
override ASFLAGS       = 


all: $(PRG) 

$(PRG): $(OBJ)


clean:
	rm -rf $(PRG) *.o  
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

depclean: clean
	rm -f *.d

lst:  $(PRG).lst

%.lst: %
	$(OBJDUMP) -h -S $< > $@

%.2dat: %.txt
	$(SED) sx-\>xxg  $^ | $(SED) sx~\>xxg > $@

keys1000x128.txt:
	$(HEXDUMP) -n `expr 1000 \* 16` -e "/8 \"%16.16X\" /8 \"%16.16X\n\"" $(RANDDEV) > $@

DEPS := $(wildcard *.d)
ifneq ($(DEPS),)
include $(DEPS)
endif

