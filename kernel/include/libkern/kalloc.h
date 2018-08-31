#ifndef LIBKERN_KALLOC_H
#define LIBKERN_KALLOC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <libkern/heap.h>
#include <libkern/list.h>
#include <libkern/poison.h>

#define _KALLOC_ZONE_SIZES_COUNT 24 

size_t 	_kalloc_zone_sizes = 
	{ 16, 32, 48, 64, 80, 96, 128, 160, 224, 256, 288, 368, 400, 512, 576, 768, 1024, 1152, 1280, 1664, 2048, 4096, 6044, 8192 };

enum 	_kalloc_zone_types
	{ KALLOC_16, KALLOC_32, KALLOC_48, KALLOC_64, KALLOC_80, KALLOC_96, KALLOC_128, KALLOC_160, KALLOC_224, KALLOC_256, KALLOC_288, KALLOC_368, KALLOC_400, KALLOC_512, KALLOC_576, KALLOC_768, KALLOC_1024, KALLOC_1152, KALLOC_1280, KALLOC_1664, KALLOC_2048, KALLOC_4096, KALLOC_6044, KALLOC_8192 };

list_head **_kalloc_head_list;

void kalloc_init() 
{
	_kalloc_head_list = malloc(sizeof(list_head *) * _KALLOC_ZONE_SIZES_COUNT);	
	for (int i = 0; i < _KALLOC_ZONE_SIZES_COUNT; i++) {
		memset(_kalloc_head_list[i], POISON_NEVER_INIT, sizeof(list_head *));
	}
	printf("[Mem ] Kalloc zones successfully initialized\n");
}

#endif /* LIBKERN_KALLOC_H */
