#ifndef EMBALAND_LIST_H
#define EMBALAND_LIST_H 1

#include "platform.h"

struct list_head {
	struct list_head *next;
	struct list_head *prev;
};

/** List initializer */
#define LIST_HEAD_INIT(name)                     \
	{                                        \
		.next = &(name), .prev = &(name) \
	}

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize list.
 * @param list is list to initialize
 */
EMB_LOCAL inline void list_init(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

/**
 * Add new item to list tail.
 * @param list is target list to add item in
 * @param item is item to be added to the tail of @a list
 */
EMB_LOCAL inline void list_add_tail(struct list_head *list,
				    struct list_head *item)
{
	list->prev->next = item;
	item->next = list;
	item->prev = list->prev;
	list->prev = item;
}

/**
 * Delete item from the list.
 * @param item is the item to delete
 */
EMB_LOCAL inline void list_del_init(struct list_head *item)
{
	item->prev->next = item->next;
	item->next->prev = item->prev;
	item->next = item;
	item->prev = item;
}

#ifdef __cplusplus
}
#endif
#endif
