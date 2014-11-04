
#ifndef __HEAP_H
#define __HEAP_H

typedef struct heap heap;

/* hpCreate
 *
 * Preconditions: compare is a valid comparison function
 * for the user enforced datatype. The comparison function
 * should return < 0 for lhs < rhs; 0 for lhs == rhs; and > 0 for lhs > rhs
 *
 * Postconditions: A heap which works with the specified comparison function
 * is returned if there are no errors allocating the space; otherwise
 * returns NULL.
 */
struct heap *hpCreate(int (*compare)(const void *lhs, const void *rhs));

/* hpFree
 *
 * Preconditions: hp is a valid heap
 *
 * Postconditions: The memory for the structure is released.
 * The remaining items in the heap are not touched.
 */
void hpFree(struct heap *hp);

/* hpAdd
 *
 * Preconditions: hp is a valid heap, data is a valid dataitem for the
 * comparison function
 *
 * Postconditions: The item is added into the heap
 */
void hpAdd(struct heap *hp, void *data);

/* hpPeek
 *
 * Preconditions: hp is a valid heap
 *
 * Postconditions: The first item in the heap is returned.
 * Returns NULL if there are no items in the heap.
 */
void *hpPeek(struct heap *hp);

/* hpPeek
 *
 * Preconditions: hp is a valid heap
 *
 * Postconditions: The first item in the heap is returned and
 * removed from the heap.
 * Returns NULL if there are no items in the heap.
 */
void *hpTop(struct heap *hp);

/* hpSize
 *
 * Preconditions: hp is a valid heap
 *
 * Postconditions: Returns the number of items in the heap
 */
unsigned hpSize(struct heap *hp);

#endif
