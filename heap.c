
#include "heap.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct heap
{
	void **heap;
	int (*compare)(void *, void *);
	unsigned count, max;
};

struct heap *hpCreate(int (*compare)(void *, void *))
{
	struct heap *hp = malloc(sizeof(struct heap));
	hp->compare = compare;
	int DEFAULTMAX = 64;
	hp->heap = malloc(sizeof(void *[DEFAULTMAX]));
	hp->max = DEFAULTMAX;
	hp->count = 0;
	return hp;
}

void hpFree(struct heap *hp)
{
	free(hp->heap);
	free(hp);
}

void hpAdd(struct heap *hp, void *data)
{
	hp->count++;
	if(hp->count >= hp->max) {
		hp->max *= 2;
		void **buffer = malloc(sizeof(void *[hp->max]));
		unsigned i;
		for(i = 0; i < hp->count - 1; i++)
			buffer[i] = hp->heap[i];
		free(hp->heap);
		hp->heap = buffer;
	}
	if(hp->count == 1) {
		hp->heap[0] = data;
	}
	else {
		unsigned pos = hp->count - 1;
		bool cont = true;
		while(cont) {
			if(pos == 0) {
				hp->heap[0] = data;
				break;
			}
			if(hp->compare(data, hp->heap[pos / 2]) < 0) {
				hp->heap[pos] = data;
				cont = false;
			}
			else {
				hp->heap[pos] = hp->heap[pos / 2];
				pos /= 2;
			}
		}
	}
}

void *hpPeek(struct heap *hp)
{
	if(hp->count > 0)
		return hp->heap[0];
	return NULL;
}

void *hpTop(struct heap *hp)
{
	if(hp->count == 0)
		return NULL;
	hp->count--;
	void *top = hp->heap[0];
	hp->heap[0] = hp->heap[hp->count];
	hp->heap[hp->count] = NULL;
	unsigned pos = 1;
	while((pos * 2 < hp->count &&
				 hp->compare(hp->heap[pos - 1], hp->heap[pos * 2]) < 0) ||
	      (pos*2 - 1 < hp->count &&
				 hp->compare(hp->heap[pos - 1], hp->heap[pos * 2 - 1]) < 0)) {
		void *buffer;
		if(pos * 2 >= hp->count ||
			 hp->compare(hp->heap[pos * 2 - 1], hp->heap[pos * 2]) > 0) {
			buffer = hp->heap[pos - 1];
			hp->heap[pos - 1] = hp->heap[pos * 2 - 1];
			hp->heap[pos * 2 - 1] = buffer;
			pos = pos * 2 - 1;
		}
		else {
			buffer = hp->heap[pos - 1];
			hp->heap[pos - 1] = hp->heap[pos * 2];
			hp->heap[pos * 2] = buffer;
			pos = pos * 2;
		}
	}
	return top;
}

unsigned hpSize(struct heap *hp)
{
	return hp->count;
}
