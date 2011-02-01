TOOLCHAIN      = arm-elf-
MCU_TARGET     = cortex-m3  
OPTIMIZE       = -O2
DEBUG          = -g
WARNING        = -pedantic -Wall -Wstrict-prototypes
DEFS           = -D$(call uc, $(subst -,_,$(MCU_TARGET)))
FLASHCMD       = $(TOOLCHAIN)gdb -x gdb-flash  #
DEP_DIR        = deps/
TEST_DIR       = test/
BIN_DIR        = bin/
TESTSRC_DIR    = test_src/
ERASECMD       =
TESTPORT       = /dev/ttyUSB1
TESTPORTBAUDR  = 115200
TESTLOG_DIR    = testlog/#
TESTPREFIX     = nessie-
SPEEDTOOL      = host/get_performance.rb
SPEEDLOG_DIR   = speed_log/
SPEEDPREFIX    =
SPEEDCMD       = performance
SIZE_DIR       = size_log/#
LIST_DIR       = listings/#
STAT_DIR       = stats/#
AUTOASM_DIR    = autoasm/#
AUTOASM_OPT    = -S
CC	       = $(TOOLCHAIN)gcc
CSTD           = gnu99

override CFLAGS_A	= -fomit-frame-pointer  \
                      -mthumb -ffunction-sections -fdata-sections -MMD \
                      -MF$(DEP_DIR)$(patsubst %.o,%.d,$(notdir $(1)))  \
                      $(DEBUG) $(WARNING) -std=$(CSTD) $(OPTIMIZE)     \
                      -mcpu=$(MCU_TARGET) -Wa,-mthumb $(DEFS) 
override CFLAGS 	= -fomit-frame-pointer   \
                      -mthumb -ffunction-sections -fdata-sections -MMD \
                      -MF$(DEP_DIR)$(patsubst %.o,%.d,$(notdir $@))    \
                      $(DEBUG) $(WARNING) -std=$(CSTD) $(OPTIMIZE)     \
                      -mcpu=$(MCU_TARGET) -Wa,-mthumb $(DEFS)  

override LDFLAGS       = -g -T lm3s9b90.ld -Wl,--gc-sections    \
                         -Wl,--entry=reset_isr -lc -lgcc        \
                          -Wl,-Map,# no space at the end
override ASFLAGS	   = -mthumb -mcpu=$(MCU_TARGET) -Wa,--g -Wa,-mthumb

SIZESTAT_FILE = sizestats.txt

OBJCOPY         = $(TOOLCHAIN)objcopy
OBJDUMP         = $(TOOLCHAIN)objdump
SIZE            = $(TOOLCHAIN)size
READELF         = readelf
RUBY            = ruby
GET_TEST        = host/get_test.rb
MAKE            = make
MAKE2GRAPH      = ~/bin/make2graph.rb
TWOPI           = twopi
