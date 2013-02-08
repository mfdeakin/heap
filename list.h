
#ifndef _LIST_H_
#define _LIST_H_

typedef struct list list;

list *listCreate();
void listFree(list *lst);

void listInsert(list *lst, void *data);
void listDeleteCurrent(list *lst);
void *listGetCurrent(list *lst);

int listSize(list *lst);

void listMoveBack(list *lst);
void listMoveForward(list *lst);

#endif
