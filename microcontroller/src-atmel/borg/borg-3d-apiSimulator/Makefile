PRG = borg3dSim
OBJ = main.o pixel3d.o programm.o
OPTIMIZE = -O2
LIBS = -lglut -lpthread -lGL -lGLU
CC = gcc

OPTIMIZE = -O2
override CFLAGS        = -g -Wall $(OPTIMIZE)
$(DEFS)
override LDFLAGS       = -Wl,-Map,$(PRG).map

all: $(PRG) 

$(PRG): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf *.o $(PRG) $(PRG).map
