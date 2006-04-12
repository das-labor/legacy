
OBJ            = main.o ks0108.o
CANADDR        = 0x42

ERASECMD       = uisp -dprog=stk200 --erase
FLASHCMD       = uisp -dprog=stk200 --upload if=$(OUT).hex

include ../../make/avr.mk


