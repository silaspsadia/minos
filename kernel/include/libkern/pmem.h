#ifndef PMEM_H
#define PMEM_H

#include <stdint.h>

#define PMEM_BLOCKS_PER_BYTE 8
#define PMEM_BLOCK_SIZE		 4096
#define PMEM_BLOCK_ALIGN	 PMEM_BLOCK_SIZE

static uint32_t  _pmem_memory_size = 0;
static uint32_t  _pmem_used_blocks = 0;
static uint32_t  _pmem_max_blocks  = 0;
static uint32_t* _pmem_memory_map  = 0;

inline void mmap_set(int);
inline void mmap_unset(int);
inline int mmap_test(int);
int mmap_first_free(void);

#endif /* PMEM_H */
