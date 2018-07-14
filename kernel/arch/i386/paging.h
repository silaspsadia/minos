#ifndef PAGING_H
#define PAGING_H

extern void load_pdt(uint32_t page_directory);
void paging_init(void);
 
#endif /* PAGING_H */
