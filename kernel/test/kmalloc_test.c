#include <libkern/heap.h>
#include <test/kmalloc_test.h>
#include <test/unit.h>

NEW_TEST_SUITE(kmallocTest, 3);

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

END_SUITE();

void kmalloc_test(void) { RUN_SUITE(kmallocTest); }
