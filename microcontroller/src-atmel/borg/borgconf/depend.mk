# How to build automatic dependencies

# Don't include dependencies if $(no_deps) is set; the master makefile
# does this for clean and other such targets that don't need
# dependencies.  That then avoids rebuilding dependencies.

ifneq ($(no_deps),t)
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),mrproper)
ifneq ($(MAKECMDGOALS),menuconfig)  

# For each .o file we need a .d file.
#-include $(subst .o,.d,$(filter %.o,$(OBJECTS))) /dev/null
-include $(subst .o,.d,$(filter %.o,$(OBJECTS)))

endif
endif
endif
endif

# Here is how to build those dependency files

define make-deps
echo "checking dependencies for $<"
if [ ! -d obj_avr ]; then mkdir obj_avr ; fi
set -e; $(CC) $(CFLAGS) $(CPPFLAGS) -M -MM $<  | \
sed > $@.new -e 's;$(*F)\.o:;$@ obj_avr/$*.o obj_avr/$*.E $*.s:;' \
	     -e 's% [^ ]*/gcc-lib/[^ ]*\.h%%g'
if test -s $@.new; then mv -f $@.new $@; else rm -f $@.new; fi
endef

# Here is how to make .d files from .c files
obj_avr/%.d: %.c ; @ $(make-deps)

obj_avr/%.d: %.S ; @ $(make-deps)
