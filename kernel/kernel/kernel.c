#include <stdio.h>

#include <kernel/tty.h>
#include "../arch/i386/idtc.h"

void kernel_welcome(void) {
	for (int i = 0; i < 9; i++) {
		printf("\n");
	}
	terminal_centerwrite("~*~*~*~*~*~*~*~*~*~*~\n");
	terminal_centerwrite("* Welcome to MinOS. *\n");
	terminal_centerwrite("*~*~*~*~*~*~*~*~*~*~*\n");
	printf("\n\n");
	terminal_centerwrite("Press any key to continue");
}

void kernel_main(void) {
	terminal_initialize();
	kernel_welcome();
	idt_init();
	kb_init();
}
