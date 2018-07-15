#ifndef PMEM_H
#define PMEM_H

#include <stdint.h>

static uint32_t  _pmem_size_kb = 0;
static uint32_t  _pmem_used_blocks = 0;
static uint32_t  _pmem_total_blocks  = 0;
static uint32_t* _pmem_memory_map  = 0;

inline void mmap_set(int);
inline void mmap_unset(int);
inline int mmap_test(int);
int mmap_first_free(void);

uint32_t kernel_phys_map_start;
uint32_t kernel_phys_map_end;

#endif /* PMEM_H */
