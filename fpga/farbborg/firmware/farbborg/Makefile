# Please add new animiations here

ANIM_OBJ  = anim_fnordlicht.o 
ANIM_OBJ += anim_colorMatrix.o
ANIM_OBJ += anim_colorSnake.o
ANIM_OBJ += anim_movingCubes.o
ANIM_OBJ += anim_took.o
ANIM_OBJ += anim_tutorial.o
ANIM_OBJ += anim_police.o
ANIM_OBJ += anim_Benny.o
ANIM_OBJ += plasmaAnims.o
ANIM_OBJ += testAnim.o

# for "make sdcard" please define the PATH_SDCARD to the path of your mounted sdcard.

# for serial upload "make upload" with the bootloader define LM32_BOOTLOADER_SERIALPORT
# for example "/dev/tty.usbserial", and you need ruby with the serial port extantion
# "serialport.so"

LM32_CC=lm32-elf-gcc
LM32_LD=lm32-elf-gcc
LM32_OBJCOPY=lm32-elf-objcopy
LM32_OBJDUMP=lm32-elf-objdump

# the main files of the farbborg
MAIN_OBJ = crt0ram.o main.o api.o slh.o ff.o mmc.o fileParser.o \
		   spike_hw.o util.o displayLoop.o

OBJ_FOLDER = obj
DEP_FOLDER = dep

OBJ = $(patsubst %.o,$(OBJ_FOLDER)/%.o,$(MAIN_OBJ) $(ANIM_OBJ))

CPU_FLAGS=-mbarrel-shift-enabled -mmultiply-enabled -msign-extend-enabled -mdivide-enabled
CFLAGS=-O2 -ffunction-sections -g2 -g -Wall -D__lm32__ -fomit-frame-pointer $(CPU_FLAGS) 
LDFLAGS=$(CPU_FLAGS) -Tram.ld -lm -lc -lgcc -Wl,--gc-sections
SEGMENTS = -j .text -j .data -j .rodata

all: $(OBJ_FOLDER) $(DEP_FOLDER) image.bin image.lst image.srec Makefile 

$(OBJ_FOLDER)/%.o: %.S
	$(LM32_CC) -MMD -MP -MF $(DEP_FOLDER)/$<.d $(CFLAGS) -c $< -o $@

$(OBJ_FOLDER)/%.o: %.c
	$(LM32_CC) -MMD -MP -MF $(DEP_FOLDER)/$<.d $(CFLAGS) -c $< -o $@

image: $(OBJ)
	$(LM32_LD) $(LDFLAGS) -o image $(OBJ)

image.lst: image
	$(LM32_OBJDUMP) -h -S $< > $@

image.bin: image
	$(LM32_OBJCOPY) $(SEGMENTS) -O binary image image.bin

image.srec: image image.lst
	$(LM32_OBJCOPY) $(SEGMENTS) -O srec image image.srec

sdcard: image.bin
	mkdir -f $(PATH_SDCARD)/sys
	cp image.bin $(PATH_SDCARD)/sys/firmware.bin

# funzt leider nicht
#	ifndef ($(PATH_SDCARD))
#		$(error Define the enviroment variable PATH_SDCARD!)
#	else
#		mkdir -f $(PATH_SDCARD)/sys
#		cp image.bin $(PATH_SDCARD)/sys/firmware.bin
#	endif

upload: image.srec
	../../tools/uploader/upload.rb -s $(LM32_BOOTLOADER_SERIALPORT) -t $<

clean:
	rm -rf $(OBJ_FOLDER) $(DEP_FOLDER) image image.lst image.bin image.srec image.map 

# Make folders
$(OBJ_FOLDER):
	mkdir $(OBJ_FOLDER)
	
$(DEP_FOLDER):
	mkdir $(DEP_FOLDER)

DEPS := $(wildcard $(DEP_FOLDER)/*.d)
ifneq ($(DEPS),)
include $(DEPS)
endif 
