GPP = gcc
SRCS1 = sample1.c
SRCS2 = sample2.c
SRCS3 = sample3.c
SRCS4 = sample4.c textures.c

OBJS1 = $(SRCS1:.c=.o)
OBJS2 = $(SRCS2:.c=.o)
OBJS3 = $(SRCS3:.c=.o)
OBJS4 = $(SRCS4:.c=.o)

LIBS = -lGL -lGLU -lXext -lX11 -lm -lglut -lXi -lXmu

default : sample1 sample2 sample3 sample4

sample1 : $(OBJS1)
	$(GPP) -o sample1 $(OBJS1) $(LIBS)

sample2 : $(OBJS2)
	$(GPP) -o sample2 $(OBJS2) $(LIBS)

sample3 : $(OBJS3)
	$(GPP) -o sample3 $(OBJS3) $(LIBS)

sample4 : $(OBJS4)
	$(GPP) -o sample4 $(OBJS4) $(LIBS)

.SUFFIXES:.c

.c.o:
	$(GPP) -c $*.c

clean :
	rm -f *.o sample1 sample2 sample3 sample4
