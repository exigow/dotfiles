CC = gcc
CFLAGS = -lm -lX11 -lSDL2 -lSDL2_image -lGL -lGLEW -g -Wall

all:
	$(CC) $(CFLAGS) -o bg main.c

run-sample:
	./bg -v quad.vert -f sample.frag -t sample.png
