#include <stdint.h>
#include "idtc.h"

/* This macro is used for general interrupt init */
#define CREATE_IDT_GATE(idx) \
	create_idt_gate(idx, (uint32_t) &interrupt_handler_##idx);

/* This is used to declare interrupts (external asm function) */
#define DECLARE_INTERRUPT_HANDLER(i) \
	void interrupt_handler##i(void);

static const uint8_t PIC1_COMMAND = 0x20;
static const uint8_t PIC1_DATA = 0x21;
static const uint8_t PIC2_COMMAND = 0xa0;
static const uint8_t PIC2_DATA = 0xa1;

struct idt_gate {
	uint16_t offset_lowerbits;
	uint16_t segsel;
	uint8_t zero;
	uint8_t config;
	uint16_t offset_higherbits;
} __attribute__((packed));
typedef struct idt_gate idt_gate_t;

struct idt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));
typedef struct idt_ptr idt_ptr_t;

idt_gate_t idt[IDT_NUM_ENTRIES];

static void create_idt_gate(uint8_t n, uint32_t handler) {
	idt[n].offset_lowerbits = handler & 0x0000FFFF;
	idt[n].segsel = SEGSEL_KERNEL_CS;
	idt[n].zero = 0;
	idt[n].config = INTERRUPT_GATE; // Needs to be customized later (different pl) 
	idt[n].offset_higherbits = (handler >> 16) & 0x0000FFFF;
}

void idt_init(void) {
	idt_ptr_t idt_ptr;
	idt_ptr.limit = IDT_NUM_ENTRIES * sizeof(idt_gate_t) - 1; 
	idt_ptr.base = (uint32_t) &idt;

	/* Add keyboard support for now */
	create_idt_gate(0x21, keyboard_handler);
	idt_load((uint32_t)	&idt_ptr);
}
