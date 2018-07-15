#ifndef VMEM_H
#define VMEM_H

#include <stdbool.h>

bool alloc_page(vaddr);
bool free_page(vaddr);
void map_page(vaddr, paddr);

#endif /* VMEM_H */

