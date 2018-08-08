#include <stdio.h>

#include <extern/multiboot.h>
#include <kernel/tty.h>
#include <arch/i386/io.h>
#include <arch/i386/idt.h>
#include <arch/i386/gdt.h>
#include <arch/i386/kb.h>
#include <libbap/bap_test.h>
#include <libkern/heap.h>
#include <libkern/phys_mem.h>
#include <libkern/virt_mem.h>

void kernel_welcome(void) {
	terminal_centerwrite("~*~*~*~*~*~*~*~*~*~*~\n");
	terminal_centerwrite("* Welcome to MinOS. *\n");
	terminal_centerwrite("*~*~*~*~*~*~*~*~*~*~*\n\n");
}

void kernel_init(struct multiboot_info *mb) {
	terminal_initialize();
	kernel_welcome();
	gdt_init();
	idt_init();
	phys_memory_init(mb);
	virt_memory_init();
	kheap_init();
	kb_init();
	asm(" sti ");

	int *arr0 = kmalloc(sizeof(int) * 5000);
	int *arr1 = kmalloc(sizeof(int) * 30000);
	int *arr2 = kmalloc(sizeof(int) * 3000);
	
	printf("Array arr0 of size 5000 at %x\n", arr0);
	printf("Array arr1 of size 30000 at %x\n", arr1);
	printf("Array arr2 of size 3000 at %x\n", arr2);
}

void kernel_main(void) {
	for(;;)
		asm("hlt");
}
