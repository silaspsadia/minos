#ifndef _LIBK_HEAP_
#define _LIBK_HEAP_

#include <libkern/memlayout.h>
#include <libkern/virt_mem.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MALLOCED_CHECKSUM 0x12345678

#define HEAP_PAGE_ACTUAL_SIZE sizeof(heap_page_t)

typedef struct heap_page_t {
  // If equal to 0x12345678, this is an actual alloced heap page
  size_t checksum;
  // Number of available heap blocks. This doesn't guarantee that
  // this number of blocks are available in sequence.
  size_t num_available_blocks;
  // bitmap: 1 represents an alloc block, 0 a free block
  unsigned char alloced_block_bitmap[HEAP_BLOCK_BIT_MAP_SIZE];
  // bitmap: 1 represents the starting block of an allocation, else 0
  unsigned char first_alloced_bitmap[HEAP_BLOCK_BIT_MAP_SIZE];
  // Actual memory being referenced by the bitmaps
  unsigned char alloc_memory[HEAP_BLOCK_COUNT * HEAP_BLOCK_SIZE];
  // Next heap page in the heap page list
  struct heap_page_t* next;
} heap_page_t;

typedef struct heap_page_list_t {
  heap_page_t* head;
} heap_page_list_t;

heap_page_list_t heap_page_list_;
virtual_addr cur_heap_addr_;

void* kmalloc(size_t size);
void* kcalloc(size_t size);
void kfree(void* ptr);

void kernel_heap_init();
heap_page_t* get_heap_block_metadata(void* ptr);

// These are functions use to compute memory leaks.
// TODO(psamora) Figure out a nice way to restructure this
// so that test maybe inject these as a struct or something

static bool is_tracking_memory_ = false;
static unsigned long memory_tracker_counter_alloc_count_ = 0;
static unsigned long memory_tracker_counter_free_count_ = 0;
static unsigned long memory_tracker_counter_bytes_ = 0;

// Reset previous memory leak run and start tracking memory usage
void track_memory_malloced();

// Stop tracking memory leaks. You can still print_memory_report() after
// this operation for the results until track_memory_malloced() is called again.
void untrack_memory_malloced();

// Print memory status from the current memory analysis (if an analysis is
// currently running) or from the previous run (if no analysis is running).
void print_memory_report(bool verbose);

void increase_memory_tracker(size_t bytes);

void decrease_memory_tracker(size_t bytes);

// Helper used for tests for the Heap that force the heap to have an
// empty Heap Page as the default one used
void force_empty_heap_page();

#endif  // _LIBK_HEAP_
