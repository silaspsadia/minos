#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define GDT_NUM_ENTRIES 3

extern void gdt_load_and_set(uint32_t addr);
void gdt_init(void);

#endif /* GDT_H */
