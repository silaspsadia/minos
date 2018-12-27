#include <stdio.h>
#include <extern/multiboot.h>
#include <kernel/tty.h>
#include <arch/i386/io.h>
#include <arch/i386/idt.h>
#include <arch/i386/gdt.h>
#include <arch/i386/kb.h>
#include <arch/i386/timer.h>
#include <libkern/heap.h>
#include <libkern/phys_mem.h>
#include <libkern/virt_mem.h>
#include <libkern/tst.h>
#include <test/macros_test.h>
#include <test/kmalloc_test.h>
#include <test/phys_mem_test.h>

// TODO: handle TEST symbol better
#define TEST 1

void kernel_welcome(void)
{
	terminal_centerwrite("~*~*~*~*~*~*~*~*~*~*~\n");
	terminal_centerwrite("* Welcome to MinOS. *\n");
	terminal_centerwrite("*~*~*~*~*~*~*~*~*~*~*\n\n");
}

void kernel_init(struct multiboot_info *mb)
{
	terminal_initialize();
	kernel_welcome();
	gdt_init();
	idt_init();
	phys_memory_init(mb);
	virt_memory_init();
	kheap_init();
	kb_init();
	timer_init();
#ifdef TEST
	macros_test();
	kmalloc_test();
	phys_mem_test();
	virt_mem_test();
#endif
	asm(" sti ");
}


void kernel_main(void)
{
	for(;;)
		asm("hlt");
}
