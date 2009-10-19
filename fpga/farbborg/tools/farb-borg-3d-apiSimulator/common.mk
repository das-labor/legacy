PRG  = farbborgSim
MAIN_OBJ = main.o fileParser.o trackball.o util.o api.o displayLoop.o 

ANIM_OBJ =  anim_fnordlicht.o testAnim.o slh.o colorMatrix.o colorSnake.o plasmaAnims.o

# Put the OBJ in .obj folder
OBJ = $(MAIN_OBJ) $(ANIM_OBJ)
OBJ_2    = $(patsubst %.o,.obj/%.o,$(OBJ))      
OPTIMIZE = -O2
CFLAGS   = -g -Wall $(OPTIMIZE)
LDFLAGS ?= -Wl,-Map,$(PRG).map 
