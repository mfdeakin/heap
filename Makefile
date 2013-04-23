
CC=gcc
CFLAGS=-g -Wall

heaptest: list.o heap.o main.c
	$(CC) $(CFLAGS) -o heaptest list.o heap.o main.c

heap.o: heap.c
	$(CC) $(CFLAGS) -c -o heap.o heap.c

list.o: list.c
	$(CC) $(CFLAGS) -c -o list.o list.c