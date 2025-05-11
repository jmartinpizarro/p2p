CC = gcc
CFLAGS = -Wall -pthread

all: server

server: server.c
	$(CC) $(CFLAGS) -o server server.c

clean:
	rm -f server
