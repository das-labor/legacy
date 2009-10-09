-include common.mk

LIBS     = -lglut -lpthread -lGL -lGLU
CC       = gcc

all: .obj .dep Makefile common.mk $(PRG) 

$(PRG): $(OBJ_2)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf .dep/ .obj/ $(PRG) 

-include common_footer.mk
