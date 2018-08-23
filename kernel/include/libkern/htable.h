#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct htable_bucket_entry {
	char *item;
	struct htable_bucket_entry *next;
};

struct htable {
	int nbuckets;
	int nitems;
	int load_factor;
	unsigned int (*hash_function)(char *, size_t);
	struct htable_bucket_entry **buf;
};

struct htable_bucket_entry *entry_init(char *);
struct htable *htable_init(int, unsigned int (*)(char *, size_t));
void htable_insert(struct htable *, char *, size_t);
unsigned int stdhash(char *, size_t);
void htable_printout(struct htable *);

struct htable_bucket_entry *entry_init(char *obj) {
	struct htable_bucket_entry *ret = malloc(sizeof(struct htable_bucket_entry));
	ret->item = obj;
	ret->next = NULL;
	return ret;
}

struct htable *htable_init(int nb, unsigned int (*hf)(char *, size_t)) {
	struct htable *ret = malloc(sizeof(struct htable));
	ret->nbuckets = nb;
	ret->hash_function = hf;
	ret->nitems = 0;
	ret->load_factor = 0;
	ret->buf = malloc(nb * sizeof(struct htable_bucket_entry *));
	return ret;
}

void htable_insert(struct htable *ht, char *obj, size_t size) {
	unsigned int index = ht->hash_function(obj, size) % ht->nbuckets;
	struct htable_bucket_entry *bucket = (ht->buf)[index];
	if (bucket == NULL) {
		bucket = entry_init(obj);;
		(ht->buf)[index] = bucket;
	} else {
		struct htable_bucket_entry *new_entry = entry_init(obj);
		while (bucket->next != NULL)
			bucket = bucket->next;
		bucket->next = new_entry;
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

void htable_printout(struct htable *ht) {
	for (int i = 0; i < ht->nbuckets; i++) {
		struct htable_bucket_entry *curr = (ht->buf)[i];
		printf("%i [-]->", i);
		while (curr != NULL) {
			printf("[%p]->", curr->item);
			curr = curr->next;
		}
		printf("[\\]\n");
	}
}
