PRG  = farbborgSim
OBJ = main.o testAnim.o slh.o colorMatrix.o colorSnake.o fileParser.o \
      trackball.o util.o api.o displayLoop.o plasmaAnims.o

# Put the OBJ in .obj folder
OBJ_2    = $(patsubst %.o,.obj/%.o,$(OBJ))      
OPTIMIZE = -O2
CFLAGS   = -g -Wall $(OPTIMIZE)
LDFLAGS ?= -Wl,-Map,$(PRG).map 
