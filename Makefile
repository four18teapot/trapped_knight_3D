CFLAGS=-Wall -Werror -std=c11 -pedantic -O2
LIBS=vec3D.c ll.c hashset.c constants.c

all: main.c
	$(CC) $(CFLAGS) -o main main.c $(LIBS)
