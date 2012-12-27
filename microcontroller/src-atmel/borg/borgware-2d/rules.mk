

##############################################################################
# rules for buildung AVR objects

OBJECTS += $(patsubst %.c,obj_avr/%.o,${SRC})
OBJECTS += $(patsubst %.S,obj_avr/%.o,${ASRC})

./obj_avr/%.o: %.S
	@ if [ ! -d obj_avr ]; then mkdir obj_avr ; fi
	@ echo "assembling $<"
	@ $(CC) -o $@ $(CPPFLAGS) $(ASFLAGS) -c $<

./obj_avr/%.o: %.c
	@ if [ ! -d obj_avr ]; then mkdir obj_avr ; fi
	@ echo "compiling $<"
	@ $(CC) -o $@ $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -c $<

objects_avr: $(OBJECTS)
	@ if [ -d obj_avr ]; then echo "writing object inventory" ; echo $(OBJECTS) > obj_avr/.objects ; fi

##############################################################################
# rules for buildung simulator objects

SRC_SIM ?= $(SRC)
OBJECTS_SIM += $(patsubst %.c,obj_sim/%.o,${SRC_SIM})

./obj_sim/%.o: %.c
	@ if [ ! -d obj_sim ]; then mkdir obj_sim ; fi
	@ echo "compiling $<"
	@ $(HOSTCC) -o $@ $(CFLAGS_SIM) -c $<

objects_sim: $(OBJECTS_SIM)
	@ echo "writing object ineventory"
	@ if [ ! -d obj_sim ]; then mkdir obj_sim ; fi
	@ echo $(OBJECTS_SIM) > obj_sim/.objects




clean-common:
	$(RM) $(TARGET) *.[odasE] *.d.new *~
	$(RM) -r ./obj_avr
	$(RM) -r ./obj_sim

clean: clean-common

all:
	$(MAKE) -C $(TOPDIR) all

include $(TOPDIR)/depend.mk
