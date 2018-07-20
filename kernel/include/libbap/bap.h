/* **********************************************************************
 * ### "Box and Pointer" Base Data Type 
 * 
 * - The most non-trivial trivial data structure.
 * - Inspired by Lisp implementations.
 * - Intended for simplifying interfaces and implementations
 * - Cache-friendly 
 * - Algorithm-friendly
 *
 ************************************************************************/


#ifndef LIBBAP_H
#define LIBBAP_H

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <libkern/heap.h>

// "Classic flavor" bap

typedef struct bap {
	int first;
	int second;
} bap_t;

bap_t *bap_init(int entry) {
	bap_t *ret = kmalloc(sizeof(bap_t));
	ret->first = entry;
	ret->second = (int) NULL;
	return ret;
}

// SLList Structures

typedef struct sll {
	bap_t metadata;
	bap_t head;
} sll_t;

sll_t *sll_init() {
	sll_t *ret = kmalloc(sizeof(sll_t));
	memset(ret, 0, sizeof(sll_t));
	return ret;	
}

int sll_push_back(sll_t *list, int entry) {
	// Allocate a bap
	bap_t *to = bap_init(entry);

	// Null case: insert at the head bap's second field
	if ((list->metadata).first == 0) {
		(list->head).second = to;
	} else {
	// Iterative case: insert new bap where NULL
		bap_t *cur = (bap_t *) (list->head).second;
		while (cur->second != (int) NULL) { 
			cur = (bap_t *) cur->second;
		}
		cur->second = (int) to; 	
	}
	// Increment the first field of the metadata bap (length)
	(list->metadata).first++;
	return 0;
}

int sll_push_front(sll_t *list, int entry) {
	// Allocate a bap
	bap_t *to = bap_init(entry);
	
	// Null case: insert at the head bap's second field
	if ((list->metadata).first == 0) {
		(list->head).second = to;
	} else {
	// Simply insert at front 
		bap_t *cur = (bap_t *) (list->head).second;
		(list->head).second = (int) to;
		to->second = (int) cur;
	}
	// Increment the first field of the metadata bap (length)
	(list->metadata).first++;
	return 0;
}

int sll_remove(sll_t *list, int index) {
	if ((list->metadata).first <= index)
		return -1;
	bap_t *cur = &(list->head);
	for (int loc = 0; loc < index; loc++) 
		cur = (bap_t *) cur->second;
	int save = cur->second;
	cur->second = ((bap_t *) cur->second)->second;
	kfree(save);
	list->metadata.first--;
	return 0;
}

int sll_swap(sll_t *list, int index1, int index2) {
	int length = (list->metadata).first;
	if (index1 == index2) 
		return 0;
	if (length <= index1 || length <= index2) 
		return -1;
	bap_t *cur = &(list->head);

	for (int loc = 0; loc < index1; loc++)
		cur = (bap_t *) cur->second;
	bap_t *entry1_prev = cur;
	bap_t *entry1 = (bap_t *) entry1_prev->second;
	bap_t *entry1_front = ((bap_t *) entry1_prev->second)->second;

	cur = &(list->head);

	for (int loc = 0; loc < index2; loc++)
		cur = (bap_t *) cur->second;
	bap_t *entry2_prev = cur;
	bap_t *entry2 = (bap_t *) entry2_prev->second;
	bap_t *entry2_front = ((bap_t *) (entry2_prev->second))->second;

	if (entry1_prev == entry2) {
		entry2_prev->second = entry1;
		entry1_prev->second = entry1_front;
		entry1->second = entry2;
	} else if (entry2_prev == entry1) { 
		entry1_prev->second = entry2;
		entry2_prev->second = entry2_front;
		entry2->second = entry1;
	} else {
		entry1_prev->second = entry2;
		entry2_prev->second = entry1;
		entry1->second = entry2_front;
		entry2->second = entry1_front;
	}
}

void sll_print(sll_t *list) {
	// Start at the address of the first bap of the sll's contents
	bap_t *cur = (bap_t *) (list->head).second;
	// Null case: don't print anything
	if ((list->metadata).first == 0) { 
		printf("<EMPTY>\n");
		return;
	}
	// Iterative case: print the bap address and its contents until NULL
	while (cur != (bap_t *) NULL) {
		printf("[%u] %u \n", (int) cur, ((bap_t *) cur)->first);
		cur = (bap_t *) cur->second;
	}
	printf("\n");
}

int sll_get_length(sll_t *list) {
	return (list->metadata).first;
}

#endif /* LIBBAP_H */
