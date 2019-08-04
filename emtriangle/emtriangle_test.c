/**
 * @file
 * Test suite for emtriangle
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "emtriangle.h"

#include <GLFW/glfw3.h>
#include <embaland/embaland.h>
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#define WINDOW ((GLFWwindow *)1)

Ensure(main_returns_zero_on_success)
{
	expect(glfwInit, will_return(1));
	expect(emb_create, will_return(EMB_SUCCESS));
	expect(glfwWindowHint, when(hint, is_equal_to(GLFW_CLIENT_API)),
	       when(value, is_equal_to(GLFW_NO_API)));
	expect(glfwCreateWindow, will_return(WINDOW),
	       when(width, is_equal_to(960)), when(height, is_equal_to(540)),
	       when(title, is_equal_to_string("Triangle")));

	expect(glfwWindowShouldClose, will_return(0));
	expect(glfwPollEvents);
	expect(glfwWindowShouldClose, will_return(1));

	expect(glfwDestroyWindow, when(window, is_equal_to(WINDOW)));
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

Ensure(main_returns_failure_on_window_creation_fail)
{
	expect(glfwInit, will_return(1));
	expect(emb_create, will_return(EMB_SUCCESS));
	expect(glfwWindowHint);
	expect(glfwCreateWindow, will_return(NULL));
	expect(emb_destroy);
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
	add_test(suite, main_returns_failure_on_window_creation_fail);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
