# Put new animations here!!
ANIM_OBJ  = anim_fnordlicht.o 
ANIM_OBJ += anim_colorMatrix.o
ANIM_OBJ += anim_colorSnake.o
ANIM_OBJ += anim_movingCubes.o
ANIM_OBJ += anim_ansteuerungZeitlupe.o
ANIM_OBJ += plasmaAnims.o
ANIM_OBJ += testAnim.o

# Name of the programm
PRG  = farbborgSim
OBJ_FOLDER = obj
DEP_FOLDER = dep
# Objets needed anyway
MAIN_OBJ = main.o trackball.o fileParser.o slh.o util.o api.o displayLoop.o 


OBJ = $(MAIN_OBJ) $(ANIM_OBJ)
# Put the OBJ in .obj folder
OBJ_2    = $(patsubst %.o,$(OBJ_FOLDER)/%.o,$(OBJ))  
OPTIMIZE = -O2
CFLAGS   = -g -Wall $(OPTIMIZE)
LDFLAGS ?= -Wl,-Map,$(PRG).map 
