#include <libkern/kobj_cache.h>
#include <test/kobj_cache_test.h>
#include <test/unit.h>

NEW_TEST_SUITE(kobj_cacheTest, 4);

TEST(KobjCacheCreate)
{
	// shouldn't panic
	struct foo {
		int i;
		int j;
	};
	EXPECT_NQ(NULL, kobj_cache_create(sizeof(struct foo)));
}

TEST(KobjAllocSafe)
{
	// tests whether pages are allocated safely
	struct bird {
		int 	feathers;
		char	*name;
	};
	struct bird	*bird0;
	kobj_cache_t	*cache0;
	kobj_ctl_t	*kobj0;

	cache0 = kobj_cache_create(sizeof(struct bird));
	kobj0 = kobj_alloc(cache0);
	bird0 = (struct bird *)kobj0->kobj_loc;
	bird0->feathers = 100;
	bird0 ->name = "steve";
	printf("kobj_cache_t: %x\n", cache0);
	printf("kobj_ctl_t: %x\n", kobj0);
}

TEST(KobjAlloc)
{
	// tests that alloc successfully pops kobj_ctl's
	struct cat {
		int 	lives;
		char 	*name;
	};
	int		before, after_alloc;
	kobj_cache_t	*cache0;
	kobj_ctl_t	*kobj_ctl0;

	cache0 = kobj_cache_create(sizeof(struct cat));
	before = __kobj_cache_count(cache0);
	kobj_ctl0 = kobj_alloc(cache0);
	after_alloc = __kobj_cache_count(cache0);
	EXPECT_EQ(after_alloc, before - 1);
}

TEST(KobjFree)
{	
	struct dog {
		int 	bones;
		char 	*name;
	};
	int		before, after_free;
	kobj_cache_t	*cache0;
	kobj_ctl_t	*kobj_ctl0;
	
	cache0 = kobj_cache_create(sizeof(struct dog));
	before = __kobj_cache_count(cache0);
	kobj_ctl0 = kobj_alloc(cache0);
	kobj_free(kobj_ctl0, cache0);
	after_free = __kobj_cache_count(cache0);
	EXPECT_EQ(after_free, before);

}

END_SUITE();

void kobj_cache_test(void)
{
	RUN_SUITE(kobj_cacheTest);
}
