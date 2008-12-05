TARGET = objects_avr
TOPDIR = ..

include $(TOPDIR)/defaults.mk

#for AVR
SRC  = prng.c persistentCounter.c
ASRC = noekeon_asm.S memxor.S

#for simulator
SRC_SIM  = prng.c persistentCounter.c noekeon.c memxor_c.c




include $(TOPDIR)/rules.mk
