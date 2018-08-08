#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <libkern/constants.h>
#include <libkern/memlayout.h>
#include <libkern/phys_mem.h>
#include <libkern/virt_mem.h>
#include <libkern/heap.h>

#define MIN_BLOCK_SIZE 1024

static header_t base;
static header_t *cur_loc;
static header_t *_flist;

void kheap_init(void) {
	cur_loc = HEAP_VIRT_ADDR_START;
	_flist = NULL;
	printf("[Mem ] Heap initialized at %x\n", cur_loc);
}

void *kmalloc(size_t nbytes) {
	header_t *cur, *prev;
	size_t nunits;	

	/* Can't allocate 0 or negative */
	if (nbytes <= 0) 
		return NULL;

	/* If no free list yet, init */
	if (_flist == NULL) {
		base.next = _flist = &base;
		base.size = 0;
	}
	
	nunits = div_ceil(nbytes + sizeof(header_t), sizeof(header_t));
	prev = _flist;

	for (cur = prev->next; ; prev = cur, cur = cur->next) {
		if (cur->size >= nunits) {
			if (cur->size == nunits) {
				prev->next = cur->next;
			} else {
				cur += cur->size - nunits;
				cur->size = nunits;
			}
			_flist = prev;
			return (void *) (cur + 1);
		} 
		if (cur == _flist) {
			if ((cur = acquire_more_heap(nunits)) == NULL)
				return NULL;
			prev->next = cur;
			cur == prev;
		}
	}
	
}

void kfree(void *p) {
	return NULL;
}

void *acquire_more_heap(size_t nunits) {
	size_t nbytes, npage_frames;		
	header_t *p;

	if (nunits < MIN_BLOCK_SIZE) 
		nunits = MIN_BLOCK_SIZE;

	npage_frames = div_ceil(nunits * sizeof(header_t), FOUR_KB);
	nbytes = npage_frames * FOUR_KB;
	printf("%n\n", npage_frames);
	printf("%n\n", nbytes);
	alloc_pages((virtual_addr *) cur_loc, npage_frames);
	p = (header_t *) cur_loc;
	p->size = nbytes / sizeof(header_t);
	p->next = _flist;
	_flist = p;
	return _flist;
}



