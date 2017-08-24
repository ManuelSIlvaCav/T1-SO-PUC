CC=gcc -std=gnu11
CFLAGS=-Wunused -lm

simulator: main.o queue.o process.o
		$(CC) -o simulator main.o queue.o process.o $(CFLAGS)
