CC = gcc -Wall

all:
	$(CC) vector.c flight.c view.c lib/3ds_draw.c lib/textures.c map_parser.c building.c infowin.c font.c main.c -o lab3d -lGL -lGLU -lm `sdl-config --cflags --libs` `lib3ds-config --cflags --libs` `freetype-config --libs --cflags` -lftgl -lexpat

clean:
	@echo Cleaning up...
	@rm lab3d
	@echo Done.
