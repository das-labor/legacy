PRG = borg3dSim
OBJ = main.o pixel3d.o rotatedAnimations.o randomAnmations.o testPrograms.o joystick.o basicAnimations.o programs.o util.o trackball.o scrolltext2.o
OPTIMIZE = -O2
LIBS = -lglut -lpthread -lGL -lGLU
CC = gcc-4.0

OPTIMIZE = -O2
override CFLAGS        = -g -Wall $(OPTIMIZE)
$(DEFS)
override LDFLAGS       = -Wl,-Map,$(PRG).map

all: $(PRG) 

$(PRG): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf *.o $(PRG) $(PRG).map
