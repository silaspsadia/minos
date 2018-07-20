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
	sll_push_back(s, 6);
	sll_push_back(s, 5);
	sll_push_back(s, 4);
	sll_push_back(s, 3);
	sll_push_back(s, 2);
	sll_push_back(s, 1);
	sll_swap(s, 3, 2);
	sll_swap(s, 3, 4);
	sll_swap(s, 0, 5);
	sll_swap(s, 2, 1);
	sll_print(s);
}
