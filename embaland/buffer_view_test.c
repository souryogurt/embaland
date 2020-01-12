/**
 * @file
 * Test suite for embaland buffer view
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "embaland.h"
#include "buffer.h"
#include "buffer_view.h"

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

Ensure(emb_buffer_view_init_returns_error_on_invalid_buffer)
{
	struct emb_buffer_view bview;
	enum emb_result result =
		emb_buffer_view_init(&bview, NULL, 0, 0, 0, EMB_ARRAY_BUFFER);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_buffer_view_init_returns_error_on_invalid_handle)
{
	struct emb_buffer buffer = { .capacity = 100 };
	enum emb_result result =
		emb_buffer_view_init(NULL, &buffer, 0, 0, 0, EMB_ARRAY_BUFFER);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_buffer_view_init_returns_error_on_invalid_range)
{
	struct emb_buffer_view bview;
	struct emb_buffer buffer = { .capacity = 100 };
	enum emb_result result = emb_buffer_view_init(&bview, &buffer, 95, 10,
						      0, EMB_ARRAY_BUFFER);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_buffer_view_init_allocates_resources_for_view)
{
	struct emb_buffer_view bview;
	struct emb_buffer buffer = { .capacity = 100 };
	enum emb_result result = emb_buffer_view_init(&bview, &buffer, 0, 100,
						      0, EMB_ARRAY_BUFFER);
	assert_that(result, is_equal_to(EMB_SUCCESS));
}

Ensure(emb_buffer_view_destroy_does_nothing)
{
	struct emb_buffer_view bview;
	emb_buffer_view_destroy(&bview);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("buffer view");
	add_test(suite, emb_buffer_view_init_returns_error_on_invalid_buffer);
	add_test(suite, emb_buffer_view_init_returns_error_on_invalid_handle);
	add_test(suite, emb_buffer_view_init_returns_error_on_invalid_range);
	add_test(suite, emb_buffer_view_init_allocates_resources_for_view);
	add_test(suite, emb_buffer_view_destroy_does_nothing);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
