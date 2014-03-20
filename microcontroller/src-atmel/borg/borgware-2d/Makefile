TARGET     := image
TARGET_SIM := borgsim
TOPDIR = .

SRC = \
	main.c            \
	display_loop.c    \
	eeprom_reserve.c  \
	pixel.c           \
	util.c            \

SRC_SIM = \
	display_loop.c    \
	pixel.c           \


LAUNCH_BOOTLOADER = launch-bootloader
#SERIAL = /dev/ttyUSB0
SERIAL = COM6
export TOPDIR
##############################################################################

all: compile-$(TARGET)
	@echo "==============================="
	@echo "$(TARGET) compiled for: $(MCU)"
	@echo "size is: "
	@$(CONFIG_SHELL) ${TOPDIR}/scripts/size $(TARGET)
	@echo "==============================="
	@echo ""
	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo "@                                                                          @"
	@echo "@  The subversion repository of this project is NOT MAINTAINED anymore!    @"
	@echo "@  Borgware-2D is now hosted on GitHub. To get the latest version,         @"
	@echo "@  install git on your system and type:                                    @"
	@echo "@                                                                          @"
	@echo "@    git clone https://github.com/das-labor/borgware-2d.git                @"
	@echo "@    git submodule init                                                    @"
	@echo "@    git submodule update                                                  @"
	@echo "@                                                                          @"
	@echo "@  The last two commands incorporate the rfm12 repo into this project.     @"
	@echo "@                                                                          @"
	@echo "@  BTW: You will get rid of this warning if you switch to the git repo ;)  @"
	@echo "@                                                                          @"
	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"

##############################################################################
# generic fluff
include defaults.mk
#include $(TOPDIR)/rules.mk

##############################################################################
# generate SUBDIRS variable
#

.subdirs: autoconf.h
	@ echo "checking in which subdirs to build"
	@ $(RM) -f $@
	@ echo "SUBDIRS += animations" >> $@
	@ echo "SUBDIRS += animations/bitmapscroller" >> $@
	@ echo "SUBDIRS += smallani" >> $@
	@ (for subdir in `grep -e "^#define .*_SUPPORT" autoconf.h \
	      | sed -e "s/^#define //" -e "s/_SUPPORT.*//" \
	      | tr "[A-Z]\\n" "[a-z] " `; do \
	  test -d $$subdir && echo "SUBDIRS += $$subdir" ; \
	done) | sort -u >> $@

ifneq ($(no_deps),t)
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),mrproper)
ifneq ($(MAKECMDGOALS),menuconfig)

include $(TOPDIR)/.subdirs
include $(TOPDIR)/.config
include $(TOPDIR)/games/games.mk

endif # MAKECMDGOALS!=menuconfig
endif # MAKECMDGOALS!=mrproper
endif # MAKECMDGOALS!=clean
endif # no_deps!=t


##############################################################################

SUBDIRS_AVR = borg_hw
SUBDIRS_AVR += $(SUBDIRS)

.PHONY: compile-subdirs_avr
compile-subdirs_avr:
	@ for dir in $(SUBDIRS_AVR); do $(MAKE) -C $$dir objects_avr || exit 5; done

.PHONY: compile-$(TARGET)
compile-$(TARGET): compile-subdirs_avr $(TARGET).hex $(TARGET).bin $(TARGET).lst



OBJECTS += $(patsubst %.c,./obj_avr/%.o,${SRC})
SUBDIROBJECTS = $(foreach subdir,$(SUBDIRS_AVR),$(foreach object,$(shell cat $(subdir)/obj_avr/.objects 2>/dev/null),$(subdir)/$(object)))

