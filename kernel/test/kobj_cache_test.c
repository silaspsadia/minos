#include <libkern/kobj_cache.h>
#include <test/kobj_cache_test.h>
#include <test/unit.h>

NEW_TEST_SUITE(kobj_cacheTest, 1);

TEST(KobjCacheCreate)
{
	// shouldn't panic
	struct foo {
		int i;
		int j;
	};
	EXPECT_NQ(NULL, kobj_cache_create(sizeof(struct foo)));
}

END_SUITE();

void kobj_cache_test(void)
{
	RUN_SUITE(kobj_cacheTest);
}
