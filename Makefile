CC=gcc -g
CFLAGS=-c -Wall

local: local.o csv.o list.o userinput.o utils.o
	$(CC) local.o csv.o list.o userinput.o utils.o -o local

local.o: main.c
	$(CC) $(CFLAGS) main.c -o local.o

server: server.o csv.o list.o userinput.o utils.o
	$(CC) server.o csv.o list.o userinput.o utils.o -o server -pthread

server.o: server.c
	$(CC) $(CFLAGS) server.c

client: client.o 
	$(CC) client.o -o client

client.o: client.c
	$(CC) $(CFLAGS) client.c

csv.o: csv.c csv.h
	$(CC) $(CFLAGS) csv.c

list.o: list.c list.h
	$(CC) $(CFLAGS) list.c

userinput.o: userinput.c userinput.h
	$(CC) $(CFLAGS) userinput.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) utils.c

clean:
	rm -rf *.o local server client