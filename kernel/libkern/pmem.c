#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <libkern/pmem.h>
#include <libkern/memlayout.h>
#include <extern/multiboot.h>

static inline void mmap_set(int bit) {
	_pmem_memory_map[bit/32] |= (1 << (bit % 32));
}

static inline void mmap_unset(int bit) {
	_pmem_memory_map[bit/32] &= ~(1 << (bit % 32));
}
	
static inline int mmap_test(int bit) {
	return _pmem_memory_map[bit/32] & (1 << (bit % 32));
}	

int mmap_first_free() {
	for (int i = 0; i < (int) _pmem_total_blocks; i++) {
		uint32_t block = _pmem_memory_map[i];
		if (block != 0xFFFFFFFF) {
			for (int j = 0; j < 32; j++) {
				int bit = 1 << j;
				if (!(bit & block))
					return (32 * i) + j;
			}
		}
	}
	return -1;	
}

void alloc_chunk(uint32_t base_addr, int len) {
	int cur_block = base_addr / PMEM_BLOCK_SIZE;
	int num_blocks = len / PMEM_BLOCK_SIZE;
	while (num_blocks-- >= 0) {
		mmap_set(cur_block++);
		_pmem_used_blocks--;
	}
}

void free_chunk(uint32_t base_addr, int len) {
	int cur_block = base_addr / PMEM_BLOCK_SIZE;
	int num_blocks = len / PMEM_BLOCK_SIZE;
	while (num_blocks-- >= 0) {
		mmap_unset(cur_block++);
		_pmem_used_blocks--;
	}
}

void free_available_memory(struct multiboot_info* mb) {
	multiboot_memory_map_t* mm = (multiboot_memory_map_t*)mb->mmap_addr;
	while ((unsigned int)mm < mb->mmap_addr + mb->mmap_length) {
		if (mm->type == MULTIBOOT_MEMORY_AVAILABLE) {
			free_chunk(mm->addr, mm->len);
		}
		mm = (multiboot_memory_map_t*)((unsigned int)mm + mm->size + sizeof(mm->size));
	}
	mmap_set(0);
}

paddr alloc_block() {
	if (_pmem_total_blocks - _pmem_used_blocks <= 0) 
		return 0;

	int free_block = mmap_first_free();
	if (free_block == -1) 
		return 0;
	
	mmap_set(free_block);
	uint32_t addr = free_block * PMEM_BLOCK_SIZE;
	_pmem_used_blocks++;
	return addr;
}

void free_block(paddr addr) {
	int block = addr / PMEM_BLOCK_SIZE;
	mmap_unset(block);
	_pmem_used_blocks--;
}

void pmem_init(struct multiboot_info *mb) {
	_pmem_size_kb = mb->mem_upper + mb->mem_lower; 
	_pmem_total_blocks = (_pmem_size_kb * 1024) / PMEM_BLOCK_SIZE;
	_pmem_used_blocks = _pmem_total_blocks;
	_pmem_memory_map = (uint32_t *)KERNEL_END_PADDR;	

	printf("[Mem ] Defaulting pmem blocks to 'used'.\n");	
	/* By default, all memory is in use */
	memset(_pmem_memory_map, 0xFF, _pmem_total_blocks / PMEM_BLOCKS_PER_BYTE);

	/* Free the memory that was just made available to us */
	free_available_memory(mb);
	printf("[Mem ] Freeing available memory successful.\n");
	
	/* Allocate memory for use by the kernel */
	alloc_chunk(KERNEL_START_PADDR, KERNEL_SIZE*KERNEL_SIZE);
	printf("[Mem ] Allocating kernel memory successful. Total blocks: %i.\n", _pmem_total_blocks);	

	kernel_phys_map_start = (uint32_t)_pmem_memory_map;
	kernel_phys_map_end = kernel_phys_map_start + (_pmem_total_blocks / PMEM_BLOCKS_PER_BYTE);
	alloc_chunk(KERNEL_START_PADDR, KERNEL_SIZE);
	printf("[Mem ] PMEM manager installed successfully.\n");
	printf("[Mem ] PMEM initialized with %i KB using address range [%x, %x].\n", _pmem_size_kb, kernel_phys_map_start, kernel_phys_map_end);
}
