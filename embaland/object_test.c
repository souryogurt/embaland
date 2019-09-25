/**
 * @file
 * Test suite for base object
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "object.h"

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

void mock_release(struct emb_object *obj)
{
	mock(obj);
}

static struct emb_type mock_object_type = {
	.release = mock_release,
};

Ensure(emb_object_init_setups_object)
{
	struct emb_object obj = { 0 };

	emb_object_init(&obj, &mock_object_type);
	assert_that(obj.ref.count, is_equal_to(1));
	assert_that(obj.type, is_equal_to(&mock_object_type));
}

Ensure(emb_object_get_increases_reference_counter)
{
	struct emb_object obj = { 0 };
	emb_object_init(&obj, &mock_object_type);

	assert_that(emb_object_get(&obj), is_equal_to(&obj));
	assert_that(obj.ref.count, is_equal_to(2));
}

Ensure(emb_object_put_calls_types_release)
{
	struct emb_object obj = { 0 };
	emb_object_init(&obj, &mock_object_type);

	expect(mock_release, when(obj, is_equal_to(&obj)));
	emb_object_put(&obj);
}

Ensure(emb_object_add_adds_object_to_hierarhy)
{
	struct emb_object root = { 0 };
	emb_object_init(&root, &mock_object_type);
	struct emb_object obj = { 0 };
	emb_object_init(&obj, &mock_object_type);

	emb_object_add(&obj, &root);
	assert_that(obj.parent, is_equal_to(&root));
	assert_that(root.ref.count, is_equal_to(2));
}

Ensure(emb_object_add_releases_parent_ref_if_already_added)
{
	struct emb_object root = { 0 };
	emb_object_init(&root, &mock_object_type);
	struct emb_object obj = { 0 };
	emb_object_init(&obj, &mock_object_type);
	emb_object_add(&obj, &root);
	struct emb_object root2 = { 0 };
	emb_object_init(&root2, &mock_object_type);

	emb_object_add(&obj, &root2);
	assert_that(obj.parent, is_equal_to(&root2));
	assert_that(root2.ref.count, is_equal_to(2));
	assert_that(root.ref.count, is_equal_to(1));
}

Ensure(emb_object_del_removes_object_from_hierarhy)
{
	struct emb_object root = { 0 };
	emb_object_init(&root, &mock_object_type);
	struct emb_object obj = { 0 };
	emb_object_init(&obj, &mock_object_type);
	emb_object_add(&obj, &root);

	emb_object_del(&obj);
	assert_that(obj.parent, is_null);
	assert_that(root.ref.count, is_equal_to(1));
}

Ensure(emb_object_put_removes_object_from_hierarhy)
{
	struct emb_object root = { 0 };
	emb_object_init(&root, &mock_object_type);
	struct emb_object obj = { 0 };
	emb_object_init(&obj, &mock_object_type);
	emb_object_add(&obj, &root);

	expect(mock_release);
	emb_object_put(&obj);
	assert_that(root.ref.count, is_equal_to(1));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("emb_object");
	add_test(suite, emb_object_init_setups_object);
	add_test(suite, emb_object_get_increases_reference_counter);
	add_test(suite, emb_object_put_calls_types_release);
	add_test(suite, emb_object_add_adds_object_to_hierarhy);
	add_test(suite, emb_object_add_releases_parent_ref_if_already_added);
	add_test(suite, emb_object_del_removes_object_from_hierarhy);
	add_test(suite, emb_object_put_removes_object_from_hierarhy);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
