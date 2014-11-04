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
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>

#include "heap.h"
#include "list.h"

typedef void (*CmdFunc)(heap *, bool);

void printmenu(void);

void cmdClear(heap *ops, bool print);
void cmdDelete(heap *ops, bool print);
void cmdInsert(heap *ops, bool print);
void cmdPeek(heap *ops, bool print);
void cmdQuit(heap *ops, bool print);
void cmdQuitNewline(heap *ops, bool print);

int runCommands(heap *hp, bool print);

int main(int argc, char **argv)
{
	heap *hp = hpCreate((int (*)(const void *, const void *))strcmp);
	if (isatty(fileno(stdin))) {
		return runCommands(hp, true);
	}
	else {
		return runCommands(hp, false);
	}
}

CmdFunc *createCmdMap()
{
	/* Because I am a lazy programmer and don't have 
	 * a good map interface in C and like functional programming,
	 * I'll just do this terrible thing */
	CmdFunc *commands = malloc(sizeof(CmdFunc[256]));
	memset(commands, 0, sizeof(commands));
	commands['c'] = cmdClear;
	commands['d'] = cmdDelete;
	commands['i'] = cmdInsert;
	commands['q'] = cmdQuit;
	commands[0] = cmdQuitNewline;
	commands['p'] = cmdPeek;
	return commands;
}

// Reads the next command from stdin.
char readCmd()
{
	char buf = 0;
	char cmd = 0;
	/* Read the command and skip to the end */
	size_t bytesRead = fread(&cmd, sizeof(cmd), 1, stdin);
	if(bytesRead != 0) {
		do {
			bytesRead = fread(&buf, sizeof(buf), 1, stdin);
		} while(buf != '\n' && bytesRead != 0);
	}
	else {
		cmd = 0;
	}
	return tolower(cmd);
}

int runCommands(heap *hp, bool print)
{
	CmdFunc *commands = createCmdMap();
	if(print)
		printmenu();
	for(int i = 0;; i++) {
		if(print)
			printf(">> ");
		char cmd = readCmd();
		if(commands[(int)cmd])
			commands[(int)cmd](hp, print);
	}
	free(commands);
}

void cmdClear(heap *ops, bool print)
{
	while(hpSize(ops) > 0)
		cmdDelete(ops, print);
}

void cmdDelete(heap *ops, bool print)
{
	if(hpSize(ops) > 0) {
		char *data = (char *)hpTop(ops);
		printf("Top: %s\n", data);
		free(data);
	}
	else if(print) {
		printf("Heap is already empty\n");
	}
}

void cmdPeek(heap *ops, bool print)
{
	if(hpSize(ops) > 0) {
		printf("Top: %s\n", (char *)hpPeek(ops));
	}
	else {
		printf("Heap is empty\n");
	}
}

void cmdInsert(heap *ops, bool print)
{
	if(print)
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

void cmdQuitNewline(heap *ops, bool print)
{
	printf("\n");
	cmdQuit(ops, print);
}

void cmdQuit(heap *ops, bool print)
{
	cmdClear(ops, print);
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
