#include <stdio.h>
#include <math.h>
#include <libkern/phys_mem.h>
#include <libkern/kobj_cache.h>

kobj_cache_t *kobj_cache_create(size_t size)
{
	kobj_cache_t    *cache;
	physical_addr	kobj_cache_addr;
	kobj_ctl_t	*kobj_ctl_arr;
	int 			i, num_blocks, num_obj, tot_size;
	
	for (num_blocks = 1, tot_size = PHYS_BLOCK_SIZE * num_blocks - sizeof(kobj_cache_t);
			tot_size - (div_ceil(tot_size, size) * size)
				>= tot_size / 8;
			tot_size = PHYS_BLOCK_SIZE * num_blocks - sizeof(kobj_cache_t))
		num_blocks *= 2;
	
	kobj_cache_addr = alloc_blocks(num_blocks);
	num_obj = div_ceil(tot_size, size);

	cache = (kobj_cache_t *)kobj_cache_addr;
	cache->num_obj = num_obj;
	cache->size_obj = size;
	cache->num_ctl_blocks = div_ceil(num_obj * sizeof(kobj_ctl_t), PHYS_BLOCK_SIZE);
	cache->num_obj_blocks = num_blocks;
	
	kobj_ctl_arr = (kobj_ctl_t *)alloc_blocks(cache->num_ctl_blocks);
	cache->kobj_ctl_list = kobj_ctl_arr;
	
	for (i = 0; i < num_obj - 1; i++) {
		kobj_ctl_arr[i].next = &kobj_ctl_arr[i+1];
		kobj_ctl_arr[i].kobj_loc = kobj_cache_addr + sizeof(kobj_cache_t) + (size * i);
	}
	kobj_ctl_arr[num_obj - 1].next = NULL;	

	printf("[KOBJ_CACHE] FINISHED CREATING CACHE..\n");
	return (kobj_cache_t *)kobj_cache_addr;
}

kobj_ctl_t *kobj_alloc(kobj_cache_t *cache)
{
	kobj_ctl_t *ret;

	if (cache->kobj_ctl_list) {
		ret = cache->kobj_ctl_list;
		cache->kobj_ctl_list = cache->kobj_ctl_list->next;
		ret->next = NULL;
		return ret;
	}
	return NULL;
}

int kobj_free(kobj_ctl_t *kobj_ctl, kobj_cache_t *cache)
{
	kobj_ctl_t *cur;
	
	cur = cache->kobj_ctl_list;
	if (cur == NULL) {
		cache->kobj_ctl_list = kobj_ctl;
		return 0;
	}
	while (cur->next != NULL)
		cur = cur->next;
	cur->next = kobj_ctl;
	return 0;
}

int __kobj_cache_count(kobj_cache_t *cache)
{
	kobj_ctl_t	*cur;
	int		i;

	i = 0;
	cur = cache->kobj_ctl_list;
	while (cur != NULL) {
		i++;
		cur = cur->next;
	}
	return i;
}

int __kobj_cache_destroy(kobj_cache_t *cache) {
	// have to explicitly free both b/c ctl array may not be adjacent
	free_blocks((physical_addr)cache->kobj_ctl_list, cache->num_ctl_blocks);
	free_blocks((physical_addr)cache, cache->num_obj_blocks);
}
