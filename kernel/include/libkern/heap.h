#ifndef LIBKERN_HEAP_H
#define LIBKERN_HEAP_H

#include <libkern/memlayout.h>
#include <libkern/virt_mem.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct header {
	struct header *prev; // pointer to prev header
	struct header *next; // pointer to next header
	size_t size; // Size in header units, sizeof(header_t)
} header_t;

void kheap_init(void);
void *kmalloc(size_t);
void kfree(void *);
void *acquire_more_heap(size_t);
void __kheap_reset(void);
void print_flist(header_t*);
void *get_flist_head(void);

#endif  // LIBKERN_HEAP_H
