/**
 * @file
 * Test suite for embaland buffer
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "embaland.h"
#include "buffer.h"
#include "instance.h"

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

Ensure(emb_buffer_init_returns_error_on_invalid_instance)
{
	struct emb_buffer buffer;
	enum emb_result result = emb_buffer_init(&buffer, NULL, NULL, 0);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_buffer_init_returns_error_on_invalid_handle)
{
	struct emb_instance emb = { NULL };
	enum emb_result result = emb_buffer_init(NULL, &emb, NULL, 0);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_buffer_init_allocates_resources_for_buffer)
{
	struct emb_instance emb = { NULL };
	struct emb_buffer buffer;
	enum emb_result result = emb_buffer_init(&buffer, &emb, NULL, 0);
	assert_that(result, is_equal_to(EMB_SUCCESS));
}

Ensure(emb_buffer_destroy_does_nothing)
{
	struct emb_buffer buffer;
	emb_buffer_destroy(&buffer);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("buffer");
	add_test(suite, emb_buffer_init_returns_error_on_invalid_instance);
	add_test(suite, emb_buffer_init_returns_error_on_invalid_handle);
	add_test(suite, emb_buffer_init_allocates_resources_for_buffer);
	add_test(suite, emb_buffer_destroy_does_nothing);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
