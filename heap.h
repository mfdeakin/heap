
#ifndef __HEAP_H
#define __HEAP_H

typedef struct heap heap;

struct heap *hpCreate(int (*compare)(const void *lhs, const void *rhs));
void hpFree(struct heap *hp);
void hpAdd(struct heap *hp, void *data);
void *hpPeek(struct heap *hp);
void *hpTop(struct heap *hp);
unsigned hpSize(struct heap *hp);

#endif
