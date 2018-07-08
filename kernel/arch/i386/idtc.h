#ifndef IDT_H
#define IDT_H

#define INTERRUPT_GATE 0x8e
#define SEGSEL_KERNEL_CS 0x08
#define SEGSEL_KERNEL_DS 0x10
#define SEGSEL_USER_SPACE_CS 0x18
#define SEGSEL_USER_SPACE_DS 0x20
#define IDT_NUM_ENTRIES 256

void idt_init(void);
extern void keyboard_handler(void); 
extern void idt_load(unsigned short idt_addr);

#endif /* IDT_H */
