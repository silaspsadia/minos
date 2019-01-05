#ifndef LIBKERN_LIST_H
#define LIBKERN_LIST_H

#include <stddef.h>

struct list_head {
	struct list_head *prev, *next;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define LIST_HEAD(name) \
		struct list_head name = LIST_HEAD_INIT(name)

// Internal list manipulation
static inline void __list_add(struct list_head *entry, 
	struct list_head *prev, struct list_head *next) 
{
	next->prev = entry;
	entry->next = next;
	entry->prev = prev;
	prev->next = entry;
}

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void __list_del_entry(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
}

// Main list functions
static inline void list_add(struct list_head *entry, struct list_head *head)
{
	__list_add(entry, head, head->next);
}

static inline void list_add_tail(struct list_head *entry, struct list_head *head)
{
	__list_add(entry, head->prev, head);
}

static inline void list_del(struct list_head *entry)
{
	__list_del_entry(entry);
	entry->next = NULL;
	entry->prev = NULL;
}

static inline int list_is_empty(struct list_head *head)
{
	return head->next == head;
}

#endif /* LIBKERN_LIST_H */
