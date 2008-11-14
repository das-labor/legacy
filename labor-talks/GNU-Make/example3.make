all: programm

programm: code_a.o code_b.o code_c.o
	gcc -o $@ $^

code_a.o: code_a.c code_a.h
code_b.o: code_b.c code_b.h code_a.h code_c.h
code_c.o: code_c.c code_c.h

%.o: %.c
	gcc -c -o $@ $<

