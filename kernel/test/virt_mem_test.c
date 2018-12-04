#include <libkern/constants.h>
#include <libkern/phys_mem.h>
#include <libkern/virt_mem.h>
#include <libkern/memlayout.h>
#include <test/virt_mem_test.h>
#include <test/unit.h>

#define TEST_ADDR 0xC10000000

NEW_TEST_SUITE(virtmemTest, 2);

TEST(AllocPage)
{
	alloc_page(TEST_ADDR);
	pd_entry *pde = pdirectory_lookup_entry(cur_directory, TEST_ADDR);
	EXPECT_NQ(NULL, pde);
	page_table *pt = (page_table *)PAGE_GET_PHYSICAL_ADDRESS(pde);
	EXPECT_NQ(NULL, pt);
	pt_entry *pte = ptable_lookup_entry(pt, TEST_ADDR);
	EXPECT_NQ(NULL, pt);
	physical_addr block = pt_entry_frame(*pte);
	EXPECT_NQ(NULL, block);	
	free_page(TEST_ADDR);
}

TEST(FreeOneVaddr)
{
	free_page(TEST_ADDR);
	pd_entry *pde = pdirectory_lookup_entry(cur_directory, TEST_ADDR);
	page_table *pt = (page_table *)PAGE_GET_PHYSICAL_ADDRESS(pde);
	pt_entry *pte = ptable_lookup_entry(pt, TEST_ADDR);
	physical_addr block = pt_entry_frame(*pte);
	EXPECT_FALSE(is_alloced(block));
	EXPECT_FALSE(pt_entry_is_present(pte));	
}

TEST(VirtToPhys)
{
	physical_addr paddr_from_lookup;
	physical_addr paddr_from_frame;
	
	alloc_page(TEST_ADDR);
	paddr_from_lookup = virt_to_phys(TEST_ADDR);
	EXPECT_NQ(NULL, paddr_from_lookup);
	free_page(TEST_ADDR);
}

END_SUITE();

void virt_mem_test(void)
{
	RUN_SUITE(virtmemTest);
}
