
CC=gcc
CFLAGS=-g

heap.o: heap.c
	$(CC) $(CFLAGS) -c -o heap.o heap.c