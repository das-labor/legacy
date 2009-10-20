# Put new animations here!!
ANIM_OBJ  = anim_fnordlicht.o 
ANIM_OBJ += anim_colorMatrix.o
ANIM_OBJ += anim_colorSnake.o
ANIM_OBJ += anim_movingCubes.o
ANIM_OBJ += plasmaAnims.o
ANIM_OBJ += testAnim.o

# PRG
PRG  = farbborgSim
# Objets needed anyway
MAIN_OBJ = main.o trackball.o fileParser.o slh.o util.o api.o displayLoop.o 

# Put the OBJ in .obj folder
OBJ = $(MAIN_OBJ) $(ANIM_OBJ)
OBJ_2    = $(patsubst %.o,.obj/%.o,$(OBJ))  
OPTIMIZE = -O2
CFLAGS   = -g -Wall $(OPTIMIZE)
LDFLAGS ?= -Wl,-Map,$(PRG).map 
