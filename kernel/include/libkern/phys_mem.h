#ifndef LIBKERN_PHYS_MEM_H
#define LIBKERN_PHYS_MEM_H

#include <libkern/memlayout.h>
#include <stdbool.h>
#include <stdint.h>

// Physical memory manager
// Currently implemented using bit map based allocation
extern uint32_t* phys_memory_map_;
extern uint32_t phys_mem_size_kb_;
extern uint32_t used_blocks_;
extern uint32_t total_blocks_;

uint32_t kernel_phys_map_start;
uint32_t kernel_phys_map_end;

void phys_memory_init(struct multiboot_info* mb);

void update_map_addr(physical_addr);

physical_addr alloc_block(void);
physical_addr alloc_blocks(uint32_t count);

void free_block(physical_addr);
void free_blocks(physical_addr, uint32_t count);

bool is_alloced(physical_addr);

#endif  /* LIBKERN_PHYS_MEM_H */
