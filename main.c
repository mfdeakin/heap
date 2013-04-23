/* cplay
 * By Michael Deakin
 * CSC 460
 * /home/deakin_mf/csc460/hw/cplay
 * February 5th, 2013
 * To build: make
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "heap.h"
#include "list.h"

void printmenu(void);

void cmdClear(heap *ops);
void cmdDelete(heap *ops);
void cmdInsert(heap *ops);
void cmdPeek(heap *ops);
void cmdQuit(heap *ops);

int main(int argc, char **argv)
{
	heap *hp = hpCreate((int (*)(void *, void *))strcmp);
	printmenu();
	char cmd;
	/* Because I am a lazy programmer and don't have 
	 * a good map interface in C and like functional programming,
	 * I'll just do this terrible thing */
	void (*commands[1 << (sizeof(cmd) * 8)])(heap *);
	memset(commands, 0, sizeof(commands));
	commands['c'] = cmdClear;
	commands['d'] = cmdDelete;
	commands['i'] = cmdInsert;
	commands['q'] = cmdQuit;
	commands['p'] = cmdPeek;
	for(;;) {
		char buf;
		printf(">> ");
		/* Read the command and skip to the end */
		fread(&cmd, sizeof(cmd), 1, stdin);
		do {
			fread(&buf, sizeof(buf), 1, stdin);
		} while(buf != '\n');
		cmd = tolower(cmd);
		if(commands[(int)cmd])
			commands[(int)cmd](hp);
	}
}

void cmdClear(heap *ops)
{
	while(hpSize(ops) > 0)
		cmdDelete(ops);
}

void cmdDelete(heap *ops)
{
	if(hpSize(ops) > 0) {
		char *data = (char *)hpTop(ops);
		printf("Top: %s\n", data);
		free(data);
	}
	else {
		printf("Heap is already empty\n");
	}
}

void cmdPeek(heap *ops)
{
	if(hpSize(ops) > 0) {
		printf("Top: %s\n", (char *)hpPeek(ops));
	}
	else {
		printf("Heap is empty\n");
	}
}

void cmdInsert(heap *ops)
{
	printf("String: ");
	char *buf = NULL;
	list *strbuf = listCreate();
	for(;;) {
		buf = malloc(sizeof(*buf));
		fread(buf, sizeof(*buf), 1, stdin);
		if(*buf == '\n')
			break;
		listInsert(strbuf, buf);
	}
	free(buf);
	int listsize = listSize(strbuf),
		i;
	buf = malloc(sizeof(char[listsize + 1]));
	memset(buf, 0, sizeof(char[listsize + 1]));
	for(i = 0, listMoveBack(strbuf);
			i < listsize;
			i++, listMoveBack(strbuf)) {
		char *tmp = listGetCurrent(strbuf);
		buf[i] = *tmp;
		free(tmp);
	}
	listFree(strbuf);
	hpAdd(ops, buf);
}

void cmdQuit(heap *ops)
{
	cmdClear(ops);
	hpFree(ops);
	exit(0);
}

void printmenu(void)
{
	puts("'c'  to clear the heap,\n"
			 "'d'  to delete the current string,\n"
			 "'i'  to insert a new string into the heap (will need to prompt for string),\n"
			 "'p'  to peek at the current string,\n"
			 "'q'  to quit this program.\n");
}
