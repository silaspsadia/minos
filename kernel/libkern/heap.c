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
static header_t base;

void kheap_init(void)
{
	wilderness = HEAP_VIRT_ADDR_START;
	alloc_pages((virtual_addr *) wilderness, 2);
	_flist_head = wilderness;
	_flist_head->size = 8192 / sizeof(header_t);
	_flist_head->prev = _flist_head;
	_flist_head->next = _flist_head;
	wilderness += 8192;
	printf("[Mem ] Heap initialized at %x\n", _flist_head);
}

void *kmalloc(size_t nbytes)
{
	header_t *cur, *prev;
	size_t nunits;	

	/* Can't allocate 0 or negative */
	if (nbytes <= 0) 
		return NULL;

	nunits = div_ceil(nbytes + sizeof(header_t), sizeof(header_t));

	for (cur = _flist_head; ; cur = cur->next) {
		if (cur->size >= nunits) {
			if (cur->size == nunits) {
				cur->prev->next = cur->next;
			} else {
				cur->size -= nunits;
				cur += cur->size;
				cur->size = nunits;
			}	
			print_flist_head();	
			return (header_t *) (cur + 1);
		} 
		if (cur == _flist_head) {
			if ((cur = acquire_more_heap(nunits)) == NULL) {
				return NULL;
			}
			cur = cur->prev;
		}
	}
}

void kfree(void *ap)
{
	header_t *bp, *p, *p_next;
	bp = (header_t *) ap - 1;
	for (p = _flist_head, p_next = _flist_head->next; ; p = p_next, p_next = p_next->next)
		if ((p < bp && p_next > bp) || (p->next == _flist_head))
			break;

	bp->prev = p;
	bp->next = p_next;
	p_next->prev = bp;
	p->next = bp;
	
	if (bp + bp->size == p_next) {
		printf("upper boundary merge\n");
		bp->size += p_next->size;
		bp->next = p_next->next;
	} else if (p + p->size == bp) {
		printf("lower boundary merge\n");
		p->next = p_next;
		p_next->prev = p;
	}

	print_flist_head();
}

void *acquire_more_heap(size_t nunits)
{
	printf("[HEAP] ***Acquiring more heap...\n");
	size_t nbytes, npage_frames;		
	header_t *p, *save;
	
	if (nunits < MIN_BLOCK_SIZE) 
		nunits = MIN_BLOCK_SIZE;
	npage_frames = div_ceil(nunits * sizeof(header_t), FOUR_KB);
	nbytes = npage_frames * FOUR_KB;	
	alloc_pages((virtual_addr *) wilderness, npage_frames);

	p = (header_t *) wilderness;
	p->size = nbytes / sizeof(header_t);
	printf("%i\n", p->size);
	p->prev = _flist_head->prev;
	p->next = _flist_head;
	_flist_head->prev->next = p;
	_flist_head->prev = p;
	
	wilderness += nbytes;
	return p;
}

void print_flist_head(void)
{
	header_t *cur = _flist_head;
	do {
		printf("@%x[%n|%x] ", cur, cur->size, cur->next); 
	} while ((cur = cur->next) != _flist_head);
	printf("\n");
}
