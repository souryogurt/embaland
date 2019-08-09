/**
 * @file
 * Test suite for double-linked list
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "list.h"

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

Ensure(emb_list_init_sets_emtpy_list)
{
	struct list_head list = { NULL };

	list_init(&list);
	assert_that(list.next, is_equal_to(&list));
	assert_that(list.prev, is_equal_to(&list));

	struct list_head list2 = LIST_HEAD_INIT(list2);
	assert_that(list2.next, is_equal_to(&list2));
	assert_that(list2.prev, is_equal_to(&list2));
}

Ensure(emb_list_add_tail_adds_item_to_tail)
{
	struct list_head list = LIST_HEAD_INIT(list);
	struct list_head item1 = LIST_HEAD_INIT(item1);
	list_add_tail(&list, &item1);
	assert_that(list.next, is_equal_to(&item1));
	assert_that(list.prev, is_equal_to(&item1));
	assert_that(item1.next, is_equal_to(&list));
	assert_that(item1.prev, is_equal_to(&list));

	struct list_head item2 = LIST_HEAD_INIT(item2);
	list_add_tail(&list, &item2);
	assert_that(list.next, is_equal_to(&item1));
	assert_that(list.prev, is_equal_to(&item2));
	assert_that(item1.next, is_equal_to(&item2));
	assert_that(item1.prev, is_equal_to(&list));
	assert_that(item2.next, is_equal_to(&list));
	assert_that(item2.prev, is_equal_to(&item1));

	struct list_head item3 = LIST_HEAD_INIT(item3);
	list_add_tail(&list, &item3);
	assert_that(list.next, is_equal_to(&item1));
	assert_that(item1.next, is_equal_to(&item2));
	assert_that(item2.next, is_equal_to(&item3));
	assert_that(item3.next, is_equal_to(&list));
	assert_that(list.prev, is_equal_to(&item3));
	assert_that(item3.prev, is_equal_to(&item2));
	assert_that(item2.prev, is_equal_to(&item1));
	assert_that(item1.prev, is_equal_to(&list));
}

Ensure(emb_list_del_init_removes_item_and_initialize)
{
	struct list_head list = LIST_HEAD_INIT(list);
	struct list_head item1 = LIST_HEAD_INIT(item1);
	struct list_head item2 = LIST_HEAD_INIT(item2);
	struct list_head item3 = LIST_HEAD_INIT(item3);
	list_add_tail(&list, &item1);
	list_add_tail(&list, &item2);
	list_add_tail(&list, &item3);

	list_del_init(&item2);
	assert_that(list.next, is_equal_to(&item1));
	assert_that(item1.next, is_equal_to(&item3));
	assert_that(item3.next, is_equal_to(&list));
	assert_that(list.prev, is_equal_to(&item3));
	assert_that(item3.prev, is_equal_to(&item1));
	assert_that(item1.prev, is_equal_to(&list));
	assert_that(item2.next, is_equal_to(&item2));
	assert_that(item2.prev, is_equal_to(&item2));

	list_del_init(&item3);
	assert_that(list.next, is_equal_to(&item1));
	assert_that(item1.next, is_equal_to(&list));
	assert_that(list.prev, is_equal_to(&item1));
	assert_that(item1.prev, is_equal_to(&list));
	assert_that(item3.next, is_equal_to(&item3));
	assert_that(item3.prev, is_equal_to(&item3));

	list_del_init(&item1);
	assert_that(list.next, is_equal_to(&list));
	assert_that(list.prev, is_equal_to(&list));
	assert_that(item1.next, is_equal_to(&item1));
	assert_that(item1.prev, is_equal_to(&item1));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("list");
	add_test(suite, emb_list_init_sets_emtpy_list);
	add_test(suite, emb_list_add_tail_adds_item_to_tail);
	add_test(suite, emb_list_del_init_removes_item_and_initialize);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
