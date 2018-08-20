#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tst_node_t {
	struct tst_node_t *left;		
	struct tst_node_t *mid;		
	struct tst_node_t *right;
	int just_made;
	int key;
	char prefix;	
} tst_node;

typedef struct tst_t {
	tst_node *head;
} tst;	

tst *tst_init(void);
tst_node *tst_node_init(void);
void tst_node_insert(tst_node *head, char *word);
void tst_insert(tst *tree, char *word);

tst *tst_init(void) {
	tst *ret = malloc(sizeof(tst));
	ret->head = tst_node_init();
	return ret;
}

tst_node *tst_node_init(void) {
	tst_node *ret = calloc(1, sizeof(tst_node));
	ret->just_made = 1;
	return ret;
}

void tst_node_insert(tst_node *head, char *word) {
	int diff;
	char *substring = malloc(1000);
	char cur_char = word[0];
	if (cur_char == '\0') {	
		head->key++;
		return;
	}
	if (head->prefix == '\0') {
		head->prefix = word[0];
		head->mid = tst_node_init();
		strcpy(substring, word + 1);		
		return tst_node_insert(head->mid, substring);
	}
	diff = head->prefix - cur_char;
	if (diff > 0) {
		if (head->left == NULL) 
			head->left = tst_node_init();
		strcpy(substring, word);
		return tst_node_insert(head->left, substring);
	} else if (diff == 0) {
		if (head->mid == NULL)
			head->mid = tst_node_init();
		strcpy(substring, word + 1);
		return tst_node_insert(head->mid, substring);
	} else {	
		if (head->right == NULL)
			head->right = tst_node_init();
		strcpy(substring, word);
		return tst_node_insert(head->right, substring);
	}
}

void tst_insert(tst *tree, char *word) {
	/* Put queuing and caching stuff here */
	tst_node_insert(tree->head, word);
}

void tst_node_printout(tst_node *head, char *strconstruct) {
	if (head == NULL)
		return;
	int len = strlen(strconstruct);
	char *strnew = calloc(len + 1, sizeof(char));
	strcpy(strnew, strconstruct);
	strnew[len] = head->prefix;
	if (head->prefix == '\0') {
		printf("%s\n", strnew);
		return;
	}
	if (head->key > 0) {
		char *finish = calloc(len + 1, sizeof(char));
		strcpy(finish, strconstruct);
		printf("%s\n", finish);
	}
	tst_node_printout(head->left, strconstruct);
	tst_node_printout(head->mid, strnew);
	tst_node_printout(head->right, strconstruct);
}

void tst_printout(tst *tree) {
	tst_node_printout(tree->head, "");
}

int tst_find(tst *tree, char *word) {
	int i, diff;
	char cur;
	tst_node *cur_node = tree->head;
	if (cur_node == NULL)
		return 0;

	i = 0;
	while ((cur = word[i]) != '\0') {
		if (cur_node == NULL)
			return 0;
		diff = cur_node->prefix - cur;
		if (diff == 0) {
			cur_node = cur_node->mid;
			i++;
		} else if (diff < 0) {
			cur_node = cur_node->right;
		} else {
			cur_node = cur_node->left;
		}
	}
	if (cur_node != NULL && cur_node->key > 0)
		return 1;
	return 0;
}
