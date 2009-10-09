# Make folders
.obj:
	mkdir .obj
.dep:
	mkdir .dep
	
#compile
.obj/%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -MF .dep/$<.d -c $< -o $@

DEPS := $(wildcard .dep/*.d)
ifneq ($(DEPS),)
include $(DEPS)
endif 
