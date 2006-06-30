TRG = test
OBJ    = test.rel

# This first section is declaring some variables which will be
# used in the rules below.  That way, you can specify some
# important options to be used everywhere and only need to
# edit one place to make the change apply everywhere (that
# makes use of a particular variable).

# Usually SDCC's small memory model is the best choice.  If
# you run out of internal RAM, you will need to declare
# variables as "xdata", or switch to large model

SDCCCFLAGS = --model-small

# These settings control where the compiler will place the code
# and variables in memory.  The executable code will begin at
# 4000.  Internal ram usage for variables will begin at 30 (which
# is just after the 8051's 4 register banks and bit variables).
# Variables in external RAM ("xdata" type) will begin at 1000.

ASLINKFLAGS = --code-loc 0x0000 --xram-loc 0xC000


# This "implicit" rule tells make that any file ending in .rel is
# build from two files with the same base name ending in .c and .h,
# and that the command "sdcc $(SDCCCFLAGS) -c" followed by the two
# .c and .h files is the way to obtain the .rel file.  Whenever any
# of the rules below specifies that a .rel file is required, make
# will use this rule (in the absence of an explicit rule for that
# file) to build the .rel file.

%.rel: %.c
	sdcc $(SDCCCFLAGS) -c $<

$(TRG).hex: $(TRG).ihx
	packihx $(TRG).ihx > $(TRG).hex

all: $(TRG).hex

$(TRG).ihx: $(OBJ)
	sdcc $(SDCCCFLAGS) $(ASLINKFLAGS) $^



clean:
	rm -f *.hex *.ihx *.lnk *.lst *.map *.rel *.rst *.sym *.asm *.mem
