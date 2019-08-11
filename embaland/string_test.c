/**
 * @file
 * Test suite for base object
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "string.h"

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

Ensure(emb_string_fmt_returns_new_string)
{
	const char *str = emb_string_fmt("name_%d", 1);
	assert_that(str, is_equal_to_string("name_1"));

	free((void *)str);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("string");
	add_test(suite, emb_string_fmt_returns_new_string);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
