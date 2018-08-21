#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libkern/list.h>

struct htable {
	int nbuckets;
	int nitems;
	int load_factor;
	unsigned int (*hash_function)(char *, size_t);
	struct htable_bucket_entry **buf;
};

struct htable_bucket_entry {
	char *item;
	struct list_head head;
};

#define HTABLE_BUCKET_ENTRY_INIT(obj) { .item = obj, .head = &(.head) }

htable *htable_init(int, unsigned int (*)(char *, size_t));
void htable_register(htable *, char *, size_t);
unsigned int stdhash(char *, size_t);

htable *htable_init(int nb, unsigned int (*hf)(char *, size_t)) {
	htable *ret = malloc(sizeof(htable));
	ret->nbuckets = nb;
	ret->hash_function = hf;
	ret->nitems = 0;
	ret->load_factor = 0;
	ret->buf = malloc(nb * sizeof(list_head *));
	return ret;
}

void htable_register(htable *ht, char *obj, size_t size) {
	unsigned int index = ht->hash_function(obj, size) % ht->nbuckets;
	struct htable_bucket_entry *bucket = (ht->buf)[index];
	if (bucket == NULL) {
		bucket = HTABLE_BUCKET_ENTRY_INIT(obj);
		(ht->buf)[index] = bucket;
	} else {
		struct htable_bucket_entry *new_entry;
		new_entry = HTABLE_BUCKET_ENTRY_INIT(obj);
		list_add(new_entry->head, bucket->head);
	}	
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
