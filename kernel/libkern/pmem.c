#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <libkern/pmem.h>

inline void mmap_set(int bit) {
	_pmem_memory_map[bit/32] |= (1 << (bit % 32));
}

inline void mmap_unset(int bit) {
	_pmem_memory_map[bit/32] &= ~(1 << (bit % 32));
}
	
inline int mmap_test(int bit) {
	return _pmem_memory_map[bit/32] & (1 << (bit % 32));
}	

int mmap_first_free() {
	for (int i = 0; i < _pmem_max_blocks; i++) {
		uint32_t block = _pmem_memory_map[i];
		if (block != 0xFFFFFFFF) {
			for (int j = 0; j < 32; j++) {
				int bit = 1 << j;
				if (!(bit & block))
					return (32 * i) + j;
			}
		}
	}
	return -1;	
}
