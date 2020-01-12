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

Ensure(emb_init_returns_error_on_invalid_vulkan_handle)
{
	struct emb_instance embaland = { NULL };
	enum emb_result result = emb_init(&embaland, VK_NULL_HANDLE);
	assert_that(result, is_equal_to(EMB_ERROR_INVALID_EXTERNAL_HANDLE));
}

Ensure(emb_init_returns_error_on_invalid_handle_pointer)
{
	VkInstance vulkan = ((VkInstance)1);

	enum emb_result result = emb_init(NULL, vulkan);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_init_allocates_new_embaland_instance)
{
	VkInstance vulkan = ((VkInstance)1);
	struct emb_instance embaland = { NULL };

	assert_that(emb_init(&embaland, vulkan), is_equal_to(EMB_SUCCESS));
	assert_that(embaland.vulkan, is_equal_to(vulkan));
}

Ensure(emb_destroy_does_nothing)
{
	emb_destroy(NULL);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("instance");
	add_test(suite, emb_init_returns_error_on_invalid_vulkan_handle);
	add_test(suite, emb_init_returns_error_on_invalid_handle_pointer);
	add_test(suite, emb_init_allocates_new_embaland_instance);
	add_test(suite, emb_destroy_does_nothing);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
