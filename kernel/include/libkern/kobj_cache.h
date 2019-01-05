#ifndef LIBKERN_KOBJ_CACHE_H
#define LIBKERN_KOBJ_CACHE_H

#include <asm.h>
#include <libkern/list.h>
#include <libkern/memlayout.h>
#include <libkern/paging.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct kobj_ctl {
	struct kobj_ctl *next;
	physical_addr kobj_loc;
} kobj_ctl_t;

typedef struct kobj_cache {
	size_t num_obj;
	size_t size_obj;
	size_t num_ctl_blocks;
	size_t num_obj_blocks;
	kobj_ctl_t *kobj_ctl_list;
	struct kobj_cache *kobj_cache_list_next;
} kobj_cache_t;

kobj_cache_t	*kobj_cache_create(size_t);
kobj_ctl_t	*kobj_alloc(kobj_cache_t *);
int		kobj_free(kobj_ctl_t *, kobj_cache_t *);
int		__kobj_cache_count(kobj_cache_t *);
#endif  // LIBKERN_KOBJ_CACHE_H
