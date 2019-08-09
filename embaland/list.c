/**
 * @file
 * Double-linked list implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "list.h"

EMB_LOCAL extern void list_init(struct list_head *list);
EMB_LOCAL extern void list_add_tail(struct list_head *list,
				    struct list_head *item);
EMB_LOCAL extern void list_del_init(struct list_head *item);
