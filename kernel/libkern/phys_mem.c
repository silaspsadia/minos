#include <stdio.h>
#include <string.h>

#include <extern/multiboot.h>
#include <libkern/phys_mem.h>

// Functions to manipulate the bitmap

inline static void map_set(int bit)
{
	phys_memory_map_[bit / 32] |= (1 << (bit % 32));
}

inline static void map_unset(int bit)
{
	phys_memory_map_[bit / 32] &= ~(1 << (bit % 32));
}

inline static bool map_test(int bit)
{
	return phys_memory_map_[bit / 32] & (1 << (bit % 32));
}

int find_free_block(void)
{
	for (uint32_t i = 0; i < total_blocks_; i++) {
		uint32_t block = phys_memory_map_[i];
		if (block != 0xFFFFFFFF) {
			for (uint8_t j = 0; j < 32; j++) {
				int bit = 1 << j;
				if (!(bit & block)) {
					return (32 * i) + j;
				}
			}
		}
	}
	return -1;
}

int find_free_blocks(uint32_t count)
{
	int starting_block = -1;
	int starting_block_bit = -1;
	uint32_t cur_block_num = 0;
	for (uint32_t i = 0; i < total_blocks_; i++) {
		uint32_t cur_block = phys_memory_map_[i];
		if (cur_block == 0xFFFFFFFF) {
			cur_block_num = 0;
			continue;
		}

		for (uint8_t j = 0; j < 32; j++) {
			int bit = 1 << j;
			if (bit & cur_block) {  // bit is set
				cur_block_num = 0;
				continue;
			}

			if (!cur_block_num) starting_block = i;
			if (!cur_block_num) starting_block_bit = j;
			cur_block_num += 1;
			if (cur_block_num == count) {
				return (32 * starting_block) + starting_block_bit;
			}
		}
	}
	return -1;
}

// Functions to manage a single block in memory
physical_addr alloc_block(void)
{
	if (total_blocks_ - used_blocks_ <= 0) 
		return 0;

	int free_block = find_free_block();
	if (free_block == -1) 
		return 0;

	map_set(free_block);
	uint32_t addr = free_block * PHYS_BLOCK_SIZE;
	used_blocks_++;
	return addr;
}

void free_block(physical_addr addr)
{
	int block = addr / PHYS_BLOCK_SIZE;

	map_unset(block);
	used_blocks_--;
}

bool is_alloced(physical_addr addr)
{
	int block = addr / PHYS_BLOCK_SIZE;
	return map_test(block);
}

// Functions to allocate multiple blocks of memory

physical_addr alloc_blocks(uint32_t count)
{
	if (total_blocks_ - used_blocks_ <= 0) {
		return 0;
	}	

	int free_block = find_free_blocks(count);
	if (free_block == -1) {
		return 0;
	}

	for (uint32_t i = 0; i < count; i++) {
		map_set(free_block + i);
	}	

	uint32_t addr = free_block * PHYS_BLOCK_SIZE;
	used_blocks_ += count;
	return addr;
}

void free_blocks(physical_addr addr, uint32_t count)
{
	int block = addr / PHYS_BLOCK_SIZE;

	for (uint32_t i = 0; i < count; i++) map_unset(block + i);

	used_blocks_ -= count;
}

// Internal functions to allocate ranges of memory

void allocate_chunk(int base_addr, int length)
{
	int cur_block_addr = base_addr / PHYS_BLOCK_SIZE;
	int num_blocks = length / PHYS_BLOCK_SIZE;
	while (num_blocks-- >= 0) {
		map_set(cur_block_addr++);
		used_blocks_--;
	}
}

void free_chunk(int base_addr, int length) 
{
	int cur_block_addr = base_addr / PHYS_BLOCK_SIZE;
	int num_blocks = length / PHYS_BLOCK_SIZE;

	while (num_blocks--) {
		map_unset(cur_block_addr++);
		used_blocks_--;
	}
}

// Functions to initialize the Physical Memory Manager

void free_available_memory(struct multiboot_info* mb)
{
	multiboot_memory_map_t* mm = (multiboot_memory_map_t*)mb->mmap_addr;
	while ((unsigned int)mm < mb->mmap_addr + mb->mmap_length) {
		if (mm->type == MULTIBOOT_MEMORY_AVAILABLE) {
			free_chunk(mm->addr, mm->len);
		}
		mm = (multiboot_memory_map_t*)((unsigned int)mm + mm->size + sizeof(mm->size));
	}
	map_set(0);
}

void phys_memory_init(struct multiboot_info* mb)
{
	phys_mem_size_kb_ = mb->mem_upper + mb->mem_lower;
	total_blocks_ = (phys_mem_size_kb_ * 1024) / PHYS_BLOCK_SIZE;
	used_blocks_ = total_blocks_;
	phys_memory_map_ = (uint32_t*)KERNEL_END_PADDR;
	memset(phys_memory_map_, 0xFF, total_blocks_ / PHYS_BLOCKS_PER_BYTE);
	printf("[Mem ] PMEM set up. Total blocks: %ld\n", total_blocks_);
	printf("[Mem ] Kernel size: %ld B\n", KERNEL_SIZE);

	// Frees memory GRUB considers available
	free_available_memory(mb);

	// From the freed memory, we need to allocate the ones used by the Kernel
	allocate_chunk(KERNEL_START_PADDR, KERNEL_SIZE);

	// We also need to allocate the memory used by the Physical Map itself
	allocate_chunk(*phys_memory_map_, total_blocks_);
	kernel_phys_map_start = (uint32_t)phys_memory_map_;
	kernel_phys_map_end = kernel_phys_map_start + (total_blocks_ / PHYS_BLOCKS_PER_BYTE);
	printf("[Mem ] PMEM Manager set up at [%lx, %lx]\n",
		kernel_phys_map_start, kernel_phys_map_end);
}

void update_map_addr(physical_addr addr)
{ 
	phys_memory_map_ = (uint32_t*)addr;
}
