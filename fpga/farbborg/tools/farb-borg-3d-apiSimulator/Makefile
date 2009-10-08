-include common.mk
LIBS     = -lglut -lpthread -lGL -lGLU
CC       = gcc
OBJ_2    = $(patsubst %.o,.obj/%.o,$(OBJ))
OPTIMIZE = -O2
CFLAGS        = -g -Wall $(OPTIMIZE) -MMD -MP
LDFLAGS       = -Wl,-Map,$(PRG).map

all: .obj .dep Makefile common.mk $(PRG) 

.obj:
	mkdir .obj

.dep:
	mkdir .dep

$(PRG): $(OBJ_2)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

.obj/%.o: %.c
	$(CC) $(CFLAGS) -MF .dep/$<.d -c $< -o $@

clean:
	rm -rf .dep/ .obj/ $(PRG) 

DEPS := $(wildcard .dep/*.d)
ifneq ($(DEPS),)
include $(DEPS)
endif