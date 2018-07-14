#include <stdint.h>
#include <stdio.h>
#include "paging.h"

extern void load_pdt(uint32_t page_directory);

void paging_init(void) {
	load_pdt(0xC0100000); // Kernel Start
	printf("[Init] Paging set up successfully.\n");
}
