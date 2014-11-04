
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
	// Initializes an empty heap
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
	// Frees the memory directly associated with the heap
	free(hp->heap);
	free(hp);
}

void hpAdd(struct heap *hp, void *data)
{
	// Add the item to the heap
	hp->count++;
	if(hp->count >= hp->max) {
		// The heap needs more memory to store this item
		hp->max *= 2;
		hp->heap = realloc(hp->heap, sizeof(void *[hp->max]));
	}
	// Initial position is the end of the heap
	// The data is then moved up the heap until the heap condition is satisfied
	unsigned pos = hp->count - 1;
	bool cont = true;
	while(cont) {
		if(pos == 0) {
			// Nowhere else to go
			hp->heap[0] = data;
			cont = false;
		}
		else {
			// Check if we're going up any further
			if(hp->compare(data, hp->heap[(pos + 1) / 2 - 1]) < 0) {
				// This position satisfies the heap condition
				hp->heap[pos] = data;
				cont = false;
			}
			else {
				// Move up another level
				hp->heap[pos] = hp->heap[(pos + 1) / 2 - 1];
				pos = (pos + 1) / 2 - 1;
			}
		}
	}
}

void *hpPeek(struct heap *hp)
{
	// Verify there's something in the heap
	if(hp->count > 0)
		return hp->heap[0];
	return NULL;
}

void *hpTop(struct heap *hp)
{
	// Verify there's something in the heap
	if(hp->count == 0)
		return NULL;
	hp->count--;
	// Remove the top item
	void *top = hp->heap[0];
	void *last = hp->heap[hp->count];
	hp->heap[hp->count] = NULL;
	unsigned pos = 0;
	int lChild = pos * 2 + 1;
	int rChild = lChild + 1;
	/* Continue moving the bottom item down until it's in a position
	 * which satisfies the heap condition */
	while((lChild < hp->count &&
				 hp->compare(last, hp->heap[lChild]) < 0) ||
	      (rChild < hp->count &&
				 hp->compare(last, hp->heap[rChild]) < 0)) {
		/* At least one of the children is greater than the node
		 * so bring that one up */
		if(rChild < hp->count) {
			if(hp->compare(hp->heap[lChild], hp->heap[rChild]) < 0) {
				// The right child is the larger of the two nodes
				hp->heap[pos] = hp->heap[rChild];
				pos = rChild;
			}
			else {
				// The left child is the larger of the two nodes
				hp->heap[pos] = hp->heap[lChild];
				pos = lChild;
			}
		}
		else {
			// There can only be a left child, and it must be greater
			hp->heap[pos] = hp->heap[lChild];
			pos = lChild;
		}
		lChild = pos * 2 + 1;
		rChild = lChild + 1;
	}
	hp->heap[pos] = last;
	return top;
}

unsigned hpSize(struct heap *hp)
{
	return hp->count;
}
