/**
 * @file
 * Test suite for reference counter
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "embaland/ref.h"

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

Ensure(emb_ref_init_sets_counter_to_one)
{
	struct emb_ref ref = { 0 };

	emb_ref_init(&ref);
	assert_that(ref.count, is_equal_to(1));
}

Ensure(emb_ref_get_increment_counter_by_one)
{
	struct emb_ref ref = { 0 };
	emb_ref_init(&ref);

	emb_ref_get(&ref);
	assert_that(ref.count, is_equal_to(2));
}

void mock_release(struct emb_ref *ref)
{
	mock(ref);
}

Ensure(emb_ref_put_decrement_counter_by_one)
{
	struct emb_ref ref = { 0 };
	emb_ref_init(&ref);
	emb_ref_get(&ref);

	never_expect(mock_release);
	bool is_removed = emb_ref_put(&ref, mock_release);
	assert_that(ref.count, is_equal_to(1));
	assert_that(is_removed, is_false);
}

Ensure(emb_ref_put_calls_release_when_counter_become_zero)
{
	struct emb_ref ref = { 0 };
	emb_ref_init(&ref);

	expect(mock_release, when(ref, is_equal_to(&ref)));
	bool is_removed = emb_ref_put(&ref, mock_release);
	assert_that(ref.count, is_equal_to(0));
	assert_that(is_removed, is_true);

	never_expect(mock_release);
	assert_that(emb_ref_put(&ref, mock_release), is_false);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("emb_ref");
	add_test(suite, emb_ref_init_sets_counter_to_one);
	add_test(suite, emb_ref_get_increment_counter_by_one);
	add_test(suite, emb_ref_put_decrement_counter_by_one);
	add_test(suite, emb_ref_put_calls_release_when_counter_become_zero);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
