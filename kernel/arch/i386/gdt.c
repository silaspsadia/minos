#include <arch/i386/gdt.h>
#include <stdio.h>
#include <stdint.h>

#define SEGMENT_BASE		0x00000
#define SEGMENT_LIMIT		0xFFFFF
#define CODE_RX_TYPE_ACCESS	0x9A
#define DATA_RW_TYPE_ACCESS	0x92
#define STANDARD_GRAN		0xCF

extern void gdt_load_and_set(uint32_t addr);

struct gdt_entry {
	uint16_t limit_low;		/* 16 bits of the limit; lowest bits */ 
	uint16_t base_low;		/* 16 bits of the base; lowest bits */
	uint8_t  base_mid;		/* 8 bits of the base; next after lowest */
	uint8_t  access;		/* Flags specifying access */
	uint8_t  granularity;	/* 4 bits granularity, 4 bits limit_high */
	uint8_t  base_high;		/* 8 bits of the base; highest bits; */
} __attribute__((packed));
typedef struct gdt_entry gdt_entry_t;

struct gdt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));
typedef struct gdt_ptr gdt_ptr_t;

gdt_entry_t gdt[GDT_NUM_ENTRIES];
gdt_ptr_t gdtp;


void gdt_setgate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, 
					uint8_t granularity) {
	gdt[num].limit_low 	 = 	limit ;

	gdt[num].base_low 	 = 	base & 0xFFFF;
	gdt[num].base_mid 	 = 	(base >> 16) & 0xFF;

	gdt[num].access		 = 	access;

	/* name | value | num_bits | description									  *	
	 * -------------------------------------------------------------------------- *   
	 * G	| 1		| 1		   | size of segment unit (def. granularity); 1 = 4KB *
	 * D/B	| 1		| 1		   | size of operation; 0 = 16-bit, 1 = 32-bit		  *
	 * L	| 0		| 1		   | arch bits; 0 = 32-bit, 1 = 64-bit				  *
	 * AVL	| 0		| 1		   | availability for use by system software		  *
	 * LIM	| F		| 4		   | four highest bits of segment limit				  *
	 */		
	
	gdt[num].granularity |= granularity | (0x01 << 7) | (0x01 << 6) | (0x0F);
	
	gdt[num].base_high	 = 	(base >> 24) & 0xFF;
}


void gdt_init(void) {
	gdtp.limit = sizeof(gdt_entry_t) * 3 - 1;
	gdtp.base  = (uint32_t) &gdt;

	/* gdt[0] : Null Descriptor */
	gdt_setgate(0, 0x00000, 0x00000, 0x00, 0x00);
	/* gdt[1] : Code Segment Descriptor */
	gdt_setgate(1, SEGMENT_BASE, SEGMENT_LIMIT, CODE_RX_TYPE_ACCESS, STANDARD_GRAN);
	/* gdt[2] : Data Segment  Descriptor */
	gdt_setgate(2, SEGMENT_BASE, SEGMENT_LIMIT, DATA_RW_TYPE_ACCESS, STANDARD_GRAN);
	gdt_load_and_set((uint32_t) &gdtp);
	printf("[Init] GDT set up successfully.\n");
} 
