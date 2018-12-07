#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <libkern/constants.h>
#include <libkern/memlayout.h>
#include <libkern/phys_mem.h>
#include <libkern/poison.h>
#include <libkern/virt_mem.h>
#include <libkern/heap.h>

#define MIN_BLOCK_SIZE 1024

static header_t *wilderness;
static header_t *_flist_head;

void kheap_init(void)
{
	wilderness = HEAP_VIRT_ADDR_START;
	alloc_pages((virtual_addr *) wilderness, 2);
	_flist_head = wilderness;
	_flist_head->size = 8192 / sizeof(header_t);
	_flist_head->next = _flist_head;
	wilderness += 8192;
	printf("[Mem ] Heap initialized at %x\n", _flist_head);
}

void *kmalloc(size_t nbytes)
{
	printf("[heap] KMALLOC\n");
	header_t *cur, *prev;
	size_t nunits;	

	/* Can't allocate 0 or negative */
	if (nbytes <= 0) 
		return NULL;

	nunits = div_ceil(nbytes + sizeof(header_t), sizeof(header_t));
	prev = _flist_head;

	for (cur = prev->next; ; prev = cur, cur = cur->next) {
		if (cur->size >= nunits) {
			if (cur->size == nunits) {
				prev->next = cur->next;
			} else {
				cur->size -= nunits;
				cur += cur->size;
				cur->size = nunits;
			}	
			print_flist_head(_flist_head);	
			return (void *) (cur + 1);
		} 
		if (cur == _flist_head) {
			if ((cur = acquire_more_heap(nunits)) == NULL)
				return NULL;
		}
	}
}

void kfree(void *ap)
{
	header_t *bp, *p, *p_prev, *start;
	bp = (header_t *) ap - 1;
	for (p_prev = NULL, p = _flist_head; !(bp < p && bp > p->next); p_prev = p, p = p->next)
		if (p_prev == NULL)
			start = _flist_head;
		else 
			if (p == start)
				break;

	if (bp + bp->size == p) {
 		/* merge with upper boundary; original p data left free */
		bp->size += p->size;
		bp->next = p->next;
		p->next = POISON_PAGE_FRAME;
		p_prev->next = bp;
	} else {
		bp->next = p->next; 
		p->next = bp;
	}

	if (bp->next + bp->next->size == bp) {
		/* merge with lower boundary; original bp data left free */
		if (bp->next == start) 
			start->next = start;
		bp->next = POISON_PAGE_FRAME;
		bp->next->size += bp->size;
	}
	print_flist_head(_flist_head);
}

void *acquire_more_heap(size_t nunits)
{
	printf("[HEAP] Acquiring more heap...\n");
	printf("Head at %x\n", _flist_head);
	size_t nbytes, npage_frames;		
	header_t *p, *save;
	
	if (nunits < MIN_BLOCK_SIZE) 
		nunits = MIN_BLOCK_SIZE;
	npage_frames = div_ceil(nunits * sizeof(header_t), FOUR_KB);
	nbytes = npage_frames * FOUR_KB;
	
	alloc_pages((virtual_addr *) wilderness, npage_frames);

	p = (header_t *) wilderness;
	p->size = nbytes / sizeof(header_t);
	p->next = _flist_head->next;

	_flist_head->next = p;
	wilderness += nbytes;
	return p;
}

void print_flist_head(header_t *head)
{
	header_t *cur = head;
	do {
		printf("@%x[%n|%x] ", cur, cur->size, cur->next); 
	} while ((cur = cur->next) != head);
	printf("\n");
}
