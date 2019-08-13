/**
 * @file
 * Test suite for embaland instance
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "embaland.h"
#include "instance.h"

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

Ensure(emb_create_always_returns_success)
{
	VkInstance vulkan = VK_NULL_HANDLE;
	emb_instance embaland = NULL;
	assert_that(emb_create(vulkan, &embaland), is_equal_to(EMB_SUCCESS));
}

Ensure(emb_destroy_does_not_do_anything)
{
	emb_instance embaland = NULL;
	emb_destroy(embaland);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("instance");
	add_test(suite, emb_create_always_returns_success);
	add_test(suite, emb_destroy_does_not_do_anything);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
