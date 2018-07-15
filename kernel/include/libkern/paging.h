#ifndef PAGING_H
#define PAGING_H

typedef uint32_t pte;
typedef uint32_t pde;

struct ptable {
	pte m_entries[PAGES_PER_TABLE];
};
 
struct pdir { 
	pde m_entries[PAGES_PER_DIR];
};

enum PAGE_PTE_FLAGS {
	I86_PTE_PRESENT = 1,
	I86_PTE_WRITABLE = 2,
	I86_PTE_USER = 4,
	I86_PTE_WRITETHOUGH = 8,
	I86_PTE_NOT_CACHEABLE = 0x10,
	I86_PTE_ACCESSED = 0x20,
	I86_PTE_DIRTY = 0x40,
	I86_PTE_PAT = 0x80,
	I86_PTE_CPU_GLOBAL = 0x100,
	I86_PTE_LV4_GLOBAL = 0x200,
	I86_PTE_FRAME = 0x7FFFF000
};

inline void		pte_add_attrib (pte *entry, uint32_t attrib);
inline void		pte_del_attrib (pte *entry, uint32_t attrib);
inline void		pte_set_frame (pte *entry, paddr addr);
inline int		pte_is_present (pte entry);
inline int		pte_is_writable (pte entry);
inline paddr	pde_entry_frame (pte entry);

enum PAGE_PDE_FLAGS {
	I86_PDE_PRESENT = 1,
	I86_PDE_WRITABLE = 2,
	I86_PDE_USER = 4,
	I86_PDE_PWT = 8,
	I86_PDE_PCD = 0x10,
	I86_PDE_ACCESSED = 0x20,
	I86_PDE_DIRTY = 0x40,
 	I86_PDE_4MB = 0x80,
	I86_PDE_CPU_GLOBAL = 0x100,
	I86_PDE_LV4_GLOBAL = 0x200,
	I86_PDE_FRAME = 0x7FFFF000
};

inline void		pde_add_attrib (pde *entry, uint32_t attrib);
inline void		pde_del_attrib (pde *entry, uint32_t attrib);
inline void		pde_set_frame (pde *entry, paddr addr);
inline int		pde_is_present (pde entry);
inline int		pde_is_user (pde entry);
inline int		pde_is_4mb (pde entry);
inline int		pde_is_writable (pde entry);
inline paddr	pde_entry_frame (pde entry);
inline void		pde_enable_global (pde entry);

#endif /* PAGING_H */
