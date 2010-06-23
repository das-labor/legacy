
CFLAGS = -march=i686 -mmmx -msse -msse2 -msse3 -O2 -pipe -fomit-frame-pointer

all: bmw224sum bmw256sum bmw384sum bmw512sum size64sum

clean:
	rm -f bmw224sum bmw256sum bmw384sum bmw512sum size64sum *.o

bmw224sum: main.o hash_bmw224.o bmw_small_speed.o memxor.o
	$(CC) $(CFLAGS) -o $@ $^

bmw256sum: main.o hash_bmw256.o bmw_small_speed.o memxor.o
	$(CC) $(CFLAGS) -o $@ $^

bmw384sum: main.o hash_bmw384.o bmw_large_speed.o memxor.o
	$(CC) $(CFLAGS) -o $@ $^

bmw512sum: main.o hash_bmw512.o bmw_large_speed.o memxor.o
	$(CC) $(CFLAGS) -o $@ $^

size64sum: main.o hash_size.o 
	$(CC) $(CFLAGS) -o $@ $^
	