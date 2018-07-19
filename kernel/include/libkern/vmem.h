#ifndef VMEM_H
#define VMEM_H

#include <stdint.h>
#include <stdbool.h>
#include <libkern/paging.h>

#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3FF)
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3FF)
#define PAGE_GET_TABLE_ADDRESS(x) (*x & ~0xFFF)
#define PAGE_GET_PHYSICAL_ADDRESS(x) (*x & ~0xFFF)

extern int enable_paging(uint32_t);

pdir *_cur_directory;

bool alloc_page(vaddr);
void free_page(vaddr);
void map_page(paddr, vaddr);
uint32_t virt_to_phys(vaddr addr);
void vmem_init();
 
pte* ptable_lookup_entry(ptable *, vaddr);
pde* pdir_lookup_entry(pdir *, vaddr);

inline void invlpg(void* m) {
	asm volatile("invlpg (%0)" : : "b"(m) : "memory");
}

inline void flush_tlb_entry(vaddr addr) { 
	invlpg((void*) addr); 
}
#endif /* VMEM_H */

