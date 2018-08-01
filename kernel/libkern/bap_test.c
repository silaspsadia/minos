#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <libbap/bap.h>

void bap_test(void) {
	sll_t *s = sll_init();
	printf("Testing linked lists:\n");
	sll_push_back(s, 9);
	sll_push_back(s, 8);
	sll_push_back(s, 7);
	for (int i = 0; i < 100; i++) {
		int *place = kmalloc(sizeof(int) * 100);
		printf("[%x]\n", place);		
	}
	sll_print(s);
}
