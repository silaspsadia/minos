#ifndef _LIBKERN_MEMLAYOUT_
#define _LIBKERN_MEMLAYOUT_

typedef uint32_t paddr;
typedef uint32_t vaddr;

#define KERNEL_START_PADDR   ((uint32_t)&kernel_pmem_start)	
#define KERNEL_END_PADDR	 ((uint32_t)&kernel_pmem_end)
#define KERNEL_SIZE			 (KERNEL_END_PADDR - KERNEL_START_PADDR) 
#define PMEM_BLOCKS_PER_BYTE 8
#define PMEM_BLOCK_SIZE      4096
#define PMEM_BLOCK_ALIGN     PMEM_BLOCK_SIZE

#define PAGES_PER_TABLE 1024
#define PAGES_PER_DIR	1024

#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3ff)
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3ff)
#define PAGE_GET_PHYSICAL_ADDRESS(x) (*x & ~0xfff)

#define PTABLE_ADDR_SPACE_SIZE 0x400000
#define DTABLE_ADDR_SPACE_SIZE 0x100000000
#define PAGE_SIZE 4096
 
extern uint32_t kernel_pmem_start;
extern uint32_t kernel_pmem_end;
extern uint32_t kernel_vmem_start;
extern uint32_t kernel_vmem_end;

#endif /* _LIBKERN_MEMLAYOUT_ */
