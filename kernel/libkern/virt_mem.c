#include <libkern/paging.h>
#include <libkern/phys_mem.h>
#include <libkern/virt_mem.h>
#include <stdio.h>
#include <string.h>

bool alloc_page(virtual_addr vaddr) {
	physical_addr paddr = alloc_block();
	if (!paddr) 
		return false;
	map_page(paddr, vaddr);
	return true;
}

void free_page(virtual_addr addr) {
	pd_entry* pd_entry = pdirectory_lookup_entry(cur_directory, addr);
	if (!pd_entry) 
		return;

	page_table* table = (page_table*)PAGE_GET_PHYSICAL_ADDRESS(pd_entry);
	pt_entry* pt_entry = ptable_lookup_entry(table, addr);
	if (!pt_entry) 
		return;

	physical_addr block = pt_entry_frame(*pt_entry);
	if (block) 
		free_block(block);

	pt_entry_del_attrib(pt_entry, I86_PTE_PRESENT);
}

void map_page(physical_addr paddr, virtual_addr vaddr) {
	pd_entry* entry = pdirectory_lookup_entry(cur_directory, vaddr);
	if (!pd_entry_is_present(*entry)) {
		// Page Directory Entry not present, allocate it
		physical_addr table = alloc_block();
		if (!table) 
			return;

		// Temporarily maps the new Page Table to the temporary table addr
		map_page(table, (virtual_addr)TEMPORARY_TABLE_ADDR);
		invlpg(TEMPORARY_TABLE_ADDR);

		// Clear the newly allocated page
		memset(TEMPORARY_TABLE_ADDR, 0, sizeof(page_table));

		// Maps the Page Directory Entry to the new table
		pd_entry_add_attrib(entry, I86_PDE_PRESENT);
		pd_entry_add_attrib(entry, I86_PDE_WRITABLE);
		pd_entry_set_frame(entry, table);
	}

	// Get table address from entry, guaranteed to be set now
	page_table* table = (page_table*)PAGE_GET_TABLE_ADDRESS(entry);

	// Get page table entry
	pt_entry* page = ptable_lookup_entry(table, vaddr);

	// Maps the Page Table Entry to the given physical address
	pt_entry_set_frame(page, paddr);
	pt_entry_add_attrib(page, I86_PTE_PRESENT);
	pt_entry_add_attrib(page, I86_PTE_WRITABLE);
	flush_tlb_entry(vaddr);
}

uint32_t virt_to_phys(virtual_addr addr) {
	pd_entry* pd_entry = pdirectory_lookup_entry(cur_directory, addr);
	if (!pd_entry) 
		return -1;
	page_table* table = (page_table*)PAGE_GET_TABLE_ADDRESS(pd_entry);
	pt_entry* pt_entry = ptable_lookup_entry(table, addr);
	return PAGE_GET_PHYSICAL_ADDRESS(pt_entry);
}

void virt_memory_init() {
	// Allocates first MB page table
	page_table* table = (page_table*)alloc_block();
	if (!table) 
		return;

	// Allocates kernel page table
	page_table* table2 = (page_table*)alloc_block();
	if (!table2) 
		return;

	// Clear allocated page tables
	memset(table, 0, sizeof(page_table));
	memset(table2, 0, sizeof(page_table));

	// Maps first MB to 3GB
	for (int frame = 0x0, virt = 0xC0000000; 
			frame < 0x100000;
			frame += 4096, virt += 4096) {
		pt_entry page = 0;
		pt_entry_add_attrib(&page, I86_PTE_PRESENT);
		pt_entry_set_frame(&page, frame);
		table->m_entries[PAGE_TABLE_INDEX(virt)] = page;
  }

	// Maps kernel pages and phys mem pages
	for (uint32_t frame = KERNEL_START_PADDR, virt = KERNEL_START_VADDR;
			frame < KERNEL_PHYS_MAP_END; 
			frame += 4096, virt += 4096) {
		pt_entry page = 0;
		pt_entry_add_attrib(&page, I86_PTE_PRESENT);
		pt_entry_set_frame(&page, frame);
		table2->m_entries[PAGE_TABLE_INDEX(virt)] = page;
	}

	// Create default directory table
	cur_directory = (page_directory*)alloc_blocks(3);
	printf("%i\n", (int) cur_directory);
	if (!cur_directory) 
		return;

	memset(cur_directory, 0, sizeof(page_directory));

	pd_entry* entry = pdirectory_lookup_entry(cur_directory, 0x00000000);
	pd_entry_add_attrib(entry, I86_PDE_PRESENT);
	pd_entry_add_attrib(entry, I86_PDE_WRITABLE);
	pd_entry_set_frame(entry, (physical_addr)table);

	pd_entry* entry2 = pdirectory_lookup_entry(cur_directory, 0xC0100000);
	pd_entry_add_attrib(entry2, I86_PDE_PRESENT);
	pd_entry_add_attrib(entry2, I86_PDE_WRITABLE);
	pd_entry_set_frame(entry2, (physical_addr)table2);
	
	enable_paging((uint32_t)cur_directory);

	// Updates the Phys Mem table to its new virtual address
	update_map_addr(KERNEL_END_VADDR);
	printf("Paging installed.\n");
}
