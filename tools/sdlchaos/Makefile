# Makefile for the sdlChaos project
#
#    This file is part of the sdlChaos.
#    Copyright (C) 2011 Daniel Otte (daniel.otte@rub.de)
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
TARGET = sdlChaos
OBJECTS = main_sdl.o \
          mandelbrot.o 
OPTIMIZE = -O2
MACHINE  = -march=pentium4
OPTIONS  = -msse2
LIBS = SDL
DEP_DIR = deps/#          
MAPFILE = main.map

CFLAGS = $(OPTIMIZE) $(MACHINE) $(OPTIONS) \
         -MMD -MF$(DEP_DIR)$(patsubst %.o,%.d,$(notdir $@)) \
         -std=gnu99 -Wall -pedantic  
		
CLDFLAGS =  $(OPTIMIZE) $(MACHINE) $(OPTIONS)  \
            $(patsubst %, -l%, $(LIBS)) 
LDFLAGS = $(CLDFLAGS)

PREFIX = #
CC=$(PREFIX)gcc
LD=$(PREFIX)ld
AS=$(PREFIX)as
GDB=$(PREFIX)gdb

.PHONY: init
init:
	mkdir $(DEP_DIR)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CLDFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f *.o $(TARGET) $(MAPFILE)

.PHONY: depclean
depclean: clean
	rm -f $(DEP_DIR)*.d
	
#-------------------------------------------------------------------------------
# dependency inclusion
#

DEPS := $(wildcard $(DEP_DIR)*.d)

ifneq ($(DEPS),)
include $(DEPS)
endif	
	
