#ifndef PMEM_H
#define PMEM_H

#include <stdint.h>
#include <libkern/memlayout.h>
#include <extern/multiboot.h>

static uint32_t* _pmem_memory_map = 0;
static uint32_t  _pmem_size_kb = 0;
static uint32_t  _pmem_used_blocks = 0;
static uint32_t  _pmem_total_blocks = 0;

uint32_t kernel_phys_map_start;
uint32_t kernel_phys_map_end;

paddr alloc_block(void);
paddr alloc_blocks(uint32_t count); 
void free_block(paddr);
int find_free_blocks(uint32_t);

void pmem_init(struct multiboot_info *);
void update_map_addr(uint32_t *);

#endif /* PMEM_H */
