#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define GDT_NUM_ENTRIES 3

void gdt_init(void);

#endif /* GDT_H */
