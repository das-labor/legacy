all: programm

programm: code_a.o code_b.o code_c.o
	gcc -o programm code_a.o code_b.o code_c.o

code_a.o: code_a.c code_a.h
	gcc -c -o code_a.o code_a.c
code_b.o: code_b.c code_b.h code_a.h code_c.h
	gcc -c -o code_b.o code_b.c
code_c.o: code_c.c code_c.h
	gcc -c -o code_c.o code_c.c

