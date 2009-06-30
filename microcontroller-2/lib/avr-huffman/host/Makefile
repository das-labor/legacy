# Makefile for the AVR-Huffman project
# author: Daniel Otte

#    This file is part of the AVR-Huffman.
#    Copyright (C) 2009  Daniel Otte (daniel.otte@rub.de)
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.


SHELL = sh
OPTIMIZE   = -O2
CC         = gcc
CSTD       = c99
DEP_DIR    = deps/
override CFLAGS	= -MMD -MF$(DEP_DIR)$(patsubst %.c,%.d,$<) -I. -gdwarf-2 -pedantic -std=$(CSTD) -Wall -Wstrict-prototypes $(OPTIMIZE) $(DEFS)

all: huffman-decode huffman-encode bin2avrS

clean:
	rm -f huffman-decode huffman-encode bin2avrS
	
%.hfm: %
	./huffman-encode $<

#%.S: %
#	./bin2avrS $<

DEPS := $(wildcard $(DEP_DIR)*.d)

ifneq ($(DEPS),)
include $(DEPS)
endif
