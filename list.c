
#include "list.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct listnode listnode;

struct listnode {
	listnode *prev, *next;
	void *data;
};

struct list {
	listnode *current;
	unsigned size;
};

int listCheck(list *lst);

list *listCreate()
{
	list *lst = malloc(sizeof(list));
	lst->current = NULL;
	lst->size = 0;
	return lst;
}

void listFree(list *lst)
{
	assert(listCheck(lst));
	if(lst->current) {
		listnode *start = lst->current;
		do {
			listnode *next = lst->current->next;
			free(lst->current);
			lst->current = next;
		} while(lst->current != start);
	}
	free(lst);
}

void listInsert(list *lst, void *data)
{
	assert(lst && listCheck(lst));
	listnode *node = malloc(sizeof(listnode));
	if(lst->current) {
		node->next = lst->current;
		node->prev = lst->current->prev;
		node->prev->next = node;
		node->next->prev = node;
		if(lst->current->next == lst->current) {
			lst->current->next = node;
		}
	}
	else {
		node->next = node;
		node->prev = node;
	}
	lst->current = node;
	node->data = data;
	lst->size++;
}

void listDeleteCurrent(list *lst)
{
	assert(lst && lst->current && listCheck(lst));
	listnode *next = lst->current->next,
		*prev = lst->current->prev;
	free(lst->current);
	lst->size--;
	if(lst->size == 0) {
		lst->current = NULL;
	}
	else {
		lst->current = next;
		next->prev = prev;
		prev->next = next;
	}
}

void *listGetCurrent(list *lst)
{
	assert(lst && lst->current && listCheck(lst));
	return lst->current->data;
}

int listSize(list *lst)
{
	assert(lst && listCheck(lst));
	return lst->size;
}

void listMoveBack(list *lst)
{
	assert(lst && listCheck(lst));
	if(lst->current)
		lst->current = lst->current->prev;
}

void listMoveForward(list *lst)
{
	assert(lst && listCheck(lst));
	if(lst->current)
		lst->current = lst->current->next;
}

int listCheck(list *lst)
{
	if(!lst->current)
		return true;
	listnode *node = lst->current;
	int listsize = lst->size,
		i;
	for(i = 0; i < listsize; i++, node = node->next) {
		if(node != node->next->prev || node != node->prev->next) {
			return 0;
		}
	}
	return true;
}
