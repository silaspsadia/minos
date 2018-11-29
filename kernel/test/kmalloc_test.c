#include <libkern/heap.h>
#include <test/kmalloc_test.h>
#include <test/unit.h>

NEW_TEST_SUITE(kmallocTest, 4);

TEST(ExpectKmalloc0ByteRequestNull)
{
	EXPECT_EQ(NULL, kmalloc(0));
}

TEST(LowerBoundaryKmalloc)
{
	EXPECT_NQ(NULL, kmalloc(1));
}

TEST(HighBoundaryKmalloc)
{
	EXPECT_NQ(NULL, kmalloc(50000));
}

TEST(InterleavedKmalloc)
{
	void *ptr_before = kmalloc(HEAP_BLOCK_SIZE * 5);
	void *ptr_mid = kmalloc(HEAP_BLOCK_SIZE * 3);
	void *ptr_after = kmalloc(HEAP_BLOCK_SIZE * 8);

	EXPECT_GT(ptr_mid, ptr_before);
	EXPECT_LT(ptr_mid, ptr_after);

	kfree(ptr_before);
	kfree(ptr_mid);
	kfree(ptr_after);
}

END_SUITE();

void kmalloc_test(void)
{
	RUN_SUITE(kmallocTest);
}
