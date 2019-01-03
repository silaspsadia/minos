#ifndef LIBKERN_VIRT_MEM_H
#define LIBKERN_VIRT_MEM_H

#include <asm.h>
#include <libkern/memlayout.h>
#include <libkern/paging.h>
#include <stdbool.h>
#include <stdint.h>

#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3FF)
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3FF)
#define PAGE_GET_TABLE_ADDRESS(x) (*x & ~0xFFF)
#define PAGE_GET_PHYSICAL_ADDRESS(x) (*x & ~0xFFF)

extern enable_paging(uint32_t page_dir);

// Page Directory holds 1024 page directory entries
typedef struct page_directory {
	pd_entry m_entries[PAGES_PER_DIR];
} page_directory;

// Page Table holds 1024 page table entries
typedef struct page_table {
	pt_entry m_entries[PAGES_PER_TABLE];
} page_table;

page_directory* cur_directory;

inline pt_entry* ptable_lookup_entry(page_table* table, virtual_addr addr)
{
	if (table)
		return &table->m_entries[PAGE_TABLE_INDEX(addr)];
	return 0;
}

inline pd_entry* pdirectory_lookup_entry(page_directory* directory,
	virtual_addr addr)
{
	if (directory)
		return &directory->m_entries[PAGE_DIRECTORY_INDEX(addr)];
	return 0;
}

bool alloc_page(virtual_addr addr);
void free_page(virtual_addr addr);
void map_page(physical_addr, virtual_addr);
uint32_t virt_to_phys(virtual_addr addr);

void virt_memory_init();

inline void flush_tlb_entry(virtual_addr addr)
{
	invlpg((void*)addr);
}

#endif  // LIBKERN_VIRT_MEM_H

