CC=gcc -g
CFLAGS=-c -Wall

local: local.o csv_parser.o customer_manager.o queries.o utils.o
	$(CC) local.o csv_parser.o customer_manager.o queries.o utils.o -o local

local.o: main.c
	$(CC) $(CFLAGS) main.c -o local.o

server: server.o csv_parser.o customer_manager.o queries.o utils.o
	$(CC) server.o csv_parser.o customer_manager.o queries.o utils.o -o server -pthread

server.o: server.c
	$(CC) $(CFLAGS) server.c

client: client.o 
	$(CC) client.o -o client

client.o: client.c
	$(CC) $(CFLAGS) client.c

csv_parser.o: csv_parser.c csv_parser.h
	$(CC) $(CFLAGS) csv_parser.c

customer_manager.o: customer_manager.c customer_manager.h
	$(CC) $(CFLAGS) customer_manager.c

queries.o: queries.c queries.h
	$(CC) $(CFLAGS) queries.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) utils.c

clean:
	rm -rf *.o local