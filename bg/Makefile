CC = gcc
CFLAGS = -Wall -lX11 -lGL -lGLEW -lSDL2 -lSDL2_image

all:
	$(CC) $(CFLAGS) -o bg main.c

run:
	./bg -v quad.vert -f raymarch.frag -t lit.png
