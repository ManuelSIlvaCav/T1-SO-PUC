CC=gcc -std=gnu11
CFLAGS=-Wunused

all: cliente servidor

cliente: client.o board.o
		$(CC) -o cliente client.o board.o $(CFLAGS) -lm -pthread

servidor: server.o board.o
		$(CC) -o servidor server.o board.o $(CFLAGS) -lm -pthread
