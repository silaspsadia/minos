#ifndef _LIBK_MEMLAYOUT_
#define _LIBK_MEMLAYOUT_

#include <stdint.h>

// Constants to the boundaries of memory
#define KERNEL_START_PADDR ((uint32_t)&kernel_phys_start)
#define KERNEL_END_PADDR ((uint32_t)&kernel_phys_end)
#define KERNEL_SIZE (KERNEL_END_PADDR - KERNEL_START_PADDR)

#define KERNEL_START_VADDR ((uint32_t)&kernel_virt_start)
#define KERNEL_END_VADDR ((uint32_t)&kernel_virt_end)

#define KERNEL_PHYS_MAP_START kernel_phys_map_start
#define KERNEL_PHYS_MAP_END kernel_phys_map_end
#define KERNEL_PHYS_MAP_SIZE kernel_phys_map_end - kernel_phys_map_start;

// Constants to the Physical Memory Manager
#define PHYS_BLOCKS_PER_BYTE 8
#define PHYS_BLOCK_SIZE 4096

// Constants to the Virtual Memory Manager
#define TEMPORARY_TABLE_ADDR (void*)0xC03FF000
#define PAGES_PER_TABLE 1024
#define PAGES_PER_DIR 1024
#define PAGE_SIZE 4096

// Constants to the Kernel heap
#define HEAP_VIRT_ADDR_START 0xC0500000  // if kernel size > 4MB, change
#define HEAP_INITIAL_BLOCK_SIZE 128

#define HEAP_BLOCK_SIZE 16          // bytes
#define HEAP_BLOCK_BIT_MAP_SIZE 31  // 31 bytes can represent 248 blocks
#define HEAP_BLOCK_COUNT 248  // amount of blocks we can fit beside the bitmap
#define HEAP_BLOCKS_NEED_FOR_N_BYTES(n)   \
	(n / HEAP_BLOCK_SIZE) + (n % HEAP_BLOCK_SIZE == 0 ? 0 : 1)

// Functions to
#define ALIGN_BLOCK(addr) (addr) - ((addr) % PHYS_BLOCK_SIZE);

// Addresses of the Kernel in the Physical and Virtual memory
extern uint32_t kernel_phys_start;
extern uint32_t kernel_phys_end;
extern uint32_t kernel_virt_start;
extern uint32_t kernel_virt_end;

// Addresses of the Physical Memory Map in the Physical memory
extern uint32_t kernel_phys_map_start;
extern uint32_t kernel_phys_map_end;

typedef uint32_t virtual_addr;
typedef uint32_t physical_addr;

#endif  // _LIBK_MEMLAYOUT_