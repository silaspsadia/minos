#include <libkern/phys_mem.h>
#include <test/phys_mem_test.h>
#include <test/unit.h>

NEW_TEST_SUITE(physmemTest, 7);

TEST(AllocBlock)
{
	physical_addr blockp = alloc_block();
	EXPECT_TRUE(is_alloced(blockp));
	free_block(blockp);
}

TEST(FreeBlock)
{
	physical_addr blockp = alloc_block();
	EXPECT_TRUE(is_alloced(blockp));
	free_block(blockp);
	EXPECT_FALSE(is_alloced(blockp));
}

TEST(AllocSameBlock)
{
	physical_addr addr0 = alloc_block();
	free_block(addr0);
	EXPECT_FALSE(is_alloced(addr0));
	physical_addr addr1 = alloc_block();
	EXPECT_EQ(addr0, addr1);
}

TEST(AllocBlocks)
{
	physical_addr first_block = alloc_blocks(5);
	EXPECT_TRUE(is_alloced(first_block));
	for (int i = 0; i < 5; i++)
		EXPECT_TRUE(is_alloced(first_block + (PHYS_BLOCK_SIZE * i)));
}

TEST(FreeBlocks)
{
	physical_addr first_block = alloc_blocks(5);
	free_blocks(first_block, 5);
	for (int i = 0; i < 5; i++)
		EXPECT_FALSE(is_alloced(first_block + (PHYS_BLOCK_SIZE * i)));
}

TEST(FreeBlocksIndividually)
{
	physical_addr loc, first_block = alloc_blocks(5);
	for (int i = 0; i < 5; i++) {
		loc = first_block + (PHYS_BLOCK_SIZE * i);
		free_block(loc);
		EXPECT_FALSE(is_alloced(loc));
	}
}

TEST(UsedBlockCounter)
{
	uint32_t used = used_blocks_;
	printf("Used: %u\tTotal:%u\n", used_blocks_, total_blocks_);
	physical_addr first_block = alloc_blocks(33);
	EXPECT_EQ(used_blocks_, used + 33);
	free_blocks(first_block, 33);
}

END_SUITE();

void phys_mem_test(void)
{
	RUN_SUITE(physmemTest);
}
