#ifndef _LIBK_HEAP_
#define _LIBK_HEAP_

#include <libkern/memlayout.h>
#include <libkern/virt_mem.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct header {
	struct header *next; // pointer to next header
	size_t size; // Size in header units, sizeof(header_t)
} header_t;

void kheap_init(void);
void *kmalloc(size_t);
void kfree(void *);
void *acquire_more_heap(size_t);
#endif  // _LIBK_HEAP_
