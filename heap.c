
#include "heap.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

struct heap
{
	void **heap;
	int (*compare)(const void *, const void *);
	unsigned count, max;
};

struct heap *hpCreate(int (*compare)(const void *, const void *))
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
		hp->heap = realloc(hp->heap, sizeof(void *[hp->max]));
	}
	unsigned pos = hp->count - 1;
	bool cont = true;
	while(cont) {
		if(pos == 0) {
			hp->heap[0] = data;
			break;
		}
		if(hp->compare(data, hp->heap[(pos + 1) / 2 - 1]) < 0) {
			hp->heap[pos] = data;
			cont = false;
		}
		else {
			hp->heap[pos] = hp->heap[(pos + 1) / 2 - 1];
			pos = (pos + 1) / 2 - 1;
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
	unsigned pos = 0;
	int lChild = pos * 2 + 1;
	int rChild = lChild + 1;
	while((lChild < hp->count &&
				 hp->compare(hp->heap[pos], hp->heap[lChild]) < 0) ||
	      (rChild < hp->count &&
				 hp->compare(hp->heap[pos], hp->heap[rChild]) < 0)) {
		// At least one of the children is greater than the node, so bring the one up
		if(rChild < hp->count) {
			if(hp->compare(hp->heap[lChild], hp->heap[rChild]) < 0) {
				// The right child is the larger of the two nodes
				void *buffer = hp->heap[rChild];
				hp->heap[rChild] = hp->heap[pos];
				hp->heap[pos] = buffer;
				pos = rChild;
			}
			else {
				// The left child is the larger of the two nodes
				void *buffer = hp->heap[lChild];
				hp->heap[lChild] = hp->heap[pos];
				hp->heap[pos] = buffer;
				pos = lChild;
			}
		}
		else {
			// There can only be a left child, and it must be greater
			void *buffer = hp->heap[lChild];
			hp->heap[lChild] = hp->heap[pos];
			hp->heap[pos] = buffer;
			pos = lChild;
		}
		lChild = pos * 2 + 1;
		rChild = lChild + 1;
	}
	return top;
}

unsigned hpSize(struct heap *hp)
{
	return hp->count;
}
