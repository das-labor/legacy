TOOLCHAIN      = arm-elf-#
MCU_TARGET     = cortex-m3  
MCU_OPTS       = -mthumb 
OPTIMIZE       = -Os -fomit-frame-pointer  
DEBUG          = -gdwarf-2
WARNING        = -pedantic -Wall -Wstrict-prototypes -Werror
DEFS           = -D$(call uc, $(subst -,_,$(MCU_TARGET)))
FLASHCMD       = $(OPENOCD) -f openocd.cfg \
	             -c "init" \
	             -c "halt" \
	             -c "flash write_image erase $(1) 0 bin" \
	             -c "reset run" \
	             -c "shutdown"
DEP_DIR        = deps/#
TEST_DIR       = test/#
BIN_DIR        = bin/#
TESTSRC_DIR    = test_src/#
ERASECMD       =
TESTPORT       = /dev/ttyUSB1
TESTPORTBAUDR  = 115200
TESTLOG_DIR    = testlog/#
TESTPREFIX     = nessie-#
SPEEDTOOL      = host/get_performance.rb
SPEEDLOG_DIR   = speed_log/#
SPEEDPREFIX    =
SPEEDCMD       = performance
SIZE_DIR       = size_log/#
LIST_DIR       = listings/#
STAT_DIR       = stats/#
AUTOASM_DIR    = autoasm/#
LIB_DIR        = lib/#
LIB_NAME       = libcrypto.a
AUTOASM_OPT    = -S
CSTD           = gnu99
LIBS           = c gcc

COMMON_FLAGS   = -ffunction-sections -fdata-sections          \
                 $(DEBUG) $(WARNING) -std=$(CSTD) $(OPTIMIZE) \
                 -mcpu=$(MCU_TARGET) $(MCU_OPTS) $(DEFS) -MMD

override CFLAGS_A	= $(COMMON_FLAGS) \
                      -MF$(DEP_DIR)$(patsubst %.o,%.d,$(notdir $(1))) 
override CFLAGS 	= $(COMMON_FLAGS) \
                      -MF$(DEP_DIR)$(patsubst %.o,%.d,$(notdir $@))
                      
override LDFLAGS       = -g -T lm3s9b90.ld -Wl,--gc-sections    \
                         -Wl,--entry=reset_isr  \
                         -Wl,-Map,# no space at the end
                          
override ASFLAGS	   =  -mcpu=$(MCU_TARGET) 

SIZESTAT_FILE = sizestats.txt

CC	            = $(TOOLCHAIN)gcc
OBJCOPY         = $(TOOLCHAIN)objcopy
OBJDUMP         = $(TOOLCHAIN)objdump
SIZE            = $(TOOLCHAIN)size
OPENOCD         = openocd
READELF         = readelf
AR              = $(TOOLCHAIN)ar
CHMOD           = chmod
CHOWN           = chown
CHGRP           = chgrp
RM              = rm
RUBY            = ruby
GET_TEST        = host/get_test.rb
MAKE            = make
MAKE2GRAPH      = ~/bin/make2graph.rb
TWOPI           = twopi
