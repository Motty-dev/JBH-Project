CC=gcc -g
CFLAGS=-c -Wall

all: main

main: main.o csv.o list.o userinput.o utils.o
	$(CC) main.o csv.o list.o userinput.o utils.o -o main

main.o: main.c
	$(CC) $(CFLAGS) main.c

csv.o: csv.c csv.h
	$(CC) $(CFLAGS) csv.c

list.o: list.c list.h
	$(CC) $(CFLAGS) list.c

userinput.o: userinput.c userinput.h
	$(CC) $(CFLAGS) userinput.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) utils.c

clean:
	rm -rf *.o main