$(TARGET): $(OBJECTS) $(SUBDIROBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(SUBDIROBJECTS) $(LIBS)


##############################################################################
#generic rules for AVR-Build
./obj_avr/%.o: %.c
	@ if [ ! -d obj_avr ]; then mkdir obj_avr ; fi
	@ echo "compiling $<"
	@ $(CC) -o $@ $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -c $<

%.hex: %
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.bin: %
	$(OBJCOPY) -O binary -R .eeprom $< $@

%.eep.hex: %
	$(OBJCOPY) --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex -j .eeprom $< $@

%.lst: %
	$(OBJDUMP) -h -S $< > $@

%-size: %.hex
	$(SIZE) $<

##############################################################################
#Rules for simulator build

SUBDIRS_SIM  = simulator
SUBDIRS_SIM += $(SUBDIRS)

.PHONY: compile-subdirs_sim
compile-subdirs_sim:
	@ for dir in $(SUBDIRS_SIM); do $(MAKE) -C $$dir objects_sim || exit 5; done
	@ $(MAKE) -C ./simulator/ objects_sim || exit 5;

simulator: autoconf.h .config .subdirs compile-subdirs_sim $(TARGET_SIM)
	@echo ""
	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@echo "@                                                                          @"
	@echo "@  The subversion repository of this project is NOT MAINTAINED anymore!    @"
	@echo "@  Borgware-2D is now hosted on GitHub. To get the latest version,         @"
	@echo "@  install git on your system and type:                                    @"
	@echo "@                                                                          @"
	@echo "@    git clone https://github.com/das-labor/borgware-2d.git                @"
	@echo "@    git submodule init                                                    @"
	@echo "@    git submodule update                                                  @"
	@echo "@                                                                          @"
	@echo "@  The last two commands incorporate the rfm12 repo into this project.     @"
	@echo "@                                                                          @"
	@echo "@  BTW: You will get rid of this warning if you switch to the git repo ;)  @"
	@echo "@                                                                          @"
	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"

SUBDIROBJECTS_SIM = $(foreach subdir,$(SUBDIRS_SIM),$(foreach object,$(shell cat $(subdir)/obj_sim/.objects 2>/dev/null),$(subdir)/$(object)))

OBJECTS_SIM = $(patsubst %.c,obj_sim/%.o,${SRC_SIM})

$(TARGET_SIM): $(OBJECTS_SIM) $(SUBDIROBJECTS_SIM)
	$(HOSTCC) $(LDFLAGS_SIM) -o $@ $(OBJECTS_SIM) $(SUBDIROBJECTS_SIM) $(LIBS_SIM)

./obj_sim/%.o: %.c
	@ if [ ! -d obj_sim ]; then mkdir obj_sim ; fi
	@ echo "compiling $<"
	@ $(HOSTCC) -o $@ $(CFLAGS_SIM) -c $<

##############################################################################
CONFIG_SHELL := $(shell if [ -x "$$BASH" ]; then echo $$BASH; \
          else if [ -x $$(which bash) ]; then echo $$(which bash); \
          else echo sh; fi ; fi)

menuconfig:
	@echo $(CONFIG_SHELL)
	$(MAKE) -C scripts/lxdialog all
	$(CONFIG_SHELL) scripts/Menuconfig config.in
	test -e .config
	@echo ""
	@echo "Next, you can: "
	@echo " * 'make' to compile your borgware"

#%/menuconfig:
#	$(SH) "$(@D)/configure"
#	@$(MAKE) what-now-msg

##############################################################################
clean:
	$(MAKE) -f rules.mk no_deps=t clean-common
	$(RM) $(TARGET) $(TARGET).bin $(TARGET).hex $(TARGET).lst .subdirs
	for subdir in `find . -type d` ; do \
	  test "x$$subdir" != "x." \
	  && test -e $$subdir/Makefile \
	  && $(MAKE) no_deps=t -C $$subdir clean ; done ; true

mrproper:
	$(MAKE) clean
	$(RM) -f autoconf.h .config config.mk .menuconfig.log .config.old

sflash: $(TARGET).hex
#	$(LAUNCH_BOOTLOADER) $(SERIAL) 115200
	avrdude -p m32 -b 115200 -u -c avr109 -P $(SERIAL) -U f:w:$< -F
	echo X > $(SERIAL)

uflash: $(TARGET).hex
	avrdude -c usbasp  -p atmega32 -V -U f:w:$< -F

.PHONY: clean mrproper sflash uflash
##############################################################################
# configure ethersex
#
show-config: autoconf.h
	@echo
	@echo "These modules are currently enabled: "
	@echo "======================================"
	@grep -e "^#define .*_SUPPORT" autoconf.h | sed -e "s/^#define / * /" -e "s/_SUPPORT.*//"

.PHONY: show-config

autoconf.h .config:
	@echo make\'s goal: $(MAKECMDGOALS)
ifneq ($(MAKECMDGOALS),menuconfig)
	# make sure menuconfig isn't called twice, on `make menuconfig'
	test -s autoconf.h -a -s .config || $(MAKE) no_deps=t menuconfig
	# test the target file, test fails if it doesn't exist
	# and will keep make from looping menuconfig.
	test -s autoconf.h -a -s .config
endif

include depend.mk
