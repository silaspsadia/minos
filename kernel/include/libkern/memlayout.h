#ifndef _LIBKERN_MEMLAYOUT_
#define _LIBKERN_MEMLAYOUT_

#define KERNEL_START_PADDR   ((uint32_t)&kernel_pmem_start)	
#define KERNEL_END_PADDR	 ((uint32_t)&kernel_pmem_end)
#define KERNEL_SIZE			 (KERNEL_END_PADDR - KERNEL_START_PADDR) 
#define PMEM_BLOCKS_PER_BYTE 8
#define PMEM_BLOCK_SIZE      4096
#define PMEM_BLOCK_ALIGN     PMEM_BLOCK_SIZE

extern uint32_t kernel_pmem_start;
extern uint32_t kernel_pmem_end;
extern uint32_t kernel_vmem_start;
extern uint32_t kernel_vmem_end;

#endif /* _LIBKERN_MEMLAYOUT_ */
