
MCU_TARGET     = cortex-m3  
OPTIMIZE       = -O2
DEFS           = -D$(call uc, $(subst -,_,$(MCU_TARGET)))
FLASHCMD       = arm-elf-gdb -x gdb-flash  #
DEP_DIR        = deps/
TEST_DIR       = test/
BIN_DIR        = bin/
TESTBIN_DIR    = test_bin/
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
CC	       = arm-elf-gcc
CSTD           = gnu99

override CFLAGS_A	= -mthumb -ffunction-sections -fdata-sections -MMD \
                      -MF$(DEP_DIR)$(patsubst %.o,%.d,$(notdir $(1)))  \
                      -I. -Itest_src -g -pedantic -std=$(CSTD)         \
                      -Wall  $(OPTIMIZE) -Wstrict-prototypes           \
                      -mcpu=$(MCU_TARGET) $(DEFS) 
override CFLAGS 	= -mthumb -ffunction-sections -fdata-sections -MMD \
                      -MF$(DEP_DIR)$(patsubst %.o,%.d,$(notdir $@))    \
                      -I. -Itest_src -g -pedantic -std=$(CSTD)         \
                      -Wall   $(OPTIMIZE) -Wstrict-prototypes          \
                      -mcpu=$(MCU_TARGET) $(DEFS)  

override LDFLAGS       = -g -T lm3s9b90.ld -Wl,--gc-sections    \
                         -Wl,--entry=reset_isr -lc -lgcc        \
                         -Wl,-Map,# no space at the end
override ASFLAGS	   = -mthumb -mcpu=$(MCU_TARGET) -Wa,--g

SIZESTAT_FILE = sizestats.txt

OBJCOPY         = arm-elf-objcopy
OBJDUMP         = arm-elf-objdump
SIZE            = arm-elf-size
READELF         = readelf
RUBY            = ruby
GET_TEST        = host/get_test.rb
MAKE            = make
MAKE2GRAPH      = ~/bin/make2graph.rb
TWOPI           = twopi
