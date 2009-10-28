# Make folders
$(OBJ_FOLDER):
	mkdir $(OBJ_FOLDER)
$(DEP_FOLDER):
	mkdir $(DEP_FOLDER)
	
#compile
$(OBJ_FOLDER)/%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_FOLDER)/$<.d -c $< -o $@

DEPS := $(wildcard $(DEP_FOLDER)/*.d)
ifneq ($(DEPS),)
include $(DEPS)
endif 
