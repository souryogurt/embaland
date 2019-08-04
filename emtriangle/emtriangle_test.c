/**
 * @file
 * Test suite for emtriangle
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "emtriangle.h"

#include <embaland/embaland.h>
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

Ensure(main_returns_zero_on_success)
{
	expect(glfwInit, will_return(1));
	expect(emb_create, will_return(EMB_SUCCESS));
	expect(emb_destroy);
	expect(glfwTerminate);
	assert_that(application_main(0, NULL), is_equal_to(EXIT_SUCCESS));
}

Ensure(main_returns_failure_on_glfw_initalization_fail)
{
	expect(glfwInit, will_return(0));
	never_expect(glfwTerminate);
	assert_that(application_main(0, NULL), is_equal_to(EXIT_FAILURE));
}

Ensure(main_returns_failure_on_embaland_initialization_fail)
{
	expect(glfwInit, will_return(1));
	expect(emb_create, will_return(EMB_ERROR_INITIALIZATION_FAILED));
	never_expect(emb_destroy);
	expect(glfwTerminate);
	assert_that(application_main(0, NULL), is_equal_to(EXIT_FAILURE));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("emtriangle");
	add_test(suite, main_returns_zero_on_success);
	add_test(suite, main_returns_failure_on_glfw_initalization_fail);
	add_test(suite, main_returns_failure_on_embaland_initialization_fail);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
