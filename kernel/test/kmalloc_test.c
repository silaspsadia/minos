#include <libkern/heap.h>
#include <test/kmalloc_test.h>
#include <test/unit.h>

NEW_TEST_SUITE(kmallocTest, 6);

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
	void *ptr_mid = kmalloc(100);
	void *ptr_after = kmalloc(1000);

	EXPECT_NQ(NULL, ptr_before);
	EXPECT_NQ(NULL, ptr_mid);
	EXPECT_NQ(NULL, ptr_after);
}

TEST(ConsecutiveTinyKmalloc)
{
	void *ptr0 = kmalloc(1);
	void *ptr1 = kmalloc(1);
	void *ptr2 = kmalloc(1);
	void *ptr3 = kmalloc(1);
	void *ptr4 = kmalloc(1);
	void *ptr5 = kmalloc(1);

	EXPECT_NQ(NULL, ptr0);
	EXPECT_NQ(NULL, ptr1);
	EXPECT_NQ(NULL, ptr2);
	EXPECT_NQ(NULL, ptr3);
	EXPECT_NQ(NULL, ptr4);
	EXPECT_NQ(NULL, ptr5);
}

TEST(ConsecutiveLargeKmalloc)
{
	void *ptr0 = kmalloc(4200);
	void *ptr1 = kmalloc(4200);
	void *ptr2 = kmalloc(4200);
	void *ptr3 = kmalloc(4200);
	void *ptr4 = kmalloc(4200);
	void *ptr5 = kmalloc(4200);

	EXPECT_NQ(NULL, ptr0);
	EXPECT_NQ(NULL, ptr1);
	EXPECT_NQ(NULL, ptr2);
	EXPECT_NQ(NULL, ptr3);
	EXPECT_NQ(NULL, ptr4);
	EXPECT_NQ(NULL, ptr5);
}

END_SUITE();

void kmalloc_test(void)
{
	RUN_SUITE(kmallocTest);
}
