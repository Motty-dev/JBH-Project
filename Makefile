CC=gcc -g
CFLAGS=-c -Wall

local: local.o csv.o list.o userinput.o utils.o
	$(CC) local.o csv.o list.o userinput.o utils.o -o local

local.o: main.c
	$(CC) $(CFLAGS) main.c -o local.o

csv.o: csv.c csv.h
	$(CC) $(CFLAGS) csv.c

list.o: list.c list.h
	$(CC) $(CFLAGS) list.c

userinput.o: userinput.c userinput.h
	$(CC) $(CFLAGS) userinput.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) utils.c

clean:
	rm -rf *.o local