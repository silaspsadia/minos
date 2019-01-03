#include <stdio.h>
#include <math.h>
#include <libkern/phys_mem.h>
#include <libkern/kobj_cache.h>

kobj_cache_t *kobj_cache_create(size_t size)
{
	physical_addr		kobj_cache_addr, kobj_ctl_addr;
	kobj_ctl_t		*kobj_ctl_arr;
	int 			i, num_blocks, num_obj, tot_size;
	
	for (num_blocks = 1, tot_size = PHYS_BLOCK_SIZE * num_blocks - sizeof(kobj_cache_t);
			tot_size - (div_ceil(tot_size, size) * size)
				>= tot_size / 8;
			tot_size = PHYS_BLOCK_SIZE * num_blocks - sizeof(kobj_cache_t))
		num_blocks *= 2;
	
	kobj_cache_addr = alloc_blocks(num_blocks);
	printf("[KOBJ_CACHE] Done allocating blocks for kobj.\n");
	num_obj = div_ceil(tot_size, size);

	((kobj_cache_t *)kobj_cache_addr)->num_obj = num_obj;
	((kobj_cache_t *)kobj_cache_addr)->size_obj = size;
	
	kobj_ctl_addr = alloc_blocks(div_ceil(num_obj * sizeof(kobj_ctl_t), PHYS_BLOCK_SIZE));
	kobj_ctl_arr = (kobj_ctl_t *)kobj_ctl_addr;
	printf("[KOBJ_CACHE] Done allocating blocks for kobj_ctl.\n");
	
	for (i = 0; i < num_obj - 1; i++) {
		printf("%i\n", i);
		kobj_ctl_arr[i].next = &(kobj_ctl_arr[i+1]);
		printf("set next\n");
		kobj_ctl_arr[i].kobj_loc = kobj_cache_addr + sizeof(kobj_cache_t) + (size * i);
		printf("set loc\n");
	}
	kobj_ctl_arr[num_obj - 1].next = NULL;	

	printf("[KOBJ_CACHE] FINISHED CREATING CACHE..\n");
	return (kobj_cache_t *)kobj_cache_addr;
}
