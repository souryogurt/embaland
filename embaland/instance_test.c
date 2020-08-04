#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "instance.h"
#include "viewport.h"

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

Ensure(emb_init_returns_error)
{
	struct emb_instance embaland = { NULL };
	VkInstance vulkan = ((VkInstance)1);
	VkResult result = emb_init(vulkan, &embaland);
	assert_that(result, is_equal_to(VK_ERROR_INITIALIZATION_FAILED));
}

Ensure(emb_cleanup_does_nothing)
{
	struct emb_instance embaland = { NULL };
	emb_cleanup(&embaland);
}

Ensure(emb_render_viewport_successes)
{
	struct emb_instance embaland = { NULL };
	struct emb_viewport viewport = { NULL };
	VkResult ret = emb_render_viewport(&embaland, &viewport, UINT64_MAX);
	assert_that(ret, is_equal_to(VK_SUCCESS));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("instance");
	add_test(suite, emb_init_returns_error);
	add_test(suite, emb_cleanup_does_nothing);
	add_test(suite, emb_render_viewport_successes);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
