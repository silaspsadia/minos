#ifndef PMEM_H
#define PMEM_H

#include <stdint.h>
#include <extern/multiboot.h>

uint32_t  _pmem_size_kb;
uint32_t  _pmem_used_blocks;
uint32_t  _pmem_total_blocks;
uint32_t* _pmem_memory_map;

uint32_t kernel_phys_map_start;
uint32_t kernel_phys_map_end;

void pmem_init(struct multiboot_info *);

#endif /* PMEM_H */
