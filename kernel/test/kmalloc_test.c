#include <libkern/heap.h>
#include <test/kmalloc_test.h>
#include <test/unit.h>

NEW_TEST_SUITE(kmallocTest, 5);

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

TEST(ConsecutiveAcquireMoreHeap)
{
	void *ptr_before = kmalloc(256);
	void *ptr_mid = kmalloc(10000);
	void *ptr_after = kmalloc(10000);

	EXPECT_NQ(NULL, ptr_before);
	EXPECT_NQ(NULL, ptr_mid);
	EXPECT_NQ(NULL, ptr_after);
}

END_SUITE();

void kmalloc_test(void)
{
	RUN_SUITE(kmallocTest);
}
