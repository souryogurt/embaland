#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "instance.h"
#include "vulkan.h"

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

Ensure(emb_init_returns_error)
{
	emb_instance embaland = NULL;
	emb_result result = emb_init(embaland);
	assert_that(result, is_equal_to(EMB_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_cleanup_does_nothing)
{
	emb_instance embaland = NULL;
	emb_cleanup(embaland);
}

Ensure(emb_get_vulkan_returns_instance)
{
	struct emb_instance embaland = {
		.vulkan = (VkInstance)1,
	};
	VkInstance result = VK_NULL_HANDLE;
	emb_result ret = emb_get_vulkan(&embaland, &result);
	assert_that(ret, is_equal_to(EMB_SUCCESS));
	assert_that(result, is_equal_to(embaland.vulkan));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("instance");
	add_test(suite, emb_init_returns_error);
	add_test(suite, emb_cleanup_does_nothing);
	add_test(suite, emb_get_vulkan_returns_instance);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
