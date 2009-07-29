CC = gcc -Wall # -ansi

theatre:
	$(CC) -I/usr/local/include textures.c slides.c effects.c main.c -o main -lGL -lGLU `sdl-config --cflags --libs` -l3ds -lm

clean:
	@rm *.o main

