
CC=gcc
all:servicemap server client
servicemap:servicemap.o
	$(CC) servicemap.c -o servicemap
server:server.o
	$(CC) server.c -o server
client:client.o
	$(CC) client.c -o client
clean:
	rm *.o servicemap server client
