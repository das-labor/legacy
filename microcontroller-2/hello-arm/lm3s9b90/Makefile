# Makefile for the Hello-ARM project
#
#    This file is part of the Hello-ARM.
#    Copyright (C) 2010 Daniel Otte (daniel.otte@rub.de)
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
TARGET = main.elf
OBJECTS = main.o \
          startup.o \
          circularbytebuffer.o \
          uart_i.o \
          sysclock.o \
          hexdigit_tab.o \
          cli.o \
          string-extras.o \
          dump.o \
          hw_gptm.o \
          ethernet.o
          
LINKERSCRIPT = lm3s9b90.ld
MAPFILE = main.map
DEP_DIR = deps/

CFLAGS = -mthumb -mcpu=cortex-m3 \
         -O0 \
         -ffunction-sections -fdata-sections \
         -MMD -MF$(DEP_DIR)$(patsubst %.o,%.d,$(notdir $@)) \
         -std=gnu99 -Wall -pedantic -gdwarf-2 
		
CLDFLAGS = -gdwarf-2 -Wl,--entry=reset_isr -Wl,--gc-sections -Wl,-Map=main.map -lc -lgcc \
          -T $(LINKERSCRIPT) -mthumb -mcpu=cortex-m3
LDFLAGS = -gdwarf-2 --entry=reset_isr --gc-sections -Map=$(MAPFILE) -lc -lgcc \
          -T $(LINKERSCRIPT) -mthumb -mcpu=cortex-m3

PREFIX = arm-none-eabi-#
CC=$(PREFIX)gcc
LD=$(PREFIX)ld
AS=$(PREFIX)as
GDB=$(PREFIX)gdb

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CLDFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f *.o $(TARGET) $(MAPFILE)

.PHONY: depclean
depclean: clean
	rm -f $(DEP_DIR)*.d
	
.PHONY: flash	
flash: $(TARGET)
	$(GDB) -x gdb-flash $(TARGET)
	
#-------------------------------------------------------------------------------
# dependency inclusion
#

DEPS := $(wildcard $(DEP_DIR)*.d)

ifneq ($(DEPS),)
include $(DEPS)
endif	
	
