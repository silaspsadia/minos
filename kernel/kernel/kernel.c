#include <stdio.h>

#include <kernel/tty.h>

void kernel_welcome(void) {
	for (int i = 0; i < 9; i++) {
		printf("\n");
	}
	terminal_centerwrite("~*~*~*~*~*~*~*~*~*~*~\n");
	terminal_centerwrite("* Welcome to MinOS. *\n");
	terminal_centerwrite("*~*~*~*~*~*~*~*~*~*~*\n");
	printf("\n\n");
}

void kernel_main(void) {
	terminal_initialize();
	kernel_welcome();
}
