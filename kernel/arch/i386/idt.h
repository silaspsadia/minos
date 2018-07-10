#ifndef _KERNEL_IDT_H_
#define _KERNEL_IDT_H_

#include <stdint.h>

#define IDT_NUM_ENTRIES 256

// Sets up the IDT, should be called on early initialization
void idt_init();

#endif  // _KERNEL_IDT_H
