PRG = borgSim
OBJ = main.o pixel.o programm.o util.o font-uni53.o scrolltext3.o  trackball.o prng.o matrix.o snake.o
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
