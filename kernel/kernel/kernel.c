#include <stdio.h>

#include <kernel/tty.h>
#include <../arch/i386/io.h>
#include <../arch/i386/gdt.h>
#include <../arch/i386/kb.h>

void kernel_welcome(void) {
	terminal_centerwrite("~*~*~*~*~*~*~*~*~*~*~\n");
	terminal_centerwrite("* Welcome to MinOS. *\n");
	terminal_centerwrite("*~*~*~*~*~*~*~*~*~*~*\n\n");
}

void kernel_init(void) {
	terminal_initialize();
	kernel_welcome();

	gdt_init();
	idt_init();
	kb_init();
	asm(" sti ");
}

void kernel_main(void) {
	for(;;)
		asm("hlt");
}
