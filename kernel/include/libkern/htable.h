#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./sllist.h"

typedef struct htable_t {
	int nbuckets;
	int nitems;
	int load_factor;
	unsigned int (*hash_function)(char *, size_t);
	struct sllnode **buf;
} htable;

htable *htable_init(int, unsigned int (*)(char *, size_t));
int htable_register(htable *, char *, size_t);
unsigned int stdhash(char *, size_t);

htable *htable_init(int nb, unsigned int (*hf)(char *, size_t)) {
	htable *ret = malloc(sizeof(htable));
	ret->nbuckets = nb;
	ret->hash_function = hf;
	ret->nitems = 0;
	ret->load_factor = 0;
	ret->buf = malloc(nb * sizeof(void *));
	return ret;
}

int htable_register(htable *ht, char *obj, size_t size) {
	unsigned int hash = ht->hash_function(obj, size) % ht->nbuckets;
	struct sllnode *loc = (ht->buf)[hash];
	struct sllnode *prev = loc;
	if (loc == NULL) {
		(ht->buf)[hash] = sllnode_init((int) obj);
		loc = (ht->buf)[hash];
		loc->item = (int) obj;
	} else {
		while (loc != NULL) {
			prev = loc;
			loc = loc->next;
		}
		prev->next = sllnode_init((int) obj);
	}
	return 1;
}

unsigned int stdhash(char *input, size_t size) {
#define A 54059
#define B 76963
#define C 86869
#define SEED 37
	unsigned int h = SEED;
	for (int i = 0; i < size; i++) 
		h = (h * A) ^ (input[i] * B);
	return h % C;
}
