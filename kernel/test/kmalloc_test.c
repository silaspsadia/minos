#include <libkern/heap.h>
#include <test/kmalloc_test.h>
#include <test/unit.h>

NEW_TEST_SUITE(kmallocTest, 12);

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

TEST(Kfree)
{
	// Should not kernel panic
	void *ptr0 = kmalloc(5000);
	void *ptr1 = kmalloc(5000);
	void *ptr2 = kmalloc(5000);
	void *ptr3 = kmalloc(5000);
	void *ptr4 = kmalloc(5000);
	void *ptr5 = kmalloc(5000);
	kfree(ptr4);
	kfree(ptr2);
	kfree(ptr0);
	kfree(ptr5);
	kfree(ptr3);
	kfree(ptr1);
}

TEST(HeapReset)
{
	header_t *head = get_flist_head();
	__kheap_reset();
	print_flist_head();	
	EXPECT_EQ(head, head->prev);	
	EXPECT_EQ(head, head->next);
}

TEST(FillandEmptyHeap)
{
	void *ptr0 = kmalloc(4000);
	void *ptr1 = kmalloc(4000);
	print_flist_head();	
	kfree(ptr0);
	kfree(ptr1);
	print_flist_head();	
	__kheap_reset();
}

TEST(UpperBoundaryMerge)
{
	header_t *head, *head_next;
	head = get_flist_head();
	void *ptr_0 = kmalloc(sizeof(header_t) * 20);
	void *ptr_1 = kmalloc(sizeof(header_t) * 10);
	void *ptr_2 = kmalloc(sizeof(header_t) * 10);
	void *ptr_3 = kmalloc(sizeof(header_t) * 10);
	void *ptr_next = kmalloc(8000);
	kfree(ptr_1);
	head_next = head->next;
	size_t size_at_start = head_next->size;
	kfree(ptr_2);
	printf("size at start: %u\n", size_at_start);
	EXPECT_NQ(head_next, head->next);
	EXPECT_EQ(head->next->size, size_at_start + 11);
	__kheap_reset();
}

TEST(LowerBoundaryMerge)
{
	header_t *head, *head_next;
	head = get_flist_head();
	void *ptr_mid = kmalloc(sizeof(header_t) * 20);
	void *ptr_first = kmalloc(sizeof(header_t) * 10);
	void *ptr_next = kmalloc(8000);
	size_t size_at_start = head->size;
	kfree(ptr_first);
	printf("size at start: %u\n", size_at_start);
	EXPECT_EQ(head->size, size_at_start + 11);
	__kheap_reset();
}

TEST(SandwichMerge)
{
	// Tests if lower boundary merge has higher precedence
	header_t *head, *head_next;
	head = get_flist_head();
	void *ptr0 = kmalloc(200);
	void *ptr1 = kmalloc(8000);
	size_t size_at_start = head->size;
	kfree(ptr0);
	printf("size at start: %u\n", size_at_start);
	EXPECT_EQ(head->size, size_at_start + 18);
	__kheap_reset();
}

__kheap_reset();

END_SUITE();

void kmalloc_test(void)
{
	RUN_SUITE(kmallocTest);
}
