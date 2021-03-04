CC = gcc
CFLAGS = -Wall -lm -lX11 -lSDL2 -lSDL2_image -lGL -lGLEW

all:
	$(CC) $(CFLAGS) -o bg main.c

run:
	./bg -v quad.vert -f sample.frag -t sample.png
