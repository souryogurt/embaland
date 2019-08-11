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
	assert_that(obj.entry.next, is_equal_to(&obj.entry));
	assert_that(obj.entry.prev, is_equal_to(&obj.entry));
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

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("emb_object");
	add_test(suite, emb_object_init_setups_object);
	add_test(suite, emb_object_get_increases_reference_counter);
	add_test(suite, emb_object_put_calls_types_release);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
