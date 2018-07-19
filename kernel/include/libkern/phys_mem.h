#ifndef _LIBK_KPHYS_MEM_H_
#define _LIBK_KPHYS_MEM_H_

#include <libkern/memlayout.h>
#include <stdbool.h>
#include <stdint.h>

// Physical memory manager
// Currently implemented using bit map based allocation
static uint32_t* phys_memory_map_ = 0;
static uint32_t phys_mem_size_kb_ = 0;
static uint32_t used_blocks_ = 0;
static total_blocks_ = 0;

uint32_t kernel_phys_map_start;
uint32_t kernel_phys_map_end;

void phys_memory_init(struct multiboot_info* mb);

void update_map_addr(physical_addr);

physical_addr alloc_block();
physical_addr alloc_blocks(uint32_t count);

void free_block(physical_addr);
void free_blocks(physical_addr, uint32_t count);

bool is_alloced(physical_addr);

#endif  // _LIBK_KPHYS_MEM_H_